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
#include "HostInterface.h"

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
    void setRemoteClientMode(CloudCreator::Mode mode);
    void setCreatorMode(CloudCreator::Mode mode);
    
    friend class Engine;
    friend class HostInterface;
    void sendToClient(int index, OSCMessage msg);
    void sentToAllClients(const OSCMessage& msg);
    void spawnOnClient(int index);
    void testSound(int clientIndex);
    
     class MainWindow    : public DocumentWindow
        {
        public:
            MainWindow (String name,PiGranulesApp* parent)  : DocumentWindow (name,
                                                        Desktop::getInstance().getDefaultLookAndFeel()
                                                                              .findColour (ResizableWindow::backgroundColourId),
                                                        DocumentWindow::allButtons),
                m_parent(parent)
            {
                setUsingNativeTitleBar (true);
                setContentOwned (m_interface = new HostInterface(parent), true);

               #if JUCE_IOS || JUCE_ANDROID
                setFullScreen (true);
               #else
                setResizable (false, true);
                centreWithSize (1000, 800);
               #endif

                setVisible (true);
            }

            void closeButtonPressed() override
            {
                // This is called when the user tries to close this window. Here, we'll just
                // ask the app to quit when this happens, but you can change this to do
                // whatever you need.
                m_parent->systemRequestedQuit();

            }

            /* Note: Be careful if you override any DocumentWindow methods - the base
               class uses a lot of them, so by overriding you might break its functionality.
               It's best to do all your work in your content component instead, but if
               you really have to override any DocumentWindow methods, make sure your
               subclass also calls the superclass's method.
            */
            HostInterface* m_interface;
        private:
            PiGranulesApp* m_parent;
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
        };

       

    
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
    
    std::map<int,bool> m_remoteFileLoadStates;
    AudioSampleBuffer m_remoteAudioFileBuffer;
    
    std::unique_ptr<MainWindow> mainInteface;
    std::vector<std::shared_ptr<ClientButton > > m_clientButtons;
};
 
