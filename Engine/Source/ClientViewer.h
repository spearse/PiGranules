/*
  ==============================================================================

    ClientViewer.h
    Created: 24 May 2020 8:59:54pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class ClientButton : public Button {
    String m_id;
public:
    enum State {
        Connecting = 0,
        Connected
    };
private:
    State m_state;
public:
    ClientButton(String _id);
    void paintButton (Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)override;
    void connected();
};


class ClientViewer : public Component{
    std::vector<Component*> m_children;
    
public:
    ClientViewer();
    void addClient(Component* child);
    void resized()override;
    
    
};
