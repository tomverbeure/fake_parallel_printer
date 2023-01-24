#! /usr/bin/env python3

import sys
import getopt
import serial
import time


def usage():
    print(f"{sys.argv[0]} -p|--port= <serial port> -t <end of file timeout>")
    pass

def main():
    port = None
    timeout = 2
    prefix = "fake_printer_output_"
    suffix = "prtcap"
    page_nr = 0
    verbose = False

    try:
        optlist, args = getopt.getopt(sys.argv[1:], "hvp:t:s:n:f:", ["help", "verbose", "port=", "prefix=", "suffix=", "timeout=", "--pagenr="])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(1)

    for o, a in optlist:
        if o in ("-p", "--port"):
            port = a
        elif o in ("-t", "--timeout"):
            timeout = int(a)
        elif o in ("-f", "--prefix"):
            prefix = a
        elif o in ("-s", "--suffix"):
            suffix = a
        elif o in ("-n", "--pagenr"):
            page_nr = int(a)
        elif o in ("-v", "--verbose"):
            verbose = True

    if len(args) > 0:
        print(f"Unprocessed arguments: {args}")
        usage()
        sys.exit(1)

    if verbose:
        print("fake printer:")
        print(f"    verbose = {verbose}")
        print(f"    port    = {port}")
        print(f"    timeout = {timeout}")
        print(f"    prefix  = {prefix}")
        print(f"    suffix  = {suffix}")
        print(f"    page nr = {page_nr}")

    with serial.Serial(port, timeout=1) as ser:
        while True:
            # Not currently printing...
            d = ser.read(1024)
            last_received_time = time.time()

            if len(d) > 0:
                filename = f"{prefix}{page_nr}.{suffix}"
                print(f"Printer data received. Creating printer capture file {filename}.")

                bytes_received = len(d)

                with open(filename, "wb") as prt_file:
                    prt_file.write(d)

                    while time.time() < (last_received_time+timeout):
                        d = ser.read(1024)
                        if len(d) > 0:
                            if verbose:
                                print(".", end="")
                            sys.stdout.flush()
                            bytes_received += len(d)
                            prt_file.write(d)
                            last_received_time = time.time()

                    print()
                    print(f"{bytes_received} bytes received.")
                    print(f"No printer data received for {timeout} seconds. Closing printer capture file.")

                page_nr += 1

if __name__ == "__main__":
    main()

