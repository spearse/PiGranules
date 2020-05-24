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
        MessageManagerLock lock;
        m_children.push_back(child);
        addAndMakeVisible( m_children.back());
        setSize(40,40 + int(40 * m_children.size()));
        child->repaint();
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
    Colour col;
    if(m_state == Connecting){
        col = Colours::darkorange;
    }else if (m_state == Connected){
        col =  Colours::darkgreen;
    }
    g.setColour(col);
    g.fillEllipse(size.toFloat());
    g.setColour(col.contrasting());
    g.drawText(m_id, getLocalBounds(), Justification::centred);
    
}
void ClientButton::connected(){
    
    m_state = Connected;
    repaint();
}
