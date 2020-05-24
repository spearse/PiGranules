/*
  ==============================================================================

    HostInterface.h
    Created: 19 May 2020 3:52:14pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SliderController.h"
#include "ClientViewer.h"

class PiGranulesApp;

class HostInterface : public Component{
    PiGranulesApp* m_parent;
    TextButton m_test;
    ToggleButton m_clientsFreeRun,m_clientPlayBack,m_clientTrigger;
    
    GroupComponent m_granularControlGroup,m_clientModeGroup,m_clientsGroup;
    Viewport m_clientViewport;
    ClientViewer m_clientViewer;
    
    friend class PiGranulesApp;
public:
    HostInterface(PiGranulesApp* parent);
    void resized()override;
    
    SliderController m_spawnRateController,m_grainSizeController,m_grainPitchController,m_playspeedController,m_selectedIndexController,m_grainAmpController,m_grainPanController;
    
};
