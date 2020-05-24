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
    m_test("Hello","Hello"),
    m_clientsFreeRun("Clients Free Run"),
    m_clientPlayBack("Clients Soudfile Playback"),
    m_clientTrigger("Clients Triggered From Host")
{
    m_clientsFreeRun.setToggleState(true, dontSendNotification);
    addAndMakeVisible(m_clientsFreeRun);
    addAndMakeVisible(m_clientPlayBack);
    addAndMakeVisible(m_clientTrigger);
    addAndMakeVisible(m_clientViewport);
    addAndMakeVisible(m_clientViewport);
    addAndMakeVisible(m_clientsGroup);
    addAndMakeVisible(m_clientViewer);
    
    m_clientViewport.setViewedComponent(&m_clientViewer);
    
    m_clientsFreeRun.setRadioGroupId(1066);
    m_clientTrigger.setRadioGroupId(1066);
    m_clientPlayBack.setRadioGroupId(1066);
    
    
    addAndMakeVisible(m_granularControlGroup);
    addAndMakeVisible(m_clientModeGroup);
    
    addAndMakeVisible(m_test);
    addAndMakeVisible(m_spawnRateController);
    addAndMakeVisible(m_grainSizeController);
    addAndMakeVisible(m_grainPitchController);
    addAndMakeVisible(m_playspeedController);
    addAndMakeVisible(m_selectedIndexController);
    addAndMakeVisible(m_grainAmpController);
    addAndMakeVisible(m_grainPanController);
    
    
    
    setSize(1000, 800);
}


void HostInterface::resized(){
    auto size = getLocalBounds().reduced(10);
    auto clientViewBar = size.removeFromLeft(100);
    auto controlBar= size.removeFromTop(200);
    //m_test.setBounds(size.removeFromTop(100));
    auto clientControlss = size.removeFromBottom(100);
    
    m_granularControlGroup.setBounds(controlBar);
    controlBar.removeFromTop(10);
    
    m_spawnRateController.setBounds(controlBar.removeFromLeft(100).reduced(10));
    m_grainSizeController.setBounds(controlBar.removeFromLeft(100).reduced(10));
    m_grainPitchController.setBounds(controlBar.removeFromLeft(100).reduced(10));
    m_playspeedController.setBounds(controlBar.removeFromLeft(100).reduced(10));
    m_grainAmpController.setBounds(controlBar.removeFromLeft(100).reduced(10));
    m_grainPanController.setBounds(controlBar.removeFromLeft(100).reduced(10));
    
    m_clientModeGroup.setBounds(clientControlss);
    m_clientsFreeRun.setBounds(clientControlss.removeFromLeft(100).reduced(10));
    m_clientTrigger.setBounds(clientControlss.removeFromLeft(100).reduced(10));
    m_clientPlayBack.setBounds(clientControlss.removeFromLeft(100).reduced(10));
  //  m_selectedIndexController.setBounds(controlBar.removeFromTop(100).reduced(10));
    
    
    m_clientsGroup.setBounds(clientViewBar);
    m_clientViewport.setBounds(clientViewBar.reduced(10));
    
    
    
}

