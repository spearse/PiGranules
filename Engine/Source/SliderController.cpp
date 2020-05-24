/*
 ==============================================================================
 
 SliderController.cpp
 Created: 23 May 2020 11:04:50pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#include "SliderController.h"



SliderController::SliderController():
Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag,Slider::TextEntryBoxPosition::TextBoxBelow)
{

//    m_label.attachToComponent(&m_slider, true);
    setSliderStyle(SliderStyle::RotaryHorizontalVerticalDrag);
    
    sliderMoved = [this](float){};

    
    
    onValueChange = [this](){
        if(m_range != nullptr){
            float v = getValue();
         //   DBG("v " + String(v));
            sliderMoved(v);
        }
        
    };
    
}

void SliderController::resized(){
    Slider::resized();
}

void SliderController::setup(NormalisableRange<double> *range, String name){
    m_range = range;
    setNormalisableRange(*m_range);
    m_label = name;
    repaint();
}
void SliderController::paint(Graphics& g){
    
    g.saveState();
    
    g.setColour(Colours::white);
    g.drawText(m_label,getLocalBounds(),Justification::centredTop);
    
    g.restoreState();
    
    Slider::paint(g);
};
