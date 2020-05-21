/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KadenzeDelayAudioProcessor::KadenzeDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mDryWetParameter = new AudioParameterFloat("drywet","Dry Wet",0.01, 1.0, 0.5));
    addParameter(mFeedbackParameter = new AudioParameterFloat("feedback","Feedback",0,0.98,0.5));
    addParameter(mTimeParameter = new AudioParameterFloat("delaytime","Delay Time", 0.01, MAX_DELAY_TIME, 0.5));
    
    mDelayTimeSmoothed = 0;
    
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    
    mDelayTimeInSamples = 0;
    mDelayReadHead = 0;
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
   // mDryWet = 0.5;  //Not used when using parameter
}

KadenzeDelayAudioProcessor::~KadenzeDelayAudioProcessor()
{
    if (mCircularBufferLeft != nullptr)
    {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight != nullptr)
    {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const String KadenzeDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KadenzeDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KadenzeDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KadenzeDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KadenzeDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KadenzeDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KadenzeDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String KadenzeDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void KadenzeDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void KadenzeDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    DBG(sampleRate);
    //
    mDelayTimeInSamples = sampleRate * *mTimeParameter;
    
    //The maximum delay time is 2 seconds. The sample rate is 44100 samples per second.
    //Therefore, the buffer length is 2 x 44100 = 88200 samples.
    //The first index value in the buffer is 0. The highest is 88199
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    if (mCircularBufferLeft != nullptr ) {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    
    if (mCircularBufferRight != nullptr ) {
        delete [] mCircularBufferLeft;
        mCircularBufferRight = nullptr;
    }
    
    
    if (mCircularBufferLeft == nullptr)
    {
        //The buffer is an array of floats (each sample is a float).
        mCircularBufferLeft = new float[mCircularBufferLength];
    }
    
    //Initialize all bytes in memory for this buffer to a value of 0 (sometimes memory contents are not blank)
    zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));
    
    if (mCircularBufferRight == nullptr)
    {
        mCircularBufferRight = new float[mCircularBufferLength];
    }
    
    zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float));
    
    mCircularBufferWriteHead = 0;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    mDelayTimeSmoothed = *mTimeParameter;
    
}

void KadenzeDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KadenzeDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KadenzeDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
   
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    for (int i =0; i< buffer.getNumSamples(); i++){
        
        //Smoothing on the time delay parameter
        mDelayTimeSmoothed = mDelayTimeSmoothed - 0.001*(mDelayTimeSmoothed - *mTimeParameter);
        
        //The sample rate is not expected to change. But the delay time will change based on user
        //the time parameter will be a time in seconds (from 0 to 2 seconds).
        //If the time parameter is at 0. The delay time in samples is 0.
        //This would make the write head and the read head equal to each other.
        //
        mDelayTimeInSamples = getSampleRate() * mDelayTimeSmoothed;
        
        //We write the current sample to the current location in the buffer.
        //The index for the buffer is initialized to 0 but incremented for each sample.
        //We are also adding the FEEDBACK component output from the previous sample.
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        //The read head will be a number of samples behind the write head.
        mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        
        //When mCircularBufferWriteHead < mDelayTimeInSamples, the Read Head will be less than 0
        //Because the buffer is circular, we set it to the congruent value modulo buffer length within
        //the range of 0 and the buffer length
        if (mDelayReadHead < 0){
            
            mDelayReadHead+= mCircularBufferLength;
        }
        
        //The write head will always be an integer. But the read head will not be.
        //We therefore need to break the read head into an integer part and the decimal part.
        
        //integer part of read head.
        int readHead_x = (int) mDelayReadHead;
        //
        
        //Next integer (used for circ buffer index). This will be used
        //in an interpolation calculation later on.
        int readHead_x1 = readHead_x + 1;
        
        //decimal part of read head
        float readHeadFloat = mDelayReadHead - readHead_x; // decimal part
        
        //modulo the next read head value back down to congruence if it exceeds the buffer length
        if (readHead_x1 >= mCircularBufferLength){
            
            readHead_x1 -= mCircularBufferLength;
        }
        
        
        //dsl and dsr is the OUTPUT signal of the DELAY component
        
        //We apply interpolation here to calculate the approximate value of the delayed sample, because the read head
        //is pointing at a value between two discrete points with no other points between them.
        //If the time delay is set to 0. This will only output the sample at readHead_x
        //If the time delay is set to 1. This will only output the sample at readHead_x1
        float delayed_sample_left = lin_interp(mCircularBufferLeft[readHead_x], mCircularBufferLeft[readHead_x1], readHeadFloat);//mCircularBufferLeft[(int)mDelayReadHead];
        float delayed_sample_right = lin_interp(mCircularBufferRight[readHead_x], mCircularBufferRight[readHead_x1],readHeadFloat);//mCircularBufferRight[(int)mDelayReadHead];
        
        //these are the OUTPUT signals of the FEEDBACK component
        mFeedbackLeft = delayed_sample_left* *mFeedbackParameter;
        mFeedbackRight = delayed_sample_right* *mFeedbackParameter;
        
        //output
        

        
        //buffer.addSample(0,i,delayed_sample_left);
        //buffer.addSample(1,i,delayed_sample_right);

        //Incrementing the write head.
        mCircularBufferWriteHead++;
        
        //Here we are taking the i sample from the audio channel,
        //we are multiplying it by a scalar driven by the dry/wet knob,
        //We are then adding the output from the delay circuit times the complement of the scalar
        
        //I see a problem here. If the delay is 0, I don't expect the signal to change.
        //But it looks like the value of the sample will double due to delayed sample left
        //being equal to the sample at i

        
        buffer.setSample(0,i,buffer.getSample(0,i) * (1 - *mDryWetParameter) + delayed_sample_left * *mDryWetParameter);
        buffer.setSample(1,i,buffer.getSample(1,i) * (1 - *mDryWetParameter) + delayed_sample_right * *mDryWetParameter);
        //If the write head reaches the buffer length, it must return to 0
        if (mCircularBufferWriteHead >= mCircularBufferLength){
            //reset circular buffer index to initial position
            mCircularBufferWriteHead = 0;
            
        }
        
        
    }
}

//==============================================================================
bool KadenzeDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KadenzeDelayAudioProcessor::createEditor()
{
    return new KadenzeDelayAudioProcessorEditor (*this);
}

//==============================================================================
void KadenzeDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KadenzeDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KadenzeDelayAudioProcessor();
}

float KadenzeDelayAudioProcessor::lin_interp(float inSampleX, float inSampleY, float inFloatPhase){
    
    return (1-inFloatPhase)* inSampleX + inFloatPhase*inSampleY;
}
