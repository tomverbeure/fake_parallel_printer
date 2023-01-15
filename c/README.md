

# Capture serial port

```
(stty raw; cat > myfile.txt) < /dev/ttyACM0
```

# Conversion TDS 420A

* TDS 420A EPS-Color: [shift-hardcopy] 

    * Format: EPS Color
    * Layout: Portrait (Yes! NOT landscape!)
    * Port: Centronics
    * Conversion: 

        `inkscape -f ./screenshot.eps -w 1280 -y=255 -e screenshot.png`

    * If rotated because landscape was selected:

        `inkscape -f ./screenshot.eps -h 1280 -y=255 -e - | convert - -rotate 90  screenshot.png`

* TDS 420A EPS Mono Image: [shift-hardcopy] 

    * Format: EPS Mono Image
    * Layout: Portrait 
    * Port: Centronics
    * Conversion: 

        `inkscape -f ./screenshot.eps -w 1280 -y=255 -e screenshot.png`


# Pico SDK example:

* github: pico-example
* build everything:

    ```sh
cd pico-example
mkdir build
cd build
cmake ..
make -j 4
    ```

* build ws2812 example:

    ```sh
cd pico-example
mkdir build
cd build
cmake ..
cd pio/ws2812
make
    ```

* 2 binaries: 
    * pio_ws2812.uf2: CPU feeds LED samples one by one to PIO. Slower.
    * pio_ws2812_parallel.uf2: Uses DMA to feed the PIOs without blocking the PCU. Faster, more complex.

For now, only focussing on the non-parallel option...

* Adapt the source code: ws2812.c

    * Change #define IS_RGBW from true to false (otherwise, colors are all wrong)
    * Change #define NUM_PIXELS from 150 to 420
    * Change #define WS2812_PIN from 2 to  0
    * Change put_pixel as follows:

    ```c
static inline void put_pixel(uint32_t pixel_grb) {
    uint16_t r = (pixel_grb >> 8) & 255;
    uint16_t g = (pixel_grb >> 16) & 255;
    uint16_t b = pixel_grb & 255;

    const int max_val = 25;

    r = (r * max_val) >> 8;
    g = (g * max_val) >> 8;
    b = (b * max_val) >> 8;

    pixel_grb = urgb_u32(r,g,b);

    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}
    ```

    This reduces the power consumption by a factor of 10. If you don't do this, 
    the power converter will get very hot and it will probably fail eventually...

* For additional debugging, enable STDIO on the USB port by adding one line to
  CMakeLists.txt:

    ```
target_link_libraries(pio_ws2812 PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(pio_ws2812)
>pico_enable_stdio_usb(pio_ws2812 1)
    ```

    You will need to rerun `cmake ..` in the `./build` directory and recompile.
    
    After that, you can connect to the serial USB port with `screen /dev/ttyACM0`.
    Exit screen with CTRL-A K.  


