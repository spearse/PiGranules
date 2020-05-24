/*
  ==============================================================================

    ClientViewer.cpp
    Created: 24 May 2020 8:59:54pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#include "ClientViewer.h"



ClientViewer::ClientViewer(){
    
    setSize(40,40);
}

void ClientViewer::addClient(Component* child){
    
    if(child!= nullptr){
        
        m_children.push_back(child);
        addAndMakeVisible( m_children.back());
        setSize(40,40 + int(40 * m_children.size()));
        resized();
            
    }
    
}
void ClientViewer::resized(){
    auto size  = getLocalBounds();
    
    for(auto* c : m_children){
        c->setBounds(size.removeFromTop(40));
    }
    
}

ClientButton::ClientButton(String _id):
    Button(_id),
    m_id(_id),
    m_state(Connecting)
{
    
}


void ClientButton::paintButton (Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown){
    
    
    auto size = getLocalBounds();
    if(m_state == Connecting){
        g.setColour(Colours::darkorange);
    }else if (m_state == Connected){
        g.setColour(Colours::darkgreen);
    }
    g.fillEllipse(size.toFloat());
}
void ClientButton::connected(){
    
    m_state = Connected;
    repaint();
}
