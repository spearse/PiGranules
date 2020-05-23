/*
 ==============================================================================
 
 SliderController.cpp
 Created: 23 May 2020 11:04:50pm
 Author:  Stephen Pearse
 
 ==============================================================================
 */

#include "SliderController.h"



SliderController::SliderController(NormalisableRange<float>& range)
:
m_slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag,Slider::TextEntryBoxPosition::TextBoxBelow),
m_range(range)
{
    addAndMakeVisible(m_slider);
    addAndMakeVisible(m_label);
    m_label.attachToComponent(&m_slider, false);
}
void SliderController::resized(){
    m_slider.setBounds(getLocalBounds());
}
