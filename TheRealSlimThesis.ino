// Title: oscillator
// Description: Control a sine wave freq with a knob
// Hardware: Daisy Seed
// Author: Ben Sergentanis
// Breadboard
// https://raw.githubusercontent.com/electro-smith/DaisyExamples/master/seed/Osc/resources/Osc_bb.png
// Schematic:
// https://raw.githubusercontent.com/electro-smith/DaisyExamples/master/seed/Osc/resources/Osc_schem.png
/*
#define DO 130.81
#define DOd 138.59
#define RE 146.83
#define REd 155.56
#define MI 164.81
#define FA 174.61
#define FAd 185.00
#define SOL 196.00
#define SOLd 207.65
#define LA 220.00
#define LAd 233.08
#define SI 246.94
#define DO2 261.63
*/

#define DO 65.41
#define DOd 69.30
#define RE 73.42
#define REd 77.78
#define MI 82.41
#define FA 87.31
#define FAd 92.50
#define SOL 98.00
#define SOLd 103.83
#define LA 110.00
#define LAd 116.54
#define SI 123.47
#define DO2 130.81

#define TONO 1.122462
#define SEMITONO 1.059463

#include "DaisyDuino.h"
#include "Oscillators.h"
DaisyHardware hw;

size_t numChannels;

int maggiore= 1;
int oldmaggiore= 1;

float primo;
float secondo;
float terzo;
float quarto;
float quinto;
float sesto;
float settimo;
float ottavo;

float tonica = 0;
int oldtonica = 0;

float parameterknob;
float func1knob;
float func2knob;
float windowknob;
int old;

Adsr env_0;
Adsr env_1;
float a_0;
float d_0;
float s_0;
float r_0;
float a_1;
float d_1;
float s_1;
float r_1;

/*
static PhaseDistortedOsc osc_0;
static PhaseDistortedOsc osc_1;
static PhaseDistortedOsc osc_2;
static PhaseDistortedOsc osc_3;
static PhaseDistortedOsc osc_4;
static PhaseDistortedOsc osc_5;
static PhaseDistortedOsc osc_6;
static PhaseDistortedOsc osc_7;
*/
static PhaseDistortedOsc osc_0;



Switch button_0;
Switch button_1;
Switch button_2;
Switch button_3;
Switch button_4;
Switch button_5;
Switch button_6;
Switch button_7;

void MyCallback(float **in, float **out, size_t size) {
  
  for (size_t i = 0; i < size; i++) {


    auto envelope_0 = env_0.Process(button_0.Pressed() || button_1.Pressed() || button_2.Pressed() || button_3.Pressed() || button_4.Pressed() || button_5.Pressed() || button_6.Pressed() || button_7.Pressed());
    //auto envelope_1 = env_1.Process(button_1.Pressed());
    //auto envelope_2 = env_2.Process(button_2.Pressed());
    auto envelope_1 = env_1.Process(button_0.Pressed() || button_1.Pressed() || button_2.Pressed() || button_3.Pressed() || button_4.Pressed() || button_5.Pressed() || button_6.Pressed() || button_7.Pressed());


    

    osc_0.setParameter(envelope_1);
    //osc_1.setParameter(parameterknob * envelope_1);
    //osc_2.setParameter(parameterknob * envelope_2);
  
    

    float sample = osc_0.getNextAudioSample() * envelope_0;

    

    
    //sample += osc_2.getNextAudioSample() * envelope_2; //static_cast<float>(button_0.Pressed());
    
    
    //osc.SetFreq(mtof(pitchknob*pitchknob2));
    
    //float sample = sample_0 + sample_1;
    /*
    for (size_t ch = 0; ch < numChannels; ch++) {
      out[ch][i] = sample * 0.3;
    }*/
    out[0][i] = sample * 0.3;
  }
}


void setup() {
  Serial.begin(19200);
  float sampleRate;
  // Initialize for Daisy pod at 48kHz
  hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
  numChannels = hw.num_channels;
  sampleRate = DAISY.get_samplerate();

  env_0.Init(sampleRate);
  env_1.Init(sampleRate);


  button_0.Init(1000,false,0,0);
  button_1.Init(1000,false,1,0);
  button_2.Init(1000,false,2,0);
  button_3.Init(1000,false,3,0);
  button_4.Init(1000,false,4,0);
  button_5.Init(1000,false,5,0);
  button_6.Init(1000,false,6,0);
  button_7.Init(1000,false,7,0);



  osc_0.prepareToPlay(sampleRate);
  osc_0.setFrequency(261.626);

  setFreq();
  //osc_1.prepareToPlay(sampleRate);
  //osc_1.setFrequency(293.665);
  //osc_2.prepareToPlay(sampleRate);
  //osc_2.setFrequency(329.628);
  //osc_3.prepareToPlay(sampleRate);
  //osc_3.setFrequency(349.228);
  //osc_4.prepareToPlay(sampleRate);
  //osc_4.setFrequency(391.995);
  //osc_5.prepareToPlay(sampleRate);
  //osc_5.setFrequency(440);
  //osc_6.prepareToPlay(sampleRate);
  //osc_6.setFrequency(493.883);
  //osc_7.prepareToPlay(sampleRate);
  //osc_7.setFrequency(523.251);


  DAISY.begin(MyCallback);
}


