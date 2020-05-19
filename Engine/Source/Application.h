/*
  ==============================================================================

    Application.h
    Created: 12 May 2020 9:42:14pm
    Author:  Stephen Pearse

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "Engine.h"
#include <vector>

#pragma once
class PiGranulesApp  : public JUCEApplicationBase, public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>
{
public:
    PiGranulesApp();
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

    
    void searchForHost();
    void hostFound();
    void sendHostInformation(String targetIp);
    
    void sendToExternal(const OSCMessage& msg);
    
    void loadSounds(String folder);
    
    //for all clients.. receiving and setting.... - if host forward these on???
    void setSpawnRate(float rate);
    void setGrainSize(int size);
    void setNumGrains(int numGrains);
    void setPlayBackSpeed(float speed);
    void setGrainPitch(float pitch);
    void setStartPos(float pos);
    
    friend class Engine;
    
    void sendToClient(int index, OSCMessage msg);
    void sentToAllClients(const OSCMessage& msg);
    void spawnOnClient(int index);
private:
    AudioDeviceManager m_deviceManager;
    OSCReceiver m_oscReceiver,m_hostOscReceiver;
    bool m_hostMode = false;
    bool m_externalUIConnected = false;
    IPAddress m_addr,m_hostAddr;
    OSCSender m_sender,m_externalUISender;
    bool m_searchingForHostState = false;
    std::vector<IPAddress> m_childAddresses;
    int m_childId = -1;
    
    Engine m_audioEngine;
    
    
};
 
