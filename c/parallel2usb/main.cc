#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "hardware/clocks.h"

#define ACK_DURATION_US         2

// Pins are defined in
// .../pico-sdk/src/boards/include/boards/pico.h
#define LED_PIN         PICO_DEFAULT_LED_PIN

#define REV_PIN         28

#define JUMPER_PIN_V1   1
#define JUMPER_PIN_V2   21

//============================================================
// 74LVC161284 control
//============================================================

// HD
// 1 -> totempole output high drive
// 0 -> pullup high drive
// A value of 1 will result in much faster rising edges of BUSY and nACK.
#define HD_PIN_V1       17  
#define HD_PIN_V2       16  

#define HD_PULLUP       0 
#define HD_HIGH_DRIVE   1 

// DIR=0 -> data from host to device. 1 -> from device to host
#define DIR_PIN_V1      16
#define DIR_PIN_V2      17

#define DIR_HOST_TO_PRINTER     0
#define DIR_PRINTER_TO_HOST     1

//============================================================
// Host to device 
//============================================================

#define nSTROBE_PIN_V1  15
#define nAUTOF_PIN_V1   14
#define nINIT_PIN_V1    13
#define nSELIN_PIN_V1   12

#define nSTROBE_PIN_V2  11
#define nAUTOF_PIN_V2   0
#define nINIT_PIN_V2    1
#define nSELIN_PIN_V2   2

#define D0_PIN_V1       11
#define D1_PIN_V1       10
#define D2_PIN_V1       9
#define D3_PIN_V1       8
#define D4_PIN_V1       7
#define D5_PIN_V1       6
#define D6_PIN_V1       5
#define D7_PIN_V1       4

#define D0_PIN_V2       3
#define D1_PIN_V2       4
#define D2_PIN_V2       5
#define D3_PIN_V2       6
#define D4_PIN_V2       7
#define D5_PIN_V2       8
#define D6_PIN_V2       9
#define D7_PIN_V2       10

//============================================================
// Device to host
//============================================================

#define nERROR_PIN_V1   22
#define nACK_PIN_V1     21
#define BUSY_PIN_V1     20
#define PE_PIN_V1       19
#define SEL_PIN_V1      18

#define nERROR_PIN_V2   12
#define nACK_PIN_V2     13
#define BUSY_PIN_V2     14
#define PE_PIN_V2       15
#define SEL_PIN_V2      18

//============================================================
// Debug pins
//============================================================

#define DEBUG0_PIN      26
#define DEBUG1_PIN      27
#define DEBUG2_PIN      28

int board_rev;

int jumper_pin;

int dir_pin;
int hd_pin;

int n_strobe_pin;
int n_autof_pin;
int n_init_pin;
int n_selin_pin;

int d0_pin;
int d1_pin;
int d2_pin;
int d3_pin;
int d4_pin;
int d5_pin;
int d6_pin;
int d7_pin;

int n_error_pin;
int n_ack_pin;
int busy_pin;
int pe_pin;
int sel_pin;


