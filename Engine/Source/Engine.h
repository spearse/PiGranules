/*
 ==============================================================================
 
 Engine.h
 Created: 12 May 2020 9:42:33pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CloudCreator.h"

class PiGranulesApp;

class Engine : public AudioIODeviceCallback {
    bool m_freeRun = true;
    bool m_activated = false;
    bool m_triggerExternal = true;
    bool m_sequenceExternal = true;
    
    CloudCreator m_cloudCreator;
    AudioFormatManager m_formatManager;
    
    PiGranulesApp* m_parent;
    
    AudioSampleBuffer m_outBufferA,m_outBufferB;
    AudioSampleBuffer m_inputBufferA,m_inputBufferB;
    bool m_useOutBufferA = true;
    bool m_useInputBufferA = true;
    
    bool m_useRemoteAudio = false;
    
    NormalisableRange<float> m_spawnRateMSRange, m_grainPitchRange,m_startPosRange, m_masterPlaybackSpeedRange,m_spawnTableIndexRange;
    
    //int m_maxNumGrains, m_numGrains ,m_samplesToNextSpawn,m_grainSize
    friend class PiGranulesApp;
    friend class HostInterface;
public:
    
    Engine(PiGranulesApp* parent);
    
    void activate();
    void deactivate();
    
    void loadFiles(Array<File> files);
    void setMainFile(int index);
    
    void     audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples)override;
    
    void     audioDeviceAboutToStart (AudioIODevice *device)override;
    
    void     audioDeviceStopped ()override;
    
    CloudCreator& getCloudCreator();
    
    std::function<void(const AudioSampleBuffer&)> externalAudioCallback;

    void sequenceExternal();
    
    AudioSampleBuffer& getActiveInputBuffer();
    
    void useRemoteAudio(bool state);
    
    
        
};
