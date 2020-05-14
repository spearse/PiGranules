/*
  ==============================================================================

    Engine.cpp
    Created: 12 May 2020 9:42:33pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#include "Engine.h"

Engine::Engine()
{
    m_formatManager.registerBasicFormats();
}
void Engine::loadFiles(Array<File> files){
    
    for(auto file : files){
        if(file.existsAsFile()){
            std::shared_ptr<AudioFormatReader> reader(m_formatManager.createReaderFor(file));
            m_cloudCreator.create_newTable();
            int64 numSamples = reader->lengthInSamples;
            AudioSampleBuffer samples(reader->numChannels,numSamples);
            reader->read(&samples, 0, numSamples, 0, true, false);
            
            GrainTable* t =  m_cloudCreator.get_lastTable();
            t->set_size(numSamples);
            for(int n = 0; n  < numSamples ; ++ n){
                t->set_sample(n, samples.getSample(0, n));
            }
        }
    }
    
}



void    Engine::audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples){
  
    for(int n = 0 ; n  < numSamples ;++n){
        
        for(int c = 0; c < numOutputChannels;++c){
            outputChannelData[c][n] = 0;
        }
        
    }
    if(m_freeRun && m_activated){
        m_cloudCreator.process(outputChannelData[0], outputChannelData[1], numSamples);
    }
    
    
};
   
   void     Engine::audioDeviceAboutToStart (AudioIODevice *device){
       m_cloudCreator.prepareToPlay(device->getCurrentSampleRate());

       
   };
   
   void     Engine::audioDeviceStopped (){
       
   };
   
void Engine::activate(){
    m_activated = true;
    m_cloudCreator.start();
}
void Engine::deactivate(){
    m_activated = false;
    m_cloudCreator.stop();
}
void Engine::setMainFile(int index){
    
    m_cloudCreator.set_tableIndex(index);
    
}
