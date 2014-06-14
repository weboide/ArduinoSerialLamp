#include "FastLED.h"
#include "SerialCommand.h"

#define NUM_LEDS 16
#define LEDS_DATA_PIN A0
#define RANDOMSEED_PIN A5

SerialCommand SCmd;

byte mode = 0;
byte anim = 0;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200); // keep serial high to not slow down animations too much when printing at the same time

  // set up serial commands callbacks
  SCmd.addCommand("mode",cmd_mode);
  SCmd.addCommand("anim",cmd_anim);
  SCmd.addDefaultHandler(cmd_unknown);
  SCmd.setEOL('\n');

  // initialize random
  randomSeed(analogRead(RANDOMSEED_PIN));

  FastLED.addLeds<NEOPIXEL, LEDS_DATA_PIN, GRB>(leds, NUM_LEDS);

  fill_rainbow(&leds[0], 16, 0, 16);
  FastLED.show();
  delay(400);
  fade_all_to_black(10);
}

void loop() {
  // Read Serial Com
  SCmd.readSerial();

  // if mode is 0, then pick a random animation
  if(mode == 0)
    anim = random(1,3);
  if(mode == 1)
    anim = 0;

  switch(anim)
  {

  case 0:
  default:
    delay(500);
    break;
  
  case 1:
    anim_wave(5, 60, 4);
    break;

  case 2:
    anim_all_rainbow(300, 20);
    break;

  case 3:
    anim_fade_blink(CHSV(144, 255, 1), 5);
    break;
  }

  if(anim > 0)
    fade_all_to_black(10);

  Serial.println("-- Loop! --");
}

void cmd_unknown()
{
  Serial.println("400 Bad Request");
}

void cmd_mode()
{
  char *arg;  
  arg = SCmd.next();
  if(arg != NULL)
  {
    mode = atoi(arg);
    Serial.print("200 Mode set to: ");
    Serial.println(mode);  
  }
}

void cmd_anim()
{
  char *arg;  
  arg = SCmd.next();
  if(arg != NULL)
  {
    mode = 2;
    anim = atoi(arg);
    Serial.print("200 Anim set to: ");
    Serial.println(anim);
  }
}
