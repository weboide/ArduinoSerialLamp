
// halfwidth must be an odd number
void anim_wave(int halfwidth, int steps, int stepdelay) {
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

void anim_fade_blink(CHSV color, int stepdelay)
{
  for(int b = 1; b < 255; b++)
  {
    color.v = b;
    for(int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = color;
    }
    FastLED.show();
    delay(stepdelay);
  }

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

