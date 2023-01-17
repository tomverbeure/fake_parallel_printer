#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/clocks.h"

// Pins are defined in
// .../pico-sdk/src/boards/include/boards/pico.h
#define LED_PIN         PICO_DEFAULT_LED_PIN

#define JUMPER_PIN      1

//============================================================
// 74LVC161284 control
//============================================================

// HD=1 -> totempole output high drive. 0 -> pullup high drive
#define HD_PIN          17  
// DIR=0 -> data from host to device. 1 -> from device to host
#define DIR_PIN         16

//============================================================
// Host to device 
//============================================================

#define nSTROBE_PIN     15
#define nAUTOF_PIN      14
#define nINIT_PIN       13
#define nSELIN          12

#define D0_PIN          11
#define D1_PIN          10
#define D2_PIN          9
#define D3_PIN          8
#define D4_PIN          7
#define D5_PIN          6
#define D6_PIN          5
#define D7_PIN          4

//============================================================
// Device to host
//============================================================

#define nERROR_PIN      22
#define nACK_PIN        21
#define BUSY_PIN        20
#define PE_PIN          19
#define SEL_PIN         18

void setup_ios()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
    gpio_put(LED_PIN, 1);

    gpio_init(JUMPER_PIN);
    gpio_set_dir(JUMPER_PIN, GPIO_IN);
    gpio_set_pulls(JUMPER_PIN, true, false);     // Pullup

    gpio_init(nSTROBE_PIN);
    gpio_set_dir(nSTROBE_PIN, GPIO_IN);

    gpio_init(nAUTOF_PIN);
    gpio_set_dir(nAUTOF_PIN, GPIO_IN);

    gpio_init(nINIT_PIN);
    gpio_set_dir(nINIT_PIN, GPIO_IN);

    gpio_init(D0_PIN);
    gpio_init(D1_PIN);
    gpio_init(D2_PIN);
    gpio_init(D3_PIN);
    gpio_init(D4_PIN);
    gpio_init(D5_PIN);
    gpio_init(D6_PIN);
    gpio_init(D7_PIN);

    gpio_set_dir(D0_PIN, GPIO_IN);
    gpio_set_dir(D1_PIN, GPIO_IN);
    gpio_set_dir(D2_PIN, GPIO_IN);
    gpio_set_dir(D3_PIN, GPIO_IN);
    gpio_set_dir(D4_PIN, GPIO_IN);
    gpio_set_dir(D5_PIN, GPIO_IN);
    gpio_set_dir(D6_PIN, GPIO_IN);
    gpio_set_dir(D7_PIN, GPIO_IN);

    gpio_init(nERROR_PIN);
    gpio_set_dir(nERROR_PIN, GPIO_OUT);
    gpio_put(nERROR_PIN, 1);

    gpio_init(nACK_PIN);
    gpio_set_dir(nACK_PIN, GPIO_OUT);
    gpio_put(nACK_PIN, 1);

    gpio_init(BUSY_PIN);
    gpio_set_dir(BUSY_PIN, GPIO_OUT);
    gpio_put(BUSY_PIN, 0);

    gpio_init(PE_PIN);
    gpio_set_dir(PE_PIN, GPIO_OUT);
    gpio_put(PE_PIN, 0);

    gpio_init(SEL_PIN);
    gpio_set_dir(SEL_PIN, GPIO_OUT);
    gpio_put(SEL_PIN, 1);
}

uint8_t get_data()
{
    uint8_t d = (gpio_get(D7_PIN)<<7) |
                (gpio_get(D6_PIN)<<6) |
                (gpio_get(D5_PIN)<<5) |
                (gpio_get(D4_PIN)<<4) |
                (gpio_get(D3_PIN)<<3) |
                (gpio_get(D2_PIN)<<2) |
                (gpio_get(D1_PIN)<<1) |
                 gpio_get(D0_PIN);

    return d;
}


bool strobe_triggered = false;
int nr_bytes = 0;
uint32_t checksum = 0;

static void __not_in_flash_func(strobe_callback)()
{
        gpio_put(BUSY_PIN, 1);
        gpio_acknowledge_irq(nSTROBE_PIN, GPIO_IRQ_EDGE_FALL);
        uint8_t d = get_data();
        sleep_us(1);
        gpio_put(BUSY_PIN, 0);
        sleep_us(1);
        putchar(d);
        checksum += d;
        ++nr_bytes;
        gpio_put(nACK_PIN, 0);
        sleep_us(1);
        gpio_put(nACK_PIN, 1);
}

int main() {
    stdio_init_all();
    stdio_set_translate_crlf(&stdio_usb, false);
    setup_ios();

    gpio_set_irq_enabled(nSTROBE_PIN, GPIO_IRQ_EDGE_FALL, true);
    irq_set_exclusive_handler(IO_IRQ_BANK0, strobe_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);

    while(1){
//        tight_loop_contents();
        if (gpio_get(JUMPER_PIN) == 0){
            printf("sum: %d, nr_bytes: %d\n", checksum, nr_bytes);
            checksum = 0;
            nr_bytes = 0;

            sleep_ms(100);
            while(gpio_get(JUMPER_PIN) == 0)
                ;
            sleep_ms(100);
        }
    }
}



