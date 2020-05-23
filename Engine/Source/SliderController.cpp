/*
 ==============================================================================
 
 SliderController.cpp
 Created: 23 May 2020 11:04:50pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#include "SliderController.h"



SliderController::SliderController():
m_slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag,Slider::TextEntryBoxPosition::TextBoxBelow)
{
    addAndMakeVisible(m_slider);
    addAndMakeVisible(m_label);
    m_label.attachToComponent(&m_slider, false);
    
    sliderMoved = [this](float){};

    
    m_slider.onValueChange = [this](){
        if(m_range != nullptr){
            float v = m_slider.getValue();
            DBG("v " + String(v));
            sliderMoved(v);
        }
        
    };
    
}
void SliderController::resized(){
    m_slider.setBounds(getLocalBounds());
}

void SliderController::setup(NormalisableRange<double> *range, String name){
    m_range = range;
    m_slider.setNormalisableRange(*m_range);
    m_label.setText(name, dontSendNotification);
    repaint();
}
