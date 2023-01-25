# Notes

Stuff that needs to be cleaned up...

## Conversion TDS 420A - Encapsulated PostScript

* TDS 420A EPS-Color: [shift-hardcopy] 

    * Capture
     
        `(stty -echo raw; cat > screenshot.eps) < /dev/ttyACM0`

    * Settings
        * Format: EPS Color
        * Layout: Portrait (Yes! NOT landscape!)
        * Port: Centronics
    * Conversion: 

        `inkscape -f ./screenshot.eps -w 1280 -y=255 -e screenshot.png`

        If rotated because landscape was selected:

        `inkscape -f ./screenshot.eps -h 1280 -y=255 -e - | convert - -rotate 90  screenshot.png`

* TDS 420A EPS Mono Image: [shift-hardcopy] 

    * Capture

        `(stty -echo raw; cat > screenshot.eps) < /dev/ttyACM0`

    * Settings
        * Format: EPS Mono Image
        * Layout: Portrait 
        * Port: Centronics
    * Conversion

        `inkscape -f ./screenshot.eps -w 1280 -y=255 -e screenshot.png`

* Conversion TDS 420A - Epson

    * Capture

        `(stty -echo raw; cat > screenshot.epson) < /dev/ttyACM0`

    * Settings:
        * Format: Epson - 9 & 24 pin dot matrix printer
        * Layout: Landscape 
        * Port: Centronics
    * Typical capture file size: ~39KB
    * Conversion 

        `/opt/printerToPDF/bin/printerToPDF -o output -f /opt/printerToPDF/lib/PrinterToPDF/font2/Epson-Standard.C16 screenshot.epson`

# Advantest PCL to PDF

* [Ghostscript](https://ghostscript.com/releases/gpcldnld.html)

    `~/tools/ghostpcl-10.0.0-linux-x86_64/gpcl6-1000-linux-x86_64 -dNOPAUSE -sOutputFile=test.png -sDEVICE=png256 -g2000x1500 -r300x300  sa_pcl_s_color_s_a.pcl`

    Best result so far:

    `~/tools/ghostpcl-10.0.0-linux-x86_64/gpcl6-1000-linux-x86_64 -dNOPAUSE -sOutputFile=test.png -sDEVICE=png256 -g8000x8000 -r600x600 sa_pcl_s_color_s_a.pcl`

    `convert test.png -filter point -resize 2000  test2.png`

* [PrinterToPDF](https://github.com/tomverbeure/PrinterToPDF)

    `/opt/printerToPDF/bin/printerToPDF -o output -f /opt/printerToPDF/lib/PrinterToPDF/font2/Epson-Standard.C16  dump.epson`

# Misc

* [Tweet by Sell My Retro](https://twitter.com/SellMyRetro/status/1614215674024398848)
	

# CircuitPython


* Download UF2: https://circuitpython.org/board/raspberry_pi_pico/
* Terminal: `picocom /dev/ttyACM0`

    Gives a command line

    Ctrl-A Ctrl-X to exit picocom

* Copy `code.py` to CIRCUITPY drive and power cycle -> your code will run

# Protocol Compatibility mode

host to device:

* nINIT: pulsed low to reset device.
* nSELIN: driven low by host to activate the device
* nAUTOF: driven low by host to enable auto-line feed. Ignore?

* nSTROBE: set low by host when data is valid. Used to store data into an latch
* D[7:0]: data must be stable as long as nSTROBE is low

device to host:

* nACK: Pulsed low by device to acknowledge received of data. Length of pulse?
    150kB/s -> 6uS is upper bound length. Let's do 2uS.
* BUSY: driven high by device to indicate that device is not ready. 
* nERROR: driven low by device to indicate an error.
* SEL: driven high by device to indicate that device is present.
* PE (Paper Error): driven high by device to indicate a paper error

# References

* [Parallel-Port Interface Powers Low Voltage Systems](https://www.analog.com/en/design-notes/parallelport-interface-powers-low-voltage-systems.html)

    Not useful: voltage regulator only goes up to +2V.

    * [Maxim MAX1841 datasheet](https://www.mouser.com/datasheet/2/256/MAX1840-MAX1841-1308121.pdf)

* [Arduino: Read data from parallel port](https://forum.arduino.cc/t/read-data-from-parallel-port/105648/15)

 * [Rue's Parallel2Serial](https://github.com/ruenahcmohr/Parallel2Serial)


