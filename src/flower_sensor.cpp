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
#include "HARD/pwm.h"

#define DESIRED_EVENT 24
#define SAMPLESIZE    32
#define MIN_DELTA     200

static flower_sensor_callback_mes _setmes;



static uint8_t      _samplesize = SAMPLESIZE / 2; //set sample array size
static uint8_t      state;
volatile uint32_t   _microseconds = 0; //sampling timer
volatile uint8_t    _sindex = 0;
volatile uint32_t   _samples[SAMPLESIZE];
volatile uint32_t   _last_samples = 0;

float threshold   = 1;  //change threshold multiplier
float threshMin   =  0.001; //scaling threshold min
float threshMax   = 15; //scaling threshold max

static uint32_t  threshold_last_millis = 0;
static uint32_t  threshold_evt = 0;

static uint16_t _activity_id = 0;
static uint16_t _activity_val[] = 
{
  0,
  8562,
  16977,
  25101,
  32795,
  39927,
  46374,
  52027,
  56787,
  60574,
  63323,
  64986,
  65534,
  64960,
  63272,
  60499,
  56689,
  51907,
  46235,
  39771,
  32625,
  24920,
  16787,
  8367,
  0,
  0,
  0,

};

extern "C" {

  void flower_sensor_interrupt (uint16_t gpio)
  {
    if(_sindex < _samplesize) {
        uint32_t now = micros();
        _samples[_sindex] = now - _microseconds;
        _microseconds = now;
        _sindex += 1;
    }
    _last_samples = millis ();
    state++;
    //led_signal_led_toggle ();
    pwm_activity_level (_activity_val[(_activity_id++ % ARRAYLEN(_activity_val))]);
  }
}

void flower_sensor_init (int interruptPin)
{

  led_signal_led_on();
  _last_samples = millis ();
  gpio_hal_enable_interrupt (interruptPin, flower_sensor_interrupt);

}

uint8_t flower_sensor_data_available (void)
{
  return (_sindex >= _samplesize ? 1 : 0);
}

void flower_sensor_set_callback (flower_sensor_callback_mes clbk)
{
  _setmes = clbk;
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
        if (threshold > threshMin)
          threshold /= 1.4;
      }
      else
      {
        if (threshold < threshMax)
          threshold *= 1.4;
      }
      threshold_last_millis = currentMillis;
      threshold_evt = 0;

    }
}

void flower_sensor_build_mes (void)
{
  if (flower_sensor_data_available ())
      flower_sensor_analyzeSample (); // build data

  flower_sensor_update_threshold (); // update measure threshold
}



uint32_t flower_sensor_get_last_sample_time_ms (void)
{
  return _last_samples;
}

void flower_sensor_analyzeSample(void)
{
  uint32_t averg = 0;
  uint32_t maxim = 0;
  uint32_t minim = 10000000;
  float stdevi = 0;
  uint32_t delta = 0;
  uint8_t change = 0;

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

    // compute std deviation
    stdevi = stdevi / (_samplesize);
    if (stdevi < 1) 
    { 
      stdevi = 1.0; //min stdevi of 1
    } 

    stdevi = sqrt(stdevi); //calculate stdevi

    delta = maxim - minim;

    

    //**********perform change detection
    if (delta > (stdevi * threshold) && delta > MIN_DELTA)
    {
      change = 1;
      threshold_evt ++;
    }

    if (change && _setmes)
    {
      //Serial.printf("%ld %ld %ld %ld %f %f\r\n", minim, maxim, averg, delta, stdevi, stdevi * threshold); 
      _setmes (minim, maxim, averg, delta, stdevi, stdevi * threshold);
    }
    //reset array for next sample
     _sindex = 0;

  }
}