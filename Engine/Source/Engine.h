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

class Engine : public AudioIODeviceCallback {
    bool m_freeRun = true;
    bool m_activated = false;
    
    CloudCreator m_cloudCreator;
    AudioFormatManager m_formatManager;
public:
    Engine();
    
    void activate();
    void deactivate();
    
    void loadFiles(Array<File> files);
    void setMainFile(int index);
    
    void     audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples)override;
    
    void     audioDeviceAboutToStart (AudioIODevice *device)override;
    
    void     audioDeviceStopped ()override;
    
    
};
