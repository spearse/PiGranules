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
class PiGranulesApp;

class HostInterface : public Component{
    PiGranulesApp* m_parent;
    TextButton m_test;
    ToggleButton m_clientsFreeRun;
    
    friend class PiGranulesApp;
public:
    HostInterface(PiGranulesApp* parent);
    void resized()override;
    
    SliderController m_spawnRateController;
    
};
