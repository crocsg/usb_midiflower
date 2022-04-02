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

#include "board.h"
#include "TOOLS/utils.h"
#include "gpio_hal.h"
#include "flower_sensor.h"
#include "HARD/led.h"

#define DESIRED_EVENT 12

flower_sensor_callback _setnote;



static uint8_t _samplesize = SAMPLESIZE / 2; //set sample array size
static uint8_t state;
volatile uint32_t _microseconds = 0; //sampling timer
volatile uint8_t _sindex = 0;
volatile uint32_t _samples[SAMPLESIZE];
uint8_t channel = 1;  //setting channel to 11 or 12 often helps simply computer midi routing setups
uint8_t noteMin = 36; //C2  - keyboard note minimum
uint8_t noteMax = 96; //C7  - keyboard note maximum

float threshold = 1;  //change threshold multiplier
float threshMin = 1.61; //scaling threshold min
float threshMax = 3.71; //scaling threshold max

uint32_t threshold_last_millis = 0;
uint32_t  threshold_evt = 0;

//******************************
//set scaled values, sorted array, first element scale length
int scaleMajor[]  = {7,1, 3, 5, 6, 8, 10, 12};
int scaleDiaMinor[]  = {7,1, 3, 4, 6, 8, 9, 11};
int scaleIndian[]  = {7,1, 2, 2, 5, 6, 9, 11};
int scaleMinor[]  = {7,1, 3, 4, 6, 8, 9, 11};
int scaleChrom[] = {12,1,2,3,4,5,6,7,8,9,10,11,12};
int *scaleSelect = scaleMajor; //initialize scaling
int root = 0; //initialize for root

//*******************************

extern "C" {
  void flower_sensor_interrupt (uint16_t gpio)
  {
    if(_sindex < _samplesize) {
        uint32_t now = micros();
        _samples[_sindex] = now - _microseconds;
        _microseconds = now;
        _sindex += 1;
    }

    state++;
    //led_signal_led_toggle ();
  }
}
void flower_sensor_init ()
{

  led_signal_led_on();

  gpio_hal_enable_interrupt (GPIO_FLOWER_SENSOR, flower_sensor_interrupt);

}

uint8_t flower_sensor_data_available (void)
{
  return (_sindex >= _samplesize ? 1 : 0);
}

void flower_sensor_set_callback (flower_sensor_callback clbk)
{
  _setnote = clbk;
}

void flower_sensor_set_analyse_short (uint8_t s)
{
  if (s != 0)
      _samplesize = SAMPLESIZE / 2;
  else
    _samplesize = SAMPLESIZE;
}

void flower_sensor_update_threshold (void)
{
  uint32_t currentMillis = millis ();
  if (currentMillis - threshold_last_millis > 5000)
  {

      if (threshold_evt < DESIRED_EVENT)
      {
        if (threshold > 0.001)
          threshold /= 1.4;
      }
      else
      {
        if (threshold < 15)
          threshold *= 1.4;
      }
      threshold_last_millis = currentMillis;
      threshold_evt = 0;

    }
}

int scaleSearch(int note, int scale[], int scalesize) {
 for(uint8_t i=1;i<scalesize;i++) {
  if(note == scale[i]) { return note; }
  else { if(note < scale[i]) { return scale[i]; } } //highest scale value less than or equal to note
  //otherwise continue search
 }
 //didn't find note and didn't pass note value, uh oh!
 return 6;//give arbitrary value rather than fail
}


int scaleNote(int note, int scale[], int root) {
  //input note mod 12 for scaling, note/12 octave
  //search array for nearest note, return scaled*octave
  int scaled = note%12;
  int octave = note/12;
  int scalesize = (scale[0]);
  //search entire array and return closest scaled note
  scaled = scaleSearch(scaled, scale, scalesize);
  scaled = (scaled + (12 * octave)) + root; //apply octave and root
  return scaled;
}

void flower_sensor_analyzeSample(void)
{
  uint32_t averg = 0;
  uint32_t maxim = 0;
  uint32_t minim = 10000000;
  float stdevi = 0;
  uint32_t delta = 0;
  uint8_t change = 0;



   //digitalWrite(LED, ((state) & 0x01) == 0 ? HIGH : LOW);
  state++;
  if (_sindex >= _samplesize) { //array is full
    unsigned long sampanalysis[SAMPLESIZE];
    for (uint8_t i=0; i < _samplesize; i++){
      //skip first element in the array
      sampanalysis[i] = _samples[i];  //load analysis table (due to volitle)
      //manual calculation
      if(sampanalysis[i] > maxim) { maxim = sampanalysis[i]; }
      if(sampanalysis[i] < minim) { minim = sampanalysis[i]; }
      averg += sampanalysis[i];

    }
    averg = averg / (_samplesize);
    for (uint8_t i = 0; i < _samplesize; i++)
    {
      stdevi += (sampanalysis[i] - averg) * (sampanalysis[i] - averg) ;  //prep stdevi
    }

    //manual calculation

    stdevi = stdevi / (_samplesize);
    if (stdevi < 1) { stdevi = 1.0; } //min stdevi of 1

    stdevi = sqrt(stdevi); //calculate stdevu

    delta = maxim - minim;


    //Serial.printf("%ld %ld %ld %ld %f %f\r\n", minim, maxim, averg, delta, stdevi, stdevi * threshold);

    //**********perform change detection
    if (delta > (stdevi * threshold)){
      change = 1;
      threshold_evt ++;

    }
    //*********

    if(change){// set note and control vector
        int dur = 100+(map(delta%127,1,127,100,1000)); //length of note
        int ramp = 3 + (dur%100) ; //control slide rate, min 25 (or 3 ;)


        //set scaling, root key, note
        int note = map(averg%127,1,127,noteMin,noteMax);  //derive note, min and max note
        note = scaleNote(note, scaleSelect, root);  //scale the note
        // setnote = setnote + root; // (apply root?)

        if (_setnote)
        {
          _setnote(note, 100, dur, channel,ramp);
          //led_hal_signal_led_toggle();
        }


        //derive control parameters and set
        //setControl(controlNumber, controlMessage.value, delta%127, ramp); //set the ramp rate for the control
     }
     //reset array for next sample
     _sindex = 0;

  }
}
