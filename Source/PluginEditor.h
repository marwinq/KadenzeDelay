/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <array>

//==============================================================================
/**
*/
class KadenzeDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    KadenzeDelayAudioProcessorEditor (KadenzeDelayAudioProcessor&);
    ~KadenzeDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void initializeSlider(std::array<Slider *,3> s, std::array<AudioParameterFloat *,3> p);

private:
    
    Slider mDryWetSlider;
    Slider mFeedbackSlider;
    Slider mDelayTimeSlider;
    
    Slider *sliderArray[3];
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KadenzeDelayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeDelayAudioProcessorEditor)
};
