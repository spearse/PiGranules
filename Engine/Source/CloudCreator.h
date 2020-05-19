/*
  ==============================================================================

    CloudCreator.h
    Created: 13 Dec 2017 12:13:39pm
    Author:  stephen

  ==============================================================================
*/

#pragma once
#include <vector>
#include "GrainTable.h"
#include <stdlib.h>  
#include "Grain.h"
#include "MultiGrainTable.h"

class CloudCreator 
{
	float m_sampleRate, m_spawnRateMS,m_masterPlaybackSpeed, m_grainPitch, m_startPos,m_spawnTableIndex;
	int m_maxNumGrains, m_numGrains ,m_samplesToNextSpawn,m_grainSize;
	GrainVector m_childGrains;
    
    MultiGrainTable m_multiGrain;
    
    
	bool m_play;
	float m_playBackSpeed;
	Phasor m_phasor;

	float m_randPitchAmount, m_randPitchScale;
	float m_randStartPosAmount;


	float scaledRandom(float lowerBound, float upperBound, float randomScale, float randomAmount);

    bool m_freeRun = true;
    
    
    
public:
	CloudCreator(float samplerate = 44100.0f, float spawnRateMS = 1000, int maxGrains = 100, int numGrains = 8,int grainSize=44100);

	void prepareToPlay(float sampleRate);
	void process(float* left, float* right, int blocksize);
	void stop();
	void start();
	void updatePhasor();
	GrainTable* get_lastTable();
    void create_newTable();
    int get_numTables();
    
    void deactivate_allGrains();
    
    
	void set_spawnRate(float spawnRate);
	void set_grainSize(int grainSize);
	void set_numGrains(int numGrains);
	void set_playBackSpeed(float speed);
	void set_grainPitch(float pitch);
	void set_startPos(float pos);

    

	void set_randGrainPitch(float amount);
	void set_randStartPos(float amount);
    
    void set_tableIndex(float tableIndex);
    
    void set_freeRunState(bool state);
    
    void spawn();
    std::function<void()> trigger;
    
};
