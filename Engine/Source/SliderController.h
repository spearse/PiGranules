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
    NormalisableRange<float>& m_range;
    float m_output;
    Label m_label;
public:
    SliderController(NormalisableRange<float>& range);
    
    void resized()override;
};
