/*
  ==============================================================================

    SliderController.h
    Created: 23 May 2020 11:04:50pm
    Author:  Stephen Pearse

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SliderController : public Slider{
    NormalisableRange<double>* m_range = nullptr;
    String m_label;
    
public:
    SliderController();
    void setup(NormalisableRange<double>* range,String name);
    void resized()override;
    void paint(Graphics& g)override;
    std::function<void(float)> sliderMoved;
};
