/*
 ==============================================================================
 
 Application.cpp
 Created: 12 May 2020 9:42:14pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#include "Application.h"

PiGranulesApp::PiGranulesApp():
    m_audioEngine(this)
{
    
}

void PiGranulesApp::initialise (const String& commandLine)
{
    DBG("Pi Granules Starting");
    String init = m_deviceManager.initialiseWithDefaultDevices(0, 2);
    DBG(init);
   
    m_addr = IPAddress::getLocalAddress();
    DBG("Ip of device: " + m_addr.toString());
    StringArray cmds = JUCEApplicationBase::getCommandLineParameterArray();
    for(auto cmd : cmds){
        DBG(cmd);
        if(cmd == "--host"){
            m_hostMode = true;
        }
        static bool LoadFiles = false;
        if(LoadFiles == true){
            
            loadSounds(cmd);
            
            LoadFiles = false;
        }
        if(cmd == "--files"){
            LoadFiles = true;
        }
        
    }
    m_deviceManager.addAudioCallback(&m_audioEngine);
    
    m_deviceManager.playTestSound();
    
    
    m_oscReceiver.addListener(this);
    bool connected = m_oscReceiver.connect(2112);
    if(!connected){
        DBG("Could not listen on port 2112. Please ensure this port is free!");
        JUCEApplicationBase::quit();
    }else{
        DBG("Listening on port 2112");
    }
    
    
    if(m_hostMode){
        m_hostOscReceiver.addListener(this);
        m_hostAddr = m_addr;
         bool connected = m_oscReceiver.connect(2113);
        DBG("Setting up host");
        if(!connected){
               DBG("Could not setup host to listen on port 2113. Please ensure this port is free!");
               JUCEApplicationBase::quit();
           }else{
               DBG("Host on port 2113");
           }
    }else{
        searchForHost();
    }
    if(!m_externalUISender.connect("127.0.0.1", 2111)){
        DBG("Warning: Couldn't connect to external UI");
    };
    
    
}


inline String GetStringSafely(const OSCMessage& msg,int pos){
    String t;
    if(pos < msg.size() && pos >=0){
        if(msg[pos].isString()){
            t = msg[pos].getString();
        }
    }
    return t;
};

inline int GetIntSafely(const OSCMessage& msg,int pos){
    int t = -666;
    if(pos < msg.size() && pos >=0){
        if(msg[pos].isInt32()){
            t = msg[pos].getInt32();
        }else if(msg[pos].isFloat32()){
            t = int(msg[pos].getFloat32());
        }
    }
    return t;
};

inline int GetFloatSafely(const OSCMessage& msg,int pos){
    float t = -666;
    if(pos < msg.size() && pos >=0){
        if(msg[pos].isFloat32()){
            t = msg[pos].getFloat32();
        }else if(msg[pos].isInt32()){
            t = float(msg[pos].isInt32());
        }
    }
    return t;
};
inline void setValue(std::function<void>(float),const OSCMessage& message,int index){
    
}

void PiGranulesApp::oscMessageReceived (const OSCMessage& message){
    String path = message.getAddressPattern().toString();
    if(path == "/playtest"){
        m_deviceManager.playTestSound();
    }else if(path == "/searchforhost"){
        searchForHost();
    }else if(path == "/queryhost" && m_hostMode == true){
        //send back your details to confirm.
        
        String clientIp = GetStringSafely(message, 0);
        DBG("New client is attempting to connect:" + clientIp );
        if(clientIp.isNotEmpty()){
            sendHostInformation(clientIp);
        }
        
        //then have child confirm
    }else if(path == "/hostinfo" && m_hostMode == false){
        String t = GetStringSafely(message, 0);
        
        DBG("Host IP received: " + t);
        if(t.isNotEmpty()){
            m_searchingForHostState = false;
            m_hostAddr = IPAddress(t);
            m_sender.connect(m_hostAddr.toString(), 2113);
            OSCMessage msg("/hostconfirmed");
            msg.addString(m_addr.toString());
            if(!m_sender.send(msg)){
                DBG("Warning: Couldn't confirm host");
                sendToExternal(OSCMessage("/nohostconfirm"));
            }else{
                DBG("Requesting ID from host");
            }
        }else{
            DBG("Warning: Host didn't send an IP!");
            sendToExternal(OSCMessage("/nohostip"));
        }
        
    }else if (path == "/hostconfirmed" && m_hostMode == true){
        //add the details back as a child to be used...
        
        String childIp = GetStringSafely(message, 0);
        DBG("Registering " + childIp + " as child");
        if(childIp.isNotEmpty()){
            bool childKnown = false;
            int knownLoc = -1;
            for(int n = 0 ; n < m_childAddresses.size();++n){
                if(m_childAddresses[n].toString() == childIp ){
                    childKnown = true;
                    knownLoc = n+1;
                }
            }
            int childId = -1;
            if(!childKnown){
                m_childAddresses.push_back(IPAddress(childIp));
                childId = int( m_childAddresses.size());
            }else{
                childId = knownLoc;
            }
                  
            DBG("Assigned " + childIp + " child id " + String(childId));
            OSCMessage msg("/childid");
            msg.addInt32(childId);
            if(m_sender.connect(childIp, 2112)){
                if(!m_sender.send(msg)){
                    DBG("Warning: Couldn't send ID to child");
                    sendToExternal(OSCMessage("/cantconnecttochild"));
                }else{
                    DBG("Id sent to child " + childIp);
                }
            }else{
                DBG("Warning: couldn't connect to child");
                sendToExternal(OSCMessage("/cantconnecttochild"));
            }
            
        }else{
            DBG("Warning: Child didn't send IP");
            sendToExternal(OSCMessage("/nochildip"));
        }
        
    }else if(path == "/childid" && m_hostMode == false){
        
        int id = GetIntSafely(message, 0);
        if(id >=0){
            m_childId = id;
            DBG("Assigned ID :" + String(id));
            OSCMessage msg("/childid");
            msg.addInt32(m_childId);
            sendToExternal(msg);
        }
        
    }else if(path == "/spawnrate"){
        if(!message.isEmpty()){
            float spawnR = GetFloatSafely(message, 0);
            if(spawnR != -666){
                m_audioEngine.getCloudCreator().set_spawnRate(spawnR);
            }
        }
    }else if (path == "/grainsize"){
        if(!message.isEmpty()){
            float size = GetIntSafely(message, 0);
            if(size != -666){
                m_audioEngine.getCloudCreator().set_grainSize(size);
            }
        }
    }else if(path == "/numgrains"){
        if(!message.isEmpty()){
            float numG = GetIntSafely(message, 0);
            if(numG != -666){
                m_audioEngine.getCloudCreator().set_numGrains(numG);
            }
        }
    }else if(path == "/playspeed"){
        if(!message.isEmpty()){
            float speed = GetFloatSafely(message, 0);
            if(speed != -666){
                m_audioEngine.getCloudCreator().set_spawnRate(speed);
            }
        }
    }else if (path == "/grainpitch"){
        if(!message.isEmpty()){
            float pitch = GetFloatSafely(message, 0);
            if(pitch != -666){
                m_audioEngine.getCloudCreator().set_grainPitch(pitch);
            }
        }
    }else if(path == "/startpos"){
        if(!message.isEmpty()){
            float start = GetFloatSafely(message, 0);
            if(start != -666){
                m_audioEngine.getCloudCreator().set_startPos(start);
            }
        }
    }else if(path == "/spawnmode"){
        
    }else if(path == "/spawn"){
        m_audioEngine.getCloudCreator().spawn();
    }
    
    

};
void PiGranulesApp::sendHostInformation(String targetIp){
    m_sender.connect(targetIp, 2112);
    OSCMessage msg("/hostinfo");
    msg.addString(m_addr.toString());
    if(!m_sender.send(msg)){
        DBG("Warning could not send out host information");
    };
}
void PiGranulesApp::searchForHost(){
    if(m_hostMode==true){
        DBG("You are on the host!");
        return;
    }
    DBG("Searching for host");

    m_searchingForHostState = true;
    for(int n = 0 ; n < 255; ++n){
        
        String newIpString = String("192.168.0.") + String(n);
        
        if(m_sender.connect(newIpString, 2113)){
            OSCMessage msg("/queryhost");
            
            msg.addString(m_addr.toString());
            
            m_sender.send(msg);
            
            m_sender.disconnect();
        };
        
    }
    Timer::callAfterDelay(5000, [this](){
        if(m_searchingForHostState == true){
            DBG("Warning: Could not find host");
            sendToExternal(OSCMessage("/hostmissing"));
        }
    });
}


void PiGranulesApp::sendToExternal(const OSCMessage& msg){
    if(m_externalUIConnected){
        m_externalUISender.send(msg);
    }
}

void PiGranulesApp::loadSounds(String folder){
    File dir(folder);
    
    DBG("Attempting to load " + folder );
    if(dir.isDirectory()){
        Array<File> files = dir.findChildFiles (File::TypesOfFileToFind::findFiles,false,"*.wav;*.aiff;*.WAV,*.AIFF");
        m_audioEngine.loadFiles(files);
        m_audioEngine.setMainFile(files.size()+1);
        m_audioEngine.activate();
    }else{
        DBG(folder + " is not a folder");
    }
}

void PiGranulesApp::sendToClient(int index, OSCMessage msg){
    if(index >=0 && index < m_childAddresses.size()){
        if(m_sender.connect(m_childAddresses[index].toString(), 2112)){
                m_sender.send(msg);
        };
        
    }
    
}
void PiGranulesApp::spawnOnClient(int index){
    OSCMessage msg("/spawn");
    sendToClient(index, msg);
}

////////


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
