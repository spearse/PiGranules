/*
  ==============================================================================

    SliderController.h
    Created: 23 May 2020 11:04:50pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class SliderController : public Component{
    Slider m_slider;
    NormalisableRange<double>* m_range = nullptr;
    Label m_label;
public:
    SliderController();
    void setup(NormalisableRange<double>* range,String name);
    void resized()override;
    
    std::function<void(float)> sliderMoved;
};
