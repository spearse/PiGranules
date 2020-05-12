/*
  ==============================================================================

    Application.h
    Created: 12 May 2020 9:42:14pm
    Author:  Stephen Pearse

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#pragma once
class PiGranulesApp  : public JUCEApplicationBase, public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
public:
    PiGranulesApp()  {}
    ~PiGranulesApp() {}
 
    void initialise (const String& commandLine) override;
    
    void shutdown() override;
    
    const String getApplicationName() override;
    const String getApplicationVersion() override;
    bool  moreThanOneInstanceAllowed ()override;
 
    virtual void     anotherInstanceStarted (const String &commandLine)override;
    virtual void     systemRequestedQuit ()override;
    virtual void     suspended ()override;
    virtual void     resumed ()override;
    virtual void     unhandledException (const std::exception*,const String& sourceFilename,int lineNumber)override ;
    virtual void oscMessageReceived (const OSCMessage& message)override;

private:
    AudioDeviceManager m_deviceManager;
    OSCReceiver m_oscReceiver;
};
 
