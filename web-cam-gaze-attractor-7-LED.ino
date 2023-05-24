// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 7

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

int globalOffset;
const int daytimeBrightness = 200;
const int nighttimeBrightness = 50;

int currentBrightness = 0;

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP

  // init brightness
  flipDayNightBrightness();
  
  // daytime brightness:
//  strip.setBrightness(daytimeBrightness); // Set BRIGHTNESS to about 1/5 (max = 255)

  // night brightness:

  globalOffset=0;
}

// Would need an input switch to flip between day/night brightness.
void flipDayNightBrightness()
{
  if (currentBrightness < daytimeBrightness)
    currentBrightness = daytimeBrightness;
  else
    currentBrightness = nighttimeBrightness;
  
    strip.setBrightness(currentBrightness); // Set BRIGHTNESS to about 1/5 (max = 255)
}

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  // Fill along the length of the strip in various colors...
//  colorWipe(strip.Color(255,   0,   0), 50); // Red
//  colorWipe(strip.Color(  0, 255,   0), 50); // Green
//  colorWipe(strip.Color(  0,   0, 50), 50); // Blue
//  colorWipe(strip.Color(  50,   0, 50), 50); // Blue

  int ledWait=100;

  chaseStart(globalOffset,
        strip.Color(0,0,100),
        strip.Color(0,0,25),
        strip.Color(0,0,6),
        strip.Color(0,0,0),
        ledWait);

  int lightCentre=random(0,2);

  int chaseCount=random(1,8);
  while (chaseCount--)
  {
    if (lightCentre)
    {
      strip.setPixelColor(0,strip.Color(random(0,40),random(0,40),random(0,40)));
      strip.show();                          //  Update strip to match
    }
    chase(globalOffset,
          strip.Color(0,0,100),
          strip.Color(0,0,25),
          strip.Color(0,0,10),
          strip.Color(0,0,0),
          ledWait);
  }
  
  strip.setPixelColor(0,strip.Color(0,0,0));
  strip.show();                          //  Update strip to match
        

  chaseEnd(globalOffset,
        strip.Color(0,0,100),
        strip.Color(0,0,25),
        strip.Color(0,0,6),
        strip.Color(0,0,0),
        ledWait);

  globalOffset+=3;
  
  randomWait();

/*
        
//  randomWait();
        
  chase(strip.Color(0,0,100),
        strip.Color(0,0,25),
        strip.Color(6,0,0),
        strip.Color(0,0,0),
        ledWait);

//  randomWait();
*/
  // Do a theater marquee effect in various colors...
//  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
//  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

//  rainbow(10);             // Flowing rainbow cycle along the whole strip
//  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}

void randomWait()
{
//  FIRST VERSION: delay(random(30,100)*100);    // between 3 and 10 seconds
  delay(random(5,20)*100);    // between 500ms and 2 seconds
}

void chaseStart(int offset,
          uint32_t color1, 
          uint32_t color2, 
          uint32_t color3, 
          uint32_t color4, 
          int wait)
{
//  int i=(2+offset)%7;  i=(i?i:1);
  int i=addI(1,offset);

  // assumes start at all dark
  
  // frame 1: first led is at color3
  // frame 2: first led is at color3, second led is at color2
  // frame 3: first led is at color3, second led is at color2, color 1
    strip.setPixelColor(i,color3);
    i=incI(i);
//    i=(i+1)%7; i=(i?i:1);
    
    strip.show();                          //  Update strip to match
    delay(wait);
    strip.setPixelColor(i,color2);
    i=incI(i);
//    i=(i+1)%7; i=(i?i:1);
    strip.show();                          //  Update strip to match
    delay(wait);
    strip.setPixelColor(i,color1);
    i=incI(i);
//    i=(i+1)%7; i=(i?i:1);
    strip.show();                          //  Update strip to match
    delay(wait);
}

int incI(int i)
{
  return addI(i,1);
}

int addI(int i,int add)
{
  i=(i-1+add)%6;
  return (i ? i+1 : 1);
}

void chaseEnd(int offset,
          uint32_t color1, 
          uint32_t color2, 
          uint32_t color3, 
          uint32_t color4, 
          int wait)
{
  // assumes start at all dark
  
  // frame 0: first led is at color3, second led is at color2, second led is at color1,
  // frame 1: first led is at color4, second led is at color3, second led is at color2
  // frame 2: first led is at color4, second led is at color4, second led is at color3
  // frame 3: first led is at color4, second led is at color4, second led is at color4
  
//    int i=2;
  int i=addI(1,offset);
    strip.setPixelColor(i,color4);
    strip.setPixelColor(addI(i,1),color3);
    strip.setPixelColor(addI(i,2),color2);
    strip.show();                          //  Update strip to match
    delay(wait);

    i=incI(i);
    strip.setPixelColor(i,color4);
    strip.setPixelColor(addI(i,1),color3);
    strip.show();                          //  Update strip to match
    delay(wait);

    i=incI(i);
    strip.setPixelColor(i,color4);
    strip.show();                          //  Update strip to match
    delay(wait);

    // last one will go to color4, last but one will go to color 
    // last but one will go da
}

void chase(int offset,
          uint32_t color1, 
          uint32_t color2, 
          uint32_t color3, 
          uint32_t color4, 
          int wait)
{
    for(int i=1+offset; i<strip.numPixels()+offset; i++) 
    
    { // For each pixel in strip...
      strip.setPixelColor(addI(i,3), color1);         //  Set pixel's color (in RAM)
      strip.setPixelColor(addI(i,2), color2);         //  Set pixel's color (in RAM)
      strip.setPixelColor(addI(i,1), color3);         //  Set pixel's color (in RAM)
      strip.setPixelColor(addI(i,0), color4);         //  Set pixel's color (in RAM)

/*
      strip.setPixelColor((i+3)%6+1, color1);         //  Set pixel's color (in RAM)
      strip.setPixelColor((i+2)%6+1, color2);         //  Set pixel's color (in RAM)
      strip.setPixelColor((i+1)%6+1, color3);         //  Set pixel's color (in RAM)
      strip.setPixelColor((i+0)%6+1, color4);         //  Set pixel's color (in RAM)
*/
      strip.show();                          //  Update strip to match
      delay(wait);                           //  Pause for a moment
    }
}

void allOff()
{
  for(int i=0; i<strip.numPixels(); i++) 
  { // For each pixel in strip...
    strip.setPixelColor(i, 0);         //  Set pixel's color (in RAM)
  }
    strip.show();                          //  Update strip to match
}

// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
