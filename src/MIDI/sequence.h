/*
 MIT License

Copyright (c) 2022 S Godin (Climate Change Lab)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


This project is based on https://github.com/electricityforprogress/MIDIsprout great
work about biodata sonification
*/


#ifndef __SEQUENCE_H
#define __SEQUENCE_H

#include "midinote.h"
#include <vector>


class CSequence
{
public:
    CSequence (size_t maxsize, uint32_t bpm, uint32_t bpmmulti, uint32_t noteratio, uint16_t midichannel);
    
    void addNote (uint32_t time, uint8_t value, uint8_t velocity, uint16_t duration, uint16_t ramp);
    uint8_t play (uint32_t time, MIDImessage* mes);
    void clear (void);
    void mute (int vel);
    
    size_t   getnbnotes () { return m_cntnote;}
    size_t   size() {return m_size;}
    void setSize (size_t size)
    {
        if (m_seq.size () >= size)
            m_size = size;
        UpdateNbNote ();    
    }

    void setRatio (uint32_t ratio)
    {
        m_noteratio = ratio > 100 ? 100 : ratio;
        UpdateNbNote ();            
    }

    uint32_t getRatio (void)
    {
        return m_noteratio;
    }

    void    setBpm (uint32_t bpm)
    {
            m_bpm = bpm;
            UpdateTempo ();
    }
    uint32_t getBpm ()
    {
        return m_bpm;
    }

    void    setBpmMultiplier (uint32_t multi)
    {
            m_bpm_multi = multi;
            UpdateTempo ();
    }

    uint32_t getBpmMultiplier ()
    {
        return m_bpm_multi;
    }

    void setRelativeVol (uint32_t vol)
    {
        m_vol = vol;
    }

    uint32_t getRelativeVol (void)
    {
        return m_vol;
    }
    bool isEnabled ()
    {
        return (m_tempo > 0);
    }
    uint16_t getMidiChannel (void)
    {
        return (m_midichannel);
    }
    void setMidiChannel(uint16_t channel)
    {
        m_midichannel = channel;
    }
private:

    void UpdateTempo ()
    {
        
        uint32_t realbpm = m_bpm * m_bpm_multi;
        if (realbpm > 0)
        {
            m_tempo = ((60 * 1000) / realbpm);
        }
        else
            m_tempo = 0;
    }
    
    void UpdateNbNote (void);
    
    uint8_t play_seq (uint32_t time, MIDImessage* mes);
    

    
    
    std::vector<MIDImessage>    m_seq;          // midi notes array
    std::vector<uint8_t>        m_melody;
    uint32_t                    m_size;         // size in notes
    uint32_t                    m_tempo;        
    uint32_t                    m_bpm_multi;    // bpm multiplier
    uint32_t                    m_bpm;          // base bpm
    uint32_t                    m_noteratio;    // sequence fill ratio
    uint32_t                    m_lastplay;     // last play time
    uint32_t                    m_cntnote;      // count note in sequence
    uint32_t                    m_vol;          // sequence relative volume
    uint16_t                    m_midichannel;  // real midi channel
};

#endif 
