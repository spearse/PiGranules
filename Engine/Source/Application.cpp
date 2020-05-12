/*
 ==============================================================================
 
 Application.cpp
 Created: 12 May 2020 9:42:14pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#include "Application.h"


void PiGranulesApp::initialise (const String& commandLine)
{
    DBG("Pi Granules Starting");
    DBG(commandLine);
    String init = m_deviceManager.initialiseWithDefaultDevices(0, 1);
    DBG(init);
    m_deviceManager.playTestSound();
    m_oscReceiver.addListener(this);
    bool connected = m_oscReceiver.connect(2112);
    if(!connected){
        DBG("Could not listen on port 2112. Please ensure this port is free!");
        JUCEApplicationBase::quit();
    }else{
        DBG("Listening on port 2112");
    }
}
void PiGranulesApp::oscMessageReceived (const OSCMessage& message){
    String path = message.getAddressPattern().toString();
    if(path == "/playtest"){
        m_deviceManager.playTestSound();
    }
    

};

void PiGranulesApp::shutdown()
{
}

const String PiGranulesApp::getApplicationName()
{
    return "PiGranules";
}

const String PiGranulesApp::getApplicationVersion()
{
    return "0.01";
}

bool  PiGranulesApp::moreThanOneInstanceAllowed (){
    return false;
};

void     PiGranulesApp::anotherInstanceStarted (const String &commandLine){
};
void     PiGranulesApp::systemRequestedQuit (){
};
void     PiGranulesApp::suspended (){
};
void     PiGranulesApp::resumed (){
};

void     PiGranulesApp::unhandledException (const std::exception*,const String& sourceFilename,int lineNumber){
    DBG("Unhandled exception " + sourceFilename + String(lineNumber));
    JUCEApplicationBase::quit();
}
START_JUCE_APPLICATION (PiGranulesApp)
