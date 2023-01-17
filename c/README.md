

# Capture serial port

`(stty -echo raw; cat > screenshot.eps) < /dev/ttyACM0`

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

# Conversion for Advantest R3273

    * ESC/P Gray: corruption seen
    * ESC/P Mono S: corruption seen
    * ESC/P Mono L: no corruption seen


# PCL to PDF

* [Ghostscript](https://ghostscript.com/releases/gpcldnld.html)

    `~/tools/ghostpcl-10.0.0-linux-x86_64/gpcl6-1000-linux-x86_64 -dNOPAUSE -sOutputFile=test.png -sDEVICE=png256 -g2000x1500 -r300x300  sa_pcl_s_color_s_a.pcl`

    Best result so far:

    `~/tools/ghostpcl-10.0.0-linux-x86_64/gpcl6-1000-linux-x86_64 -dNOPAUSE -sOutputFile=test.png -sDEVICE=png256 -g8000x8000 -r600x600 sa_pcl_s_color_s_a.pcl`

    `convert test.png -filter point -resize 2000  test2.png`

* [PrinterToPDF](https://github.com/tomverbeure/PrinterToPDF)

    `/opt/printerToPDF/bin/printerToPDF -o output -f /opt/printerToPDF/lib/PrinterToPDF/font2/Epson-Standard.C16  dump.epson`

    [Tweet by Sell My Retro](https://twitter.com/SellMyRetro/status/1614215674024398848)
	
