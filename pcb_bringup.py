# Micropython script to bring up PCV v1.

import board
import digitalio
import time
import supervisor

supervisor.disable_autoreload()

#if False:
#    f = open("test.log", "a")
#    f.write("Hello world!")
#    f.close()

led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

test_pin = digitalio.DigitalInOut(board.GP0)
test_pin.direction = digitalio.Direction.OUTPUT

# high_drive. When 1, outputs are in totem-pole mode, otherwise
# they are in open-drain mode.
hd_pin = digitalio.DigitalInOut(board.GP17)
hd_pin.direction = digitalio.Direction.OUTPUT
hd_pin.value = False

# direction: when high, then direction is from connector to Pico
dir_pin = digitalio.DigitalInOut(board.GP16)
dir_pin.direction = digitalio.Direction.OUTPUT

# For now, test other direction...
dir_pin.value = False

#============================================================
# Control: from host to printer
#============================================================
strobe_pin = digitalio.DigitalInOut(board.GP15)
strobe_pin.direction = digitalio.Direction.INPUT

autof_pin = digitalio.DigitalInOut(board.GP14)
autof_pin.direction = digitalio.Direction.INPUT

init_pin = digitalio.DigitalInOut(board.GP13)
init_pin.direction = digitalio.Direction.INPUT

selin_pin = digitalio.DigitalInOut(board.GP12)
selin_pin.direction = digitalio.Direction.INPUT

#============================================================
# Data: from host to printer
#============================================================

d0_pin = digitalio.DigitalInOut(board.GP11)
d0_pin.direction = digitalio.Direction.INPUT

d1_pin = digitalio.DigitalInOut(board.GP10)
d1_pin.direction = digitalio.Direction.INPUT

d2_pin = digitalio.DigitalInOut(board.GP9)
d2_pin.direction = digitalio.Direction.INPUT

d3_pin = digitalio.DigitalInOut(board.GP8)
d3_pin.direction = digitalio.Direction.INPUT

d4_pin = digitalio.DigitalInOut(board.GP7)
d4_pin.direction = digitalio.Direction.INPUT

d5_pin = digitalio.DigitalInOut(board.GP6)
d5_pin.direction = digitalio.Direction.INPUT

d6_pin = digitalio.DigitalInOut(board.GP5)
d6_pin.direction = digitalio.Direction.INPUT

d7_pin = digitalio.DigitalInOut(board.GP4)
d7_pin.direction = digitalio.Direction.INPUT

#============================================================
# Status: from printer to host
#============================================================
error_pin = digitalio.DigitalInOut(board.GP22)
error_pin.direction = digitalio.Direction.OUTPUT
error_pin.value = False

ack_pin = digitalio.DigitalInOut(board.GP21)
ack_pin.direction = digitalio.Direction.OUTPUT
ack_pin.value   = True

busy_pin = digitalio.DigitalInOut(board.GP20)
busy_pin.direction = digitalio.Direction.OUTPUT
busy_pin.value  = False

pe_pin = digitalio.DigitalInOut(board.GP19)
pe_pin.direction = digitalio.Direction.OUTPUT
pe_pin.value    = True

sel_pin = digitalio.DigitalInOut(board.GP18)
sel_pin.direction = digitalio.Direction.OUTPUT
sel_pin.value   = True

all_inputs = [ d0_pin, d1_pin, d2_pin, d3_pin, d4_pin, d5_pin, d6_pin, d7_pin, strobe_pin, autof_pin, init_pin, selin_pin ]

def test_inputs():
    val = 0
    for i in all_inputs:
        val = (val << 1) | i.value

    print(f"%04x" % (val))

if False:
    led.value = False
    print("1")
    time.sleep(1)
    led.value = True
    print("2")
    time.sleep(1)
    led.value = False
    print("3")
    time.sleep(1)
    led.value = True
    print("4")
   
while False:
    led.value           = True
    test_pin.value      = True
    sel_pin.value       = True
    time.sleep(1)
    test_inputs()
    led.value           = False
    test_pin.value      = False
    sel_pin.value       = False
    time.sleep(1)
    test_inputs()
    pass

def get_data():
    b = ((((((((((((((
            d7_pin.value) << 1) | 
            d6_pin.value) << 1) | 
            d5_pin.value) << 1) | 
            d4_pin.value) << 1) | 
            d3_pin.value) << 1) | 
            d2_pin.value) << 1) | 
            d1_pin.value) << 1) | d0_pin.value

    return b

sel_pin.value   = True
pe_pin.value    = False
ack_pin.value   = True
busy_pin.value  = False
error_pin.value = False

print("Listening...")

time.sleep(0.5)
ack_pin.value   = False
busy_pin.value  = True
error_pin.value  = True
time.sleep(0.5)
ack_pin.value   = True
busy_pin.value  = False
error_pin.value  = True

pe_pin.value = False
error_pin.value  = True

while True:
    print(f"nStrobe: %d, nAUTOF: %d, nINIT: %d, nSELIN: %d, data: %d" % 
            (strobe_pin.value, autof_pin.value, init_pin.value, selin_pin.value, get_data()))

