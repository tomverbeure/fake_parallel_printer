
# CircuitPython


* Download UF2: https://circuitpython.org/board/raspberry_pi_pico/
* Terminal: `picocom /dev/ttyACM0`

    Gives a command line

    Ctrl-A Ctrl-X to exit picocom

* Copy `code.py` to CIRCUITPY drive and power cycle -> your code will run

# Protocol

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
* ERROR: driven high by device to indicate an error.
* SEL: driven high by device to indicate that device is present.
* PE: driven high by device to indicate that the device is powered on.


# References

* [Parallel-Port Interface Powers Low Voltage Systems](https://www.analog.com/en/design-notes/parallelport-interface-powers-low-voltage-systems.html)

    Not useful: voltage regulator only goes up to +2V.

    * [Maxim MAX1841 datasheet](https://www.mouser.com/datasheet/2/256/MAX1840-MAX1841-1308121.pdf)

* [Arduino: Read data from parallel port](https://forum.arduino.cc/t/read-data-from-parallel-port/105648/15)

 * [Rue's Parallel2Serial](https://github.com/ruenahcmohr/Parallel2Serial)
