/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 S Godin (Climate Change Lab)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#include <cmath>
#include <cstdint>

#include "usb_midiflower.h"
#include "board.h"
#include "flower_sensor.h"
#include "flower_music.h"
#include "MIDI/midinote.h"

#include "MIDI/sequence.h"
#include "MidiFlowerSequencer.h"
#include "TOOLS/utils.h"
#include "HARD/led.h"
#include "HARD/pwm.h"


#define NBNOTE_FOR_BETTER_MEASURE   15


// get the sequencer from flower_music
extern CMidiFlowerSequencer sequencer;  


void flowersensor_measure (uint32_t min, uint32_t max, uint32_t averg, uint32_t delta, float stdevi, float stdevical);



#ifdef __cplusplus
extern "C" {
#endif

void setup(void)
{
  // start music sequencer
  flower_music_init ();

  //attachInterrupt(interruptPin, sample, RISING);  //begin sampling data from interrupt
  // start flower sensor
  flower_sensor_init(GPIO_FLOWER_SENSOR);
  flower_sensor_set_analyse_short(1);

  flower_sensor_set_callback (flowersensor_measure);

  // enable sequencer
  sequencer.setLock(false);

  pwm_activity_level (0);
}

void loop(void)
{
  // good music is good rythm. You must call this very often
  // a least one time by 10 millisec
  sequencer.Loop ();
  
  
  // build flower measure
  // must be called often
  flower_sensor_build_mes();

  // do some control stuff
  ControlMusic ();

  // if we have some music notes, change configuration for better measure
  if (sequencer.get_track_nbnote(0) > NBNOTE_FOR_BETTER_MEASURE)
  {
    flower_sensor_set_analyse_short(0);
  }

}

#ifdef __cplusplus
}
#endif

static uint16_t pwmvalue = 0;
// Flower sensor measure callback. receive flower measures 
// min    min value
// max    max value
// averg  average
// delta  max - min
// stdevi standard deviation
// stdevical standard deviation * threshold
void flowersensor_measure (uint32_t min, uint32_t max, uint32_t averg, uint32_t delta, float stdevi, float stdevical)
{
    //Serial.println ("measures received \n");
    uint16_t val = (delta % 256) * 256;

    
    led_signal_led_toggle ();

    // give all the measure to flower music generation code
    BuildNoteFromMeasure (millis(), min, max, averg, delta, stdevi, stdevical);
}

