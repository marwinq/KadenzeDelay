/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <array>

//==============================================================================
KadenzeDelayAudioProcessorEditor::KadenzeDelayAudioProcessorEditor (KadenzeDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* feedbackParameter = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* delayTimeParameter = (AudioParameterFloat*)params.getUnchecked(2);
    
    AudioParameterFloat* parameterArray[3];
    
    std::array<AudioParameterFloat *,3> pArray {dryWetParameter,feedbackParameter,delayTimeParameter};
    std::array<Slider *, 3> sArray {&mDryWetSlider, &mFeedbackSlider, &mDelayTimeSlider};
    
    parameterArray[0]= dryWetParameter;
    parameterArray[1]= feedbackParameter;
    parameterArray[2]= delayTimeParameter;
    
    //Slider Declaration
    
    sliderArray[0]= &mDryWetSlider;
    sliderArray[1]= &mFeedbackSlider;
    sliderArray[2]= &mDelayTimeSlider;
    

    initializeSlider(sArray,pArray);


}

KadenzeDelayAudioProcessorEditor::~KadenzeDelayAudioProcessorEditor()
{
}

//==============================================================================
void KadenzeDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::lightviolet); // Colours::violet
    g.setFont (15.0f);
    g.drawFittedText ("Delay Prototype", getLocalBounds(), Justification::centred, 1);
}

void KadenzeDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void KadenzeDelayAudioProcessorEditor::initializeSlider(std::array<Slider *,3> s, std::array<AudioParameterFloat *,3> p)
{
    for (int i=0; i < s.size();i++){
        
        s.at(i)->setBounds((i)*100,0,100,100);
        s.at(i)->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        s.at(i)->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 30);
        s.at(i)->setRange(p.at(i)->range.start, p.at(i)->range.end);
        s.at(i)->setValue(*p.at(i));
        addAndMakeVisible(s.at(i));
        
        s.at(i)->onValueChange = [=] { *p.at(i) = s.at(i)->getValue() ;};
        s.at(i)->onDragStart = [=] { p.at(i)->beginChangeGesture();};
        s.at(i)->onDragEnd = [=] { p.at(i)->endChangeGesture();};
    }
 
}

