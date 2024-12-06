#include <MIDI.h>

const int btnStart = 5;
const int btnSelect = 6;
const int leds[5] = {12,11,10,9,8};


int btnStartState = 0;
int btnSelectState = 0;
unsigned long btnStartDebounce = 0;
unsigned long btnSelectDebounce = 0;
bool midiSent = false;
bool selectedTriggered = false;
int selected = 0;
int ccVals[5] = {1,2,3,4,5};
bool reset = false;
unsigned long debounceDelay = 200;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial,midiOut);

void setup() {
  pinMode(btnStart, INPUT);
  pinMode(btnSelect, INPUT);
  
  for(int i=0;i<5;i++){
    pinMode(leds[i],OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  digitalWrite(leds[selected],HIGH);
  
  midiOut.begin();
  delay(100);
}

void loop() {
  btnStartState = digitalRead(btnStart);
  btnSelectState = digitalRead(btnSelect);
  if(btnStartState==HIGH){
    if(!midiSent){
      btnStartDebounce = millis();
      midiSent = true;
      midiOut.sendControlChange(ccVals[selected],127,16);
      delay(10);
      midiOut.sendControlChange(ccVals[selected],0,16);
    } 
    
  } else {
    if(midiSent && millis()-btnStartDebounce>debounceDelay){
      midiSent = false;
    }
  }

  if(btnSelectState==HIGH){
    if(!selectedTriggered){
      btnSelectDebounce = millis();
      selectedTriggered = true;
      digitalWrite(leds[selected],LOW);
      selected++;
      if(selected>4)selected = 0;
      digitalWrite(leds[selected],HIGH);
      
    } else {
      if(millis()-btnSelectDebounce>500){
        digitalWrite(leds[selected],LOW);
        delay(50);
        digitalWrite(leds[selected],HIGH);
        delay(50);
      } 
    }  
  } else {
    if(selectedTriggered && millis()-btnSelectDebounce>debounceDelay){
      if(selectedTriggered && millis()-btnSelectDebounce>500){
        digitalWrite(leds[selected],LOW);
        selected = 0;
        digitalWrite(leds[selected],HIGH);
      }
      selectedTriggered = false;
    }
  }
 

}
