/*
 ==============================================================================
 
 CloudCreator.cpp
 Created: 13 Dec 2017 12:13:39pm
 Author:  stephen
 
 ==============================================================================
 */

#include "CloudCreator.h"
#include "../JuceLibraryCode/JuceHeader.h"


CloudCreator::CloudCreator(float samplerate, float spawnRateMS, int maxGrains, int numGrains,int grainSize):
m_sampleRate(samplerate),
m_spawnRateMS(spawnRateMS),
m_maxNumGrains(maxGrains),
m_numGrains(numGrains),
m_samplesToNextSpawn((samplerate * 0.001)*spawnRateMS),
m_grainSize(grainSize),
m_childGrains(maxGrains),
m_playBackSpeed(1),
m_masterPlaybackSpeed(1),
m_phasor(44100),
m_grainPitch(1),
m_startPos(0),
m_randPitchAmount(0),
m_randPitchScale(20),
m_randStartPosAmount(0),
m_spawnTableIndex(1)
{
    
    trigger = [this](){};
    updatePhasor();
    
    set_startPos(0);
    set_playBackSpeed(0);
    set_grainSize(44100);
    set_numGrains(1);
    set_spawnRate(4);
    
}
float CloudCreator::scaledRandom(float lowerBound, float upperBound, float randomScale, float randomAmount) {
    
    //	float t = ((((rand() % 10000)* 0.0001)* randomAmount )*randomScale)+ (upperBound - lowerBound) * 0.5;
    float t =( (((rand() % 10000)* 0.0001) * randomAmount)  * ( upperBound - lowerBound) )+ lowerBound;
    if (t > upperBound)t = upperBound;
    if (t < lowerBound)t = lowerBound;
    return 1;
}

void CloudCreator::prepareToPlay(float sampleRate) 
{
    m_sampleRate = sampleRate;
    m_samplesToNextSpawn = (sampleRate*0.001) * m_spawnRateMS;
    
}


void CloudCreator::process(float* left, float* right, int blocksize) {
    if (m_play == true) {
        bool needsToSpawn = false;
        for (int n = 0; n < blocksize; ++n) {
            
            if(m_freeRun){
                
                m_samplesToNextSpawn -= 1;
                if (m_samplesToNextSpawn <= 0) needsToSpawn = true;
                if (needsToSpawn == true) {
                    int grainsAlive = 0;
                    for (int g = 0; g < m_numGrains; ++g) {
                        //	DBG(g);
                        if (  needsToSpawn == true) {
                            spawn();
                            //	DBG("Spawncount " += spawnCount);
                            needsToSpawn = false;
                        }
                        else {
                            ++grainsAlive;
                        }
                        
                    }
                    //	DBG("Grains:" += grainsAlive);
                    m_samplesToNextSpawn = (m_sampleRate*0.001) * m_spawnRateMS;
                }
                
            }
                
            
            
            
            for (int grain = 0; grain < m_numGrains; ++grain) {
                
                m_childGrains[grain].process(left, right, n, &m_multiGrain, m_multiGrain.get_table(0));
                //DBG("Grains " += grain );
            }
            m_phasor.tick();
            
        }
    }
    
    
}

void CloudCreator::spawn(){
    
    trigger();
    
    for(int g = 0 ; g < m_maxNumGrains;++g){
        if(m_childGrains[g].available()){
    
            int audioSize =m_multiGrain.get_table(1)->get_size();
            float rStart = (scaledRandom(0, 1, m_randStartPosAmount, m_randStartPosAmount)*audioSize);
            m_childGrains[g].spawn(m_grainSize, (m_phasor.get_phase() * audioSize) + (m_startPos * audioSize) , m_grainPitch ,m_sampleRate, m_spawnTableIndex   );
            return;
        }
    }
}


void CloudCreator::stop() {
    
    m_play = false;
    
    
}
void CloudCreator::start() {
    m_play = true;
}


void CloudCreator::updatePhasor() {
    
    m_phasor.set_frequency(m_masterPlaybackSpeed);
    
    
}


void CloudCreator::set_spawnRate(float spawnRate) {
    if (spawnRate <= 0)spawnRate = 0;
    
    m_spawnRateMS = spawnRate;
    m_samplesToNextSpawn = (m_sampleRate*0.001) * m_spawnRateMS;
}
void CloudCreator::set_grainSize(int grainSize) {
    if (grainSize <= 1)grainSize = 1;
    m_grainSize = grainSize;
}
void CloudCreator::set_numGrains(int numGrains) {
    if (numGrains >= m_maxNumGrains)numGrains = m_maxNumGrains;
    if (numGrains < 0)numGrains = 0;
    m_numGrains = numGrains;
    //	DBG("Num Grains " += m_numGrains);
}
void CloudCreator::set_playBackSpeed(float speed) {
    m_masterPlaybackSpeed = speed;
    m_phasor.set_frequency(m_masterPlaybackSpeed);
    
}

void CloudCreator::set_grainPitch(float pitch) {
    m_grainPitch = pitch;
}

void CloudCreator::set_startPos(float pos) {
    if (pos < 0) pos = 0;
    if (pos > 1.0f) pos = 1;
    
    m_startPos = pos;
}

void CloudCreator::set_randGrainPitch(float amount) {
    
    if (amount < 0) amount = 0;
    if (amount > 1.0f) amount = 1;
    m_randPitchAmount = amount;
    
}
void CloudCreator::set_randStartPos(float amount) {
    
    if (amount < 0) amount = 0;
    if (amount > 1.0f) amount = 1;
    m_randStartPosAmount = amount;
    
}
void CloudCreator::set_tableIndex(float tableIndex){
    if(tableIndex < 0)tableIndex = 0;
    if(tableIndex > m_multiGrain.get_numberTables()) tableIndex =m_multiGrain.get_numberTables();
    m_spawnTableIndex = tableIndex;
  //  std::cout << m_spawnTableIndex <<std::endl;
    
    
}

GrainTable* CloudCreator::get_lastTable(){
    return m_multiGrain.get_lastTable();
}


void CloudCreator::create_newTable(){
    m_multiGrain.create_table();
}

int CloudCreator::get_numTables(){
    return m_multiGrain.get_numberTables();
}


void CloudCreator::deactivate_allGrains(){
    
    for(auto g : m_childGrains){
        g.deactivate();
    }
    
}

void CloudCreator::set_freeRunState(bool state){
    m_freeRun = state;
}