void setFreq()
{

  switch(oldtonica)
  {
    case 0:
    tonica = DO;
    break;
    case 1:
    tonica = DOd;
    break;
    case 2:
    tonica = RE;
    break;
    case 3:
    tonica = REd;
    break;
    case 4:
    tonica = MI;
    break;
    case 5:
    tonica = FA;
    break;
    case 6:
    tonica = FAd;
    break;
    case 7:
    tonica = SOL;
    break;
    case 8:
    tonica = SOLd;
    break;
    case 9:
    tonica = LA;
    break;
    case 10:
    tonica = LAd;
    break;
    case 11:
    tonica = SI;
    break;
    case 12:
    tonica = DO2;
    break;
    default:
    break;  
  }

  if (oldmaggiore == 1)
  {
    primo = tonica;
    secondo = tonica * TONO;
    terzo = secondo * SEMITONO;
    quarto = terzo * TONO;
    quinto = quarto * TONO;
    sesto = quinto * SEMITONO;
    settimo = sesto * TONO;
    ottavo = settimo * TONO;
  }
  else if ( maggiore == 0)
  {
    primo = tonica;
    secondo = tonica * TONO;
    terzo = secondo * TONO;
    quarto = terzo * SEMITONO;
    quinto = quarto * TONO;
    sesto = quinto * TONO;
    settimo = sesto *TONO;
    ottavo = settimo * SEMITONO;
  }
}


void loop() { 

  //manipulate table with a
  button_0.Debounce();
  button_1.Debounce();
  button_2.Debounce();
  button_3.Debounce();
  button_4.Debounce();
  button_5.Debounce();
  button_6.Debounce();
  button_7.Debounce();

 
  //pitchknob = 24.0 + ((analogRead(A0) / 1023.0) * 60.0); 
  ///////////////parameterknob = analogRead(A11) / 1023.0;
  func1knob = analogRead(A8) / 102.3 /2 ;
  func2knob = analogRead(A9) / 102.3 /2 ;
  ////////////////windowknob = analogRead(A10) /102.3 /2;

  tonica = analogRead(A11) /1023.0 * 12;
  maggiore = static_cast<int>(analogRead(A10)/512.0);

  if (static_cast<int>(tonica) != oldtonica)
  {
    oldtonica = static_cast<int>(tonica);
    setFreq();
  }

  if (maggiore != oldmaggiore)
  {
    oldmaggiore = maggiore;
    setFreq();
  }



  
  a_0 = analogRead(A0) / 1023.0;
  d_0 = analogRead(A1) / 1023.0;
  s_0 = analogRead(A2) / 1023.0;
  r_0 = analogRead(A3) / 1023.0;

  a_1 = analogRead(A4) / 1023.0;
  d_1 = analogRead(A5) / 1023.0;
  s_1 = analogRead(A6) / 1023.0;
  r_1 = analogRead(A7) / 1023.0;
  

  if (button_0.Pressed() && old != 0)
    osc_0.setFrequency(primo);
    old = 0;
  if (button_1.Pressed() && old != 1)
    osc_0.setFrequency(secondo);
    old = 1;
  if (button_2.Pressed() && old != 2)
    osc_0.setFrequency(terzo);
    old = 2;
  if (button_3.Pressed() && old != 3)
    osc_0.setFrequency(quarto);
    old = 3;
  if (button_4.Pressed() && old != 4)
    osc_0.setFrequency(quinto);
    old = 4;
  if (button_5.Pressed() && old != 5)
    osc_0.setFrequency(sesto);
    old = 5;
  if (button_6.Pressed() && old != 6)
    osc_0.setFrequency(settimo);
    old = 6;
  if (button_7.Pressed() && old != 7)
    osc_0.setFrequency(ottavo);  
    old = 7;

  env_0.SetTime(ADSR_SEG_ATTACK, a_0);
  env_0.SetTime(ADSR_SEG_DECAY, d_0);
  env_0.SetTime(ADSR_SEG_RELEASE, r_0);
  env_0.SetSustainLevel(s_0);

  
  env_1.SetTime(ADSR_SEG_ATTACK, a_1);
  env_1.SetTime(ADSR_SEG_DECAY, d_1);
  env_1.SetTime(ADSR_SEG_RELEASE, r_1);
  env_1.SetSustainLevel(s_1);

  
  //pdosc.setFrequency(mtof(pitchknob));
  //osc_0.setParameter(parameterknob);
  //osc_0.setWindow(static_cast<int>(windowknob));
  osc_0.setFunc1(static_cast<int>(func1knob));
  osc_0.setFunc2(static_cast<int>(func2knob));
  //osc_1.setParameter(parameterknob);
  //osc_1.setWindow(static_cast<int>(windowknob));
  //osc_1.setFunc1(static_cast<int>(func1knob));
  //osc_1.setFunc2(static_cast<int>(func2knob));
  Serial.print(maggiore);
  Serial.println(oldmaggiore);
  }
