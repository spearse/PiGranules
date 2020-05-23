/*
  ==============================================================================

    HostInterface.cpp
    Created: 19 May 2020 3:52:14pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#include "HostInterface.h"
#include "Engine.h"
#include "Application.h"

HostInterface::HostInterface(PiGranulesApp* parent):
    m_parent(parent),
    m_test("Hello","Hello")
{
    addAndMakeVisible(m_test);
    addAndMakeVisible(m_spawnRateController);
    setSize(300, 1000);
}


void HostInterface::resized(){
    auto size = getLocalBounds();
    m_test.setBounds(size.removeFromTop(100));
    m_spawnRateController.setBounds(size.removeFromTop(100));
    
}

