/*
  ==============================================================================

    Grain.h
    Created: 2 Jan 2018 11:16:35am
    Author:  spearse

  ==============================================================================
*/

#pragma once
#include "GrainTable.h"
#include <vector>
#include "MultiGrainTable.h"


class Grain {
    bool m_available,m_running;
    float m_sampleDuration, m_sampleIndex;
    float m_currentPosition,m_startPosition,m_grainSpeed,m_tableIndex;
    Phasor m_phasor;
public:
    Grain();
    void process(float* left, float* right, int index, MultiGrainTable* audio, GrainTable* window);
    
   
   
    void spawn(int duration, float startPos,float grainSpeed=1,float sampleRate = 44100.0f,float tableIndex=1 );
    
    bool available();

    void deactivate();
    
};



typedef std::vector<Grain> GrainVector;