void setup_ios()
{
    // The REV pin (GPIO28) is used to differentiate between revision
    // 1 and 2 of the PCB. 
    // Revision 1 of the PCB has this pin floating while revision 2 has
    // it strapped to ground. If we enable the pullup on this GPIO,
    // this pin will read back a logic value of 1 for revision 1 and a
    // value of 0 for revision 2.
    gpio_init(REV_PIN); 
    gpio_set_dir(REV_PIN, GPIO_IN);
    sleep_us(10);
    gpio_pull_up(REV_PIN);

    board_rev = gpio_get(REV_PIN) == 1 ? 1 : 2;

    // Assign pin function to pin number based on board revision.
    jumper_pin    = board_rev == 1 ? JUMPER_PIN_V1  : JUMPER_PIN_V2; 

    dir_pin       = board_rev == 1 ? DIR_PIN_V1     : DIR_PIN_V2; 
    hd_pin        = board_rev == 1 ? HD_PIN_V1      : HD_PIN_V2; 

    d0_pin        = board_rev == 1 ? D0_PIN_V1      : D0_PIN_V2;
    d1_pin        = board_rev == 1 ? D1_PIN_V1      : D1_PIN_V2;
    d2_pin        = board_rev == 1 ? D2_PIN_V1      : D2_PIN_V2;
    d3_pin        = board_rev == 1 ? D3_PIN_V1      : D3_PIN_V2;
    d4_pin        = board_rev == 1 ? D4_PIN_V1      : D4_PIN_V2;
    d5_pin        = board_rev == 1 ? D5_PIN_V1      : D5_PIN_V2;
    d6_pin        = board_rev == 1 ? D6_PIN_V1      : D6_PIN_V2;
    d7_pin        = board_rev == 1 ? D7_PIN_V1      : D7_PIN_V2;

    n_strobe_pin  = board_rev == 1 ? nSTROBE_PIN_V1 : nSTROBE_PIN_V2; 
    n_autof_pin   = board_rev == 1 ? nAUTOF_PIN_V1  : nAUTOF_PIN_V2; 
    n_init_pin    = board_rev == 1 ? nINIT_PIN_V1   : nINIT_PIN_V2; 
    n_selin_pin   = board_rev == 1 ? nSELIN_PIN_V1  : nSELIN_PIN_V2; 

    n_error_pin   = board_rev == 1 ? nERROR_PIN_V1  : nERROR_PIN_V2; 
    busy_pin      = board_rev == 1 ? BUSY_PIN_V1    : BUSY_PIN_V2; 
    n_ack_pin     = board_rev == 1 ? nACK_PIN_V1    : nACK_PIN_V2; 
    pe_pin        = board_rev == 1 ? PE_PIN_V1      : PE_PIN_V2; 
    sel_pin       = board_rev == 1 ? SEL_PIN_V1     : SEL_PIN_V2; 

    // Blink LED at bootup. It's just the right thing to do.
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
    gpio_put(LED_PIN, 1);

    gpio_init(dir_pin);
    gpio_set_dir(dir_pin, GPIO_OUT);
    gpio_put(dir_pin, DIR_HOST_TO_PRINTER);

    gpio_init(hd_pin);
    gpio_set_dir(hd_pin, GPIO_OUT);
    gpio_put(hd_pin, HD_HIGH_DRIVE);

    gpio_init(jumper_pin);
    gpio_set_dir(jumper_pin, GPIO_IN);
    gpio_pull_up(jumper_pin);

    gpio_init(n_strobe_pin);
    gpio_set_dir(n_strobe_pin, GPIO_IN);

    gpio_init(n_autof_pin);
    gpio_set_dir(n_autof_pin, GPIO_IN);

    gpio_init(n_init_pin);
    gpio_set_dir(n_init_pin, GPIO_IN);

    gpio_init(n_selin_pin);
    gpio_set_dir(n_selin_pin, GPIO_IN);

    gpio_init(d0_pin);
    gpio_init(d1_pin);
    gpio_init(d2_pin);
    gpio_init(d3_pin);
    gpio_init(d4_pin);
    gpio_init(d5_pin);
    gpio_init(d6_pin);
    gpio_init(d7_pin);

    gpio_set_dir(d0_pin, GPIO_IN);
    gpio_set_dir(d1_pin, GPIO_IN);
    gpio_set_dir(d2_pin, GPIO_IN);
    gpio_set_dir(d3_pin, GPIO_IN);
    gpio_set_dir(d4_pin, GPIO_IN);
    gpio_set_dir(d5_pin, GPIO_IN);
    gpio_set_dir(d6_pin, GPIO_IN);
    gpio_set_dir(d7_pin, GPIO_IN);

    gpio_init(n_error_pin);
    gpio_set_dir(n_error_pin, GPIO_OUT);
    gpio_put(n_error_pin, 1);

    gpio_init(n_ack_pin);
    gpio_set_dir(n_ack_pin, GPIO_OUT);
    gpio_put(n_ack_pin, 1);

    gpio_init(busy_pin);
    gpio_set_dir(busy_pin, GPIO_OUT);
    gpio_put(busy_pin, 0);

    gpio_init(pe_pin);
    gpio_set_dir(pe_pin, GPIO_OUT);
    gpio_put(pe_pin, 0);

    gpio_init(sel_pin);
    gpio_set_dir(sel_pin, GPIO_OUT);
    gpio_put(sel_pin, 1);

    gpio_init(DEBUG0_PIN);
    gpio_set_dir(DEBUG0_PIN, GPIO_OUT);
    gpio_put(DEBUG0_PIN, 0);

    gpio_init(DEBUG1_PIN);
    gpio_set_dir(DEBUG1_PIN, GPIO_OUT);
    gpio_put(DEBUG1_PIN, 0);

    gpio_init(DEBUG2_PIN);
    gpio_set_dir(DEBUG2_PIN, GPIO_OUT);
    gpio_put(DEBUG2_PIN, 0);
}

uint8_t get_data()
{
    uint8_t d = (gpio_get(d7_pin)<<7) |
                (gpio_get(d6_pin)<<6) |
                (gpio_get(d5_pin)<<5) |
                (gpio_get(d4_pin)<<4) |
                (gpio_get(d3_pin)<<3) |
                (gpio_get(d2_pin)<<2) |
                (gpio_get(d1_pin)<<1) |
                 gpio_get(d0_pin);

    return d;
}

int nr_bytes = 0;
uint32_t checksum = 0;

// __not_in_flash_func attribute increase the CPU performance
// The goal was to make sure that BUSY could be asserted before nSTROBE gets
// deasserted (which can happen after only 500ns.)
//
// However, testing on my equipment showed that printing works even when you never
// assert BUSY at all.

volatile bool strobe_detect = false;
volatile bool error         = false;
static void __not_in_flash_func(strobe_callback)()
{
        gpio_put(busy_pin, 1);
        gpio_put(DEBUG0_PIN, 1);

        uint8_t d = get_data();
        multicore_fifo_push_blocking(d);
        checksum += d;
        ++nr_bytes;

        gpio_put(busy_pin, 0);

        // Pulse ACK low.
        gpio_put(n_ack_pin, 0);
        sleep_us(ACK_DURATION_US);
        gpio_put(n_ack_pin, 1);

        gpio_put(DEBUG0_PIN, 0);
        gpio_acknowledge_irq(n_strobe_pin, GPIO_IRQ_EDGE_FALL);
}

void core1_main()
{
    setup_ios();

    irq_set_exclusive_handler(IO_IRQ_BANK0, strobe_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
    irq_set_priority(IO_IRQ_BANK0, PICO_HIGHEST_IRQ_PRIORITY);

    gpio_set_irq_enabled(n_strobe_pin, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(n_strobe_pin, GPIO_IRQ_EDGE_RISE, false);
}

int main() {
    stdio_init_all();
    stdio_set_translate_crlf(&stdio_usb, false);

    multicore_launch_core1(core1_main);

    while(1){
        uint8_t c = multicore_fifo_pop_blocking();
        putchar(c);

        // Plugging in the jumper prints this out this debug information.
        // If you do this while the host is printing, the print data will
        // be corrupted..
        if (gpio_get(jumper_pin) == 0){
            printf("sum: %d, nr_bytes: %d\n", checksum, nr_bytes);
            checksum = 0;
            nr_bytes = 0;

            // These delays are a crude debounce...
            sleep_ms(200);
            while(gpio_get(jumper_pin) == 0)
                ;
            sleep_ms(200);
        }
    }
}



