#include "FastLED.h"

#define NUM_LEDS 16
#define DATA_PIN A0

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


int mode = 0;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  inputString.reserve(200);

  // initialize random
  randomSeed(analogRead(A5));

  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  FastLED.addLeds<NEOPIXEL, DATA_PIN, GRB>(leds, NUM_LEDS);

  fill_rainbow(&leds[0], 16, 0, 16);
  FastLED.show();
  delay(400);
  fade_all_to_black(10);
}

void loop() {
  if (stringComplete) {
    Serial.print("Received: ");
    Serial.println(inputString);
    processCommand(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  int anim = random(1,3);

  switch(anim)
  {
  case 1:
    wave(5, 60, 4);
    break;

  case 2:
    anim_all_rainbow(300, 20); // 10 seconds
    break;


  }

  fade_all_to_black(10);

  Serial.println("-- Loop --");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

void processCommand(String command)
{
  if(command.startsWith("mode"))
  {
    if(command.length() > 4)
      mode = command.substring(5).toInt();
    Serial.print("Mode=");
    Serial.println(mode);
  }
}

void anim_all_rainbow(int steps, int stepdelay)
{
  CHSV start (0, 255, 255);
  while(steps-- > 0)
  {
    start.hue += 1;
    fill_solid(&leds[0], 16, start);
    FastLED.show();
    delay(stepdelay);
  }
}

void fade_all_to_black(int stepdelay)
{
  boolean done = false;
  while(!done)
  {
    done = true; // set to true and see if we're done below
    for(int i = 0; i < NUM_LEDS; i++)
    {
      leds[i].fadeToBlackBy(1);
      if(leds[i])
        done = false;

    }
    FastLED.show();
    delay(stepdelay);
  }
}

// halfwidth must be an odd number
void wave(int halfwidth, int steps, int stepdelay) {
  int current = halfwidth;
  uint32_t currentvalue = 0, tmpvalue = 0;

  // set up initial layout
  FastLED.clear(); 
  for(int i = 1; i <= halfwidth; i++)
  {
    leds[current+i].red = 255*(halfwidth-i)/halfwidth;
    leds[current-i].red = 255*(halfwidth-i)/halfwidth;
  }
  leds[current].red = 255;
  FastLED.show();

  // go to next led and start the loop
  current++;
  while(current < steps)
  {
    //Serial.print("Current: ");
    //Serial.println(current);
    while(leds[current%NUM_LEDS].red < 255) // wait till current red is at 255
    {
      // make previous halfwidth darker
      for(int i = halfwidth; i >= 1; i--)
      {
        leds[(current-i)%NUM_LEDS].red--;
      }

      // make current and next halfwidth brighter
      for(int i = 0; i <= halfwidth-1; i++)
      {
        leds[(current+i)%NUM_LEDS].red++;
      }


      FastLED.show();
      //Serial.println();
      delay(stepdelay);
    }

    current++;
  }

  // reset minimum value to all leds
  for(int i = 0; i < NUM_LEDS; i++)
    leds[i] |= CRGB(0, 0, 0);
}

