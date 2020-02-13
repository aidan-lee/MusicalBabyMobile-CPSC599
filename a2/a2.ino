/*
 * Note that this code was created in part by adapting potions of the code available from the following sources
 * 1. https://www.arduinoplatform.com/arduino-project/create-a-touch-button-with-copper-aluminum-foil/
 * 2. https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
 * 3. https://solarianprogrammer.com/2017/01/31/arduino-buzzer-player-adjustable-volume-playing-fur-elise-beethoven/
 */

#include <CapacitiveSensor.h>
#include "pitches.h" 

// Constructing 4 CapacitiveSensor objects
CapacitiveSensor cs_13_12 = CapacitiveSensor(13, 12);  
CapacitiveSensor cs_9_8 = CapacitiveSensor(9, 8);    
CapacitiveSensor cs_7_6 = CapacitiveSensor(7, 6);     
CapacitiveSensor cs_5_4 = CapacitiveSensor(5, 4);      

// The minimum value needed for a HIGH reading on the capacitive sensors 
int sensorThreshold = 500; 

// An array of pins used for speaker output
int speakerPins[4] = {31, 33, 35, 37};
// A list of CapacitiveSensor objects
CapacitiveSensor capacitiveSensors[4] = { cs_13_12,  cs_9_8, cs_7_6, cs_5_4  };

// The pins used for the ultrasonic sensor 
int trigPin = 23;
int echoPin = 22;

// A list of frequencies, used to determine which note is played  
int lowTones[4] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4 };
int highTones[4] = {NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5 };

const int NUMBER_OF_SENSORS = 4;
const int DISTANCE_FOR_TONE_SWITCH = 4;

void setup()
{
  Serial.begin(9600);

  setupSensors();
  setupSpeakerPins();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}
void loop()
{
  // Read distance from the ultrasonic sensor
  int distance = readDistance();

  // Check each capcitive sensor and react accordingly  
  for (int i = 0; i < NUMBER_OF_SENSORS; i++) 
  {
    long sensorValue =  capacitiveSensors[i].capacitiveSensor(30);

    // If sensor is high, play appropriate note 
    if (sensorValue > sensorThreshold) 
    {
      int note = getNote(distance, i);
      tone(speakerPins[i], note); 
    }
    // Otherwise, play nothing
    else 
    {
      noTone(speakerPins[i]);
    }                                            
  }
     
  delay(10);                           
}

// Read the distance from the ultrasonic sensor 
int readDistance() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  int distance= duration*0.034/2;
  
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

// Initializes the capactive sensors 
void setupSensors() 
{
  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
      capacitiveSensors[i].set_CS_AutocaL_Millis(0xFFFFFFFF); 
  }
}

// Declares all speaker pins as output pins
void setupSpeakerPins() 
{
  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
     pinMode(speakerPins[i], OUTPUT);
  }
}

// Determines the note to play through the speaker, depending on which sensor is
// reading high, and the distance value
int getNote(int distance, int index) 
{
  if (distance < DISTANCE_FOR_TONE_SWITCH) {
    return highTones[index];
  }
  return lowTones[index];
}
