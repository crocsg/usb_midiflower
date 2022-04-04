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
#include "flower_sensor.h"
#include "MIDI/midinote.h"
#include "MIDI/midisequencer.h"
#include "MIDI/sequence.h"
#include "TOOLS/utils.h"
#include "HARD/led.h"


void midiSerial(int type, int channel, int data1, int data2);
void setNote(int value, int velocity, long duration, int notechannel, int ramp);


uint32_t previousMillis = 0;
uint32_t currentMillis = 1;



#define BPM90 ((60*1000)/90)
#define BPM120 ((60*1000)/120)
#define BPM160 ((60*1000)/160)
#define BPM BPM120
CMidiSequencer sequencer = CMidiSequencer(64);
CSequence A = CSequence(60, BPM, 75);
CSequence B = CSequence(120, BPM, 60);
CSequence C = CSequence(120, BPM/4, 75);
CSequence D = CSequence(120, BPM, 35);


std::vector<CSequence*> psequences;
uint32_t sequence_time = 0;
uint16_t sequence_index = 0;
uint32_t last_sample_check = 0;

#ifdef __cplusplus
extern "C" {
#endif

void setup(void)
{

//MIDIpanic(); //dont panic, unless you are sure it is nessisary
  A.clear ();
  B.clear ();
  C.clear ();
  D.clear ();
  psequences.push_back(&A);
  psequences.push_back(&B);
  psequences.push_back(&C);
  psequences.push_back(&D);
  last_sample_check =  millis ();

  //attachInterrupt(interruptPin, sample, RISING);  //begin sampling data from interrupt
  flower_sensor_init ();
  flower_sensor_set_callback (&setNote);
}

void loop(void)
{
  currentMillis = millis();   //manage time


  if (currentMillis - sequence_time >= (BPM / 8))
  {

    MIDImessage note;
    sequence_time = currentMillis;
    if ((sequence_index > 0 && psequences[sequence_index]->getnbnotes() > psequences[sequence_index]->size() / 5) ||
      sequence_index == 0)
    {
      if (psequences[sequence_index]->play(currentMillis, &note) == 1)
      {

          //Serial.printf ("play seq=%d milli=%lu\n", sequence_index, currentMillis);
          sequencer.Play (currentMillis, &note);
      }
    }
    sequence_index = (sequence_index + 1) % psequences.size ();
  }
  else
  {
    sequencer.Control (currentMillis);
  }

  ///
  if (flower_sensor_data_available ())
    flower_sensor_analyzeSample ();

  //checkNote();  //turn off expired notes
  //checkControl();

  flower_sensor_update_threshold ();
  uint32_t last_samples = flower_sensor_get_last_sample_time_ms ();
  if (currentMillis - last_sample_check >= 1000)
  {
    if (currentMillis - last_samples > 15000)
    {
        psequences[2]->mute (80);
        psequences[3]->clear ();
    }
    if (currentMillis - last_samples > 45000)
    {
        psequences[0]->mute (75);
        psequences[1]->clear ();
        psequences[2]->clear ();
        psequences[3]->clear ();
    }
    if (currentMillis - last_samples > 75000)
    {
        psequences[0]->clear ();
    }

    last_sample_check = currentMillis;
  }

  if (psequences[0]->getnbnotes () > 15)
    flower_sensor_set_analyse_short (0);

  previousMillis = currentMillis;   //manage time

}

#ifdef __cplusplus
}
#endif


void setNote(int value, int velocity, long duration, int notechannel, int ramp)
{

      led_signal_led_toggle();
      if (psequences[0]->getnbnotes() < psequences[0]->size () / 3)
        psequences[0]->addNote (currentMillis, value, velocity, duration, 1);
      else if (psequences[1]->getnbnotes() < psequences[1]->size () / 3)
        psequences[1]->addNote (currentMillis, value, velocity, duration, 2);
      /*else if (psequences[2]->getnbnotes() < psequences[2]->size () / 2)
        psequences[2]->addNote (currentMillis, value, velocity, duration, 3);*/
      else
      {
        uint16_t seq = currentMillis % psequences.size();
        psequences[seq]->addNote (currentMillis, value, velocity, duration, seq + 1);
      }
}

void MIDIpanic()
{
  //120 - all sound off
  //123 - All Notes off
 // midiSerial(21, panicChannel, 123, 0); //123 kill all notes

  //brute force all notes Off
  for(uint8_t i=1;i<128;i++)
  {

    midiSerial(144, 0, i, 0); //clear notes on main channel
  }


}

