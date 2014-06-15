

// halfwidth must be an odd number
void anim_wave(int halfwidth, int steps, int stepdelay) {
  int current = halfwidth;
  uint8_t hue = 200;
  uint8_t saturation = 255;
  uint8_t values[NUM_LEDS];
  uint32_t currentvalue = 0, tmpvalue = 0;

  for(int i = 0; i < NUM_LEDS; i++)
    values[i] = 1;

  // set up initial layout
  FastLED.clear(); 
  for(int i = 1; i <= halfwidth; i++)
  {
    values[current+i] = 255*(halfwidth-i)/halfwidth;
    values[current-i] = 255*(halfwidth-i)/halfwidth;
  }
  values[current] = 255;

  // show our initial set up
  for(int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV(hue, saturation, values[i]);
  FastLED.show();

  // go to next led and start the loop
  current++;
  while(current < steps)
  {
    //Serial.print("Current: ");
    //Serial.println(current);
    while(values[current%NUM_LEDS] < 255) // wait till current value is at 255
    {
      // make previous halfwidth darker
      for(int i = halfwidth; i >= 1; i--)
      {
        values[(current-i)%NUM_LEDS]--;
      }

      // make current and next halfwidth brighter
      for(int i = 0; i <= halfwidth-1; i++)
      {
        values[(current+i)%NUM_LEDS]++;
      }

      // Apply colors and show
      for(int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(hue, saturation, values[i]);
      FastLED.show();
      //Serial.println();
      delay(stepdelay);
    }

    current++;
  }
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

void cmd_anim_fade_blink()
{
  char *h = SCmd.next();
  char *s = SCmd.next();
  char *stepdelay = SCmd.next();

  if(h != NULL && s != NULL && stepdelay != NULL)
  {
    anim_fade_blink(CHSV(atoi(h), atoi(s), 1), atoi(stepdelay));
    Serial.println("200 Fade Blink.");
  }
  else
  {
    Serial.println("400 Bad Request");
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

