

# Capture serial port

`(stty raw; cat > screenshot.eps) < /dev/ttyACM0`

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


