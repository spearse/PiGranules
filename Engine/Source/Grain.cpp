/*
  ==============================================================================

    Grain.cpp
    Created: 2 Jan 2018 11:16:35am
    Author:  spearse

  ==============================================================================
*/

#include "Grain.h"



Grain::Grain():
m_available(true),
m_running(false),
m_sampleDuration(4410),
m_sampleIndex(0),
m_grainSpeed(1),
m_tableIndex(1),
m_phasor(44100.0f)
{
    
}


void Grain::process(float* left, float* right, int index, MultiGrainTable* audio, GrainTable* window) {

    
    if (m_running == true) {
        float windowSize = window->get_size();
        
        float s = audio->get_sample(m_tableIndex,m_startPosition + m_currentPosition) * window->get_sample((m_sampleIndex / m_sampleDuration)  * windowSize);
        left[index] += s;
        right[index] += s;
        
        m_currentPosition += m_grainSpeed;
        ++m_sampleIndex;
        if (m_sampleIndex >= m_sampleDuration) {
            m_running = false;
            m_available = true;
        }
        m_phasor.tick();
    }
    
}


void Grain::spawn(int duration, float startPos,float grainSpeed,float sampleRate,float tableIndex) {
    m_sampleDuration = duration;
    m_startPosition = startPos;
    m_grainSpeed = grainSpeed;
    m_sampleIndex = 0;
    m_currentPosition = 0;
    m_phasor.set_phase(0);
    m_phasor.set_frequency(grainSpeed);
    m_tableIndex = tableIndex;
    
    std::cout << m_tableIndex <<std::endl;
    
    m_available = false;
    m_running = true;
    
}

bool Grain::available() {
    return m_available;
}

void Grain::deactivate(){
    m_running = false;
    m_available = true;
}

