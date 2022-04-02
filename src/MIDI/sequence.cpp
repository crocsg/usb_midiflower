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

#include "sequence.h"

#define DEFAULT_TTL 12

CSequence::CSequence (size_t size, uint32_t tempo, uint32_t noteratio)
{
    m_tempo = tempo;
    m_seq.resize (size);
    m_noteratio = noteratio;
    m_lastplay = 0;
    m_cntnote = 0;
    m_delnote = 0;
}

void CSequence::addNote (uint32_t time, uint8_t value, uint8_t velocity, uint16_t duration, uint8_t notechannel)
{

    if (velocity == 0)
        return;
    MIDImessage mes;
    mes.type = 0;
    mes.value = value;
    mes.velocity = velocity;
    mes.duration = duration;
    mes.channel = notechannel;
    mes.ttl = DEFAULT_TTL;

    size_t pos = (time / m_tempo) % m_seq.size ();
    if (m_seq[pos].velocity == 0)
        m_cntnote++;
    m_seq[pos] = mes;
    //Serial.printf("addNote pos=%d, size=%d, ch=%d, nbnote=%d\n", pos, m_seq.size(), notechannel, m_cntnote);

    // delete a note if sequence is full
    if (m_cntnote > (m_seq.size() * m_noteratio) / 100)
    {
        uint8_t delnote = 0;
        //Serial.printf("Sequence full  ch=%d, nbnote=%d\n", notechannel, m_cntnote);
        #if 1
        for (auto it = m_seq.begin () + pos + 1; it != m_seq.end (); it++)
        {
            if (it->velocity > 0)
            {
                it->velocity = 0;
                m_cntnote--;
                delnote++;
                //Serial.printf("delete note ch=%d, nbnote=%d\n", notechannel, m_cntnote);
                break;
            }
        }
        if (delnote == 0)
        {
            for (auto it = m_seq.begin () ; it != m_seq.end () + pos; it++)
            {
                if (it->velocity > 0)
                {
                    it->velocity = 0;
                    m_cntnote--;
                    delnote++;
                    //Serial.printf("delete note ch=%d, nbnote=%d\n", notechannel, m_cntnote);
                    break;
                }
            }
        }
        #else
        for (int p = pos + 1; p < m_seq.size(); p++)
        {
            if (m_seq[p].velocity == 1)
            {
                m_seq[p].velocity = 0;
                m_cntnote--;
                //Serial.printf("delete note ch=%d, nbnote=%d\n", notechannel, m_cntnote);
                break;
            }
        }
        #endif
    }
}

uint8_t CSequence::play (uint32_t time, MIDImessage* mes)
{
    if (time - m_lastplay < m_tempo)
        return (0);

    m_lastplay = time;
    return play_learn (time, mes);

}

uint8_t CSequence::play_learn (uint32_t time, MIDImessage* mes)
{
    size_t pos = (time / m_tempo) % m_seq.size ();

    if (m_seq[pos].velocity != 0)
    {
        //Serial.printf ("node pos=%d, ch=%d, val=%d, vel=%d\n", pos, m_seq[pos].channel, m_seq[pos].value, m_seq[pos].velocity);
        *mes = m_seq[pos];

        m_seq[pos].ttl--;
        if (m_seq[pos].ttl == 0)
        {
          m_seq[pos].velocity = 0;
          m_cntnote--;
        }
        return (1);
    }
    return (0);
}
