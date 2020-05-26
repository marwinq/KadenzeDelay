/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    parameterArray[0]= dryWetParameter;
    parameterArray[1]= feedbackParameter;
    parameterArray[2]= delayTimeParameter;
    
    //Slider Declaration
    
    sliderArray[0]= &mDryWetSlider;
    sliderArray[1]= &mFeedbackSlider;
    sliderArray[2]= &mDelayTimeSlider;
    
    //Function prototype for initializing multiple sliders.. 
    
    for (int i=0; i <3;i++){
        
        sliderArray[i]->setBounds((i)*100,0,100,100);
        sliderArray[i]->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        sliderArray[i]->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 30);
        sliderArray[i]->setRange(parameterArray[i]->range.start, parameterArray[i]->range.end);
        sliderArray[i]->setValue(*parameterArray[i]);
        addAndMakeVisible(sliderArray[i]);
        
        sliderArray[i]->onValueChange = [=] { *parameterArray[i] = sliderArray[i]->getValue() ;};
        sliderArray[i]->onDragStart = [=] { parameterArray[i]->beginChangeGesture();};
        sliderArray[i]->onDragEnd = [=] { parameterArray[i]->endChangeGesture();};
    }
    
    //Function should take in an array of sliders and their respective parameters and initialize the sliders.
    //sizeof operator to state the upper bound results in crashes.
    //initializeSlider(sliderArray,parameterArray,3);
     
    
    //This results in a crash. Need to continue debugging.
   // initializeSlider(sliderArray,parameterArray, sizeof(sliderArray));

    

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


void KadenzeDelayAudioProcessorEditor::initializeSlider(Slider* s[], AudioParameterFloat* p[], int l)
{
    for (int i=0; i < l;i++){
        
        s[i]->setBounds((i)*100,0,100,100);
        s[i]->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        s[i]->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 30);
        s[i]->setRange(p[i]->range.start, p[i]->range.end);
        s[i]->setValue(*p[i]);
        addAndMakeVisible(s[i]);
        
        s[i]->onValueChange = [=] { *p[i] = s[i]->getValue() ;};
        s[i]->onDragStart = [=] { p[i]->beginChangeGesture();};
        s[i]->onDragEnd = [=] { p[i]->endChangeGesture();};
    }
 
}

