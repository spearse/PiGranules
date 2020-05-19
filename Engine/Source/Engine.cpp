/*
 ==============================================================================
 
 Engine.cpp
 Created: 12 May 2020 9:42:33pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#include "Engine.h"
#include "Application.h"


Engine::Engine(PiGranulesApp* parent):
m_parent(parent)
{
    externalAudioCallback = [this](const AudioSampleBuffer& buffer){};

    m_formatManager.registerBasicFormats();
    
    m_cloudCreator.trigger = [this](){
        sequenceExternal();
    };
    
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
    if( m_activated==true && m_useRemoteAudio == false){
        m_cloudCreator.process(outputChannelData[0], outputChannelData[1], numSamples);
    }
    
    if(m_useRemoteAudio){
        //channel counts should match!
        if(m_useInputBufferA){
            for (int c = 0 ; c < m_inputBufferA.getNumChannels();++c){
                for(int n = 0 ;  n < m_inputBufferA.getNumSamples();++n){
                    outputChannelData[c][n] = m_inputBufferA.getSample(c, n);
                }
            }
            m_inputBufferA.clear();
        }else{
            for (int c = 0 ; c < m_inputBufferB.getNumChannels();++c){
                for(int n = 0 ;  n < m_inputBufferB.getNumSamples();++n){
                    outputChannelData[c][n] = m_inputBufferB.getSample(c, n);
                }
            }
            m_inputBufferB.clear();
        }
        m_useInputBufferA = ! m_useInputBufferA;
    }
    
    static AudioSampleBuffer t(outputChannelData,numOutputChannels,numSamples);
    
    if(m_useOutBufferA){
        
        m_outBufferA.makeCopyOf(t);
        externalAudioCallback(m_outBufferA);
    }else{
        m_outBufferB.makeCopyOf(t);
        externalAudioCallback(m_outBufferB);
    }
    m_useOutBufferA = !m_useOutBufferA;
    
    
    
};

void     Engine::audioDeviceAboutToStart (AudioIODevice *device){
    m_cloudCreator.prepareToPlay(device->getCurrentSampleRate());
    
    m_outBufferA.setSize(device->getOutputChannelNames().size(), device->getCurrentBufferSizeSamples());
    m_outBufferB.setSize(device->getOutputChannelNames().size(), device->getCurrentBufferSizeSamples());

    
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
CloudCreator& Engine::getCloudCreator(){
    return m_cloudCreator;
}

//Quick and nasty sequencer for testing
void Engine::sequenceExternal(){
    
    static int pos = 0;
    
    if(m_parent->m_childAddresses.size() > 0 ){
        
        m_parent->spawnOnClient(pos);
        
        ++pos %=  m_parent->m_childAddresses.size() ;
    }
    
}

AudioSampleBuffer& Engine::getActiveInputBuffer(){
    if(m_useInputBufferA){
        return m_inputBufferB;
    }else{
        return m_inputBufferA;
    }
}

void Engine::useRemoteAudio(bool state){
    m_useRemoteAudio = state;
}


