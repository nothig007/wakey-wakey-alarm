#include <IRremote.hpp> // lib for IR communication btwn ir receiver and remote
#define IR_RECEIVE_PIN 8
int Switch = 2; // Digital Pin of Switch
int Led[] = {12, 13}; // Digital Pin of Leds 
int LED[] = {5, 6, 9}; //Pwm Pin of Leds
int BuzzPin = 4; //Digtal Pin of Buzzer
int ObsAV = 7; //Digital Pin of Obstacle Avoidance sensor
int LDR = A0; //Analog Pin of LDR(Light Dependent Resistor)
int SwitchState = LOW; //SwitchState of Switch
int lastSwitchState = LOW; //SwitchState of Switch before loop
unsigned long lastDebounceTime = 0;
unsigned long lastDebounceTime_2 = 0;
unsigned long debounceDelay = 50;      
unsigned long debounceDelay_2 = 6000;
bool first_condition = false;
bool logic = false;
bool stoploop = false;
int ldr_reading = 0;
int ifChecker = 0;
unsigned long startMillis;
int current_hour = 0;      //Enter current hour (24(HR)) i.e. 0(12 am)
int current_min = 13;    //Enter current mins (MM) i.e. 13
int current_date = 9;     //Enter current date (DD) i.e. 9(th day of month)
int input_hrs = 5;    //Enter the time you want the alarm to ring (HH) i.e. 5(5am)
int input_mins = 00;  //Enter the time you want the alarm to ring (MM) i.e. 00
int input_date = 9;   //Enter the time you want the alarm to ring (DD) i.e. 9
int final_hrs, final_mins, final_date;
int final_time;
void setup() {
  pinMode(Switch, INPUT_PULLUP);  // Enables internal pull-up resistor
  pinMode(BuzzPin, OUTPUT); 
  pinMode(ObsAV, INPUT);
  pinMode(LDR, INPUT);
  IrReceiver.begin(IR_RECEIVE_PIN);

  for (int i = 0; i < 3; i++) {
    pinMode(Led[i], OUTPUT); //Gives output to Led Digital pins in for loop
  }
  for (int i = 0; i < 3; i++) {
    pinMode(LED[i], OUTPUT); //Gives output to LED pwm pins in for loop
  }
  Serial.begin(9600);
  startMillis = millis(); //starts timer

  final_date = (input_date - current_date);  // ensuring it's above before operations on it
  final_hrs = (input_hrs - current_hour);
  if (final_hrs < 0) {
    final_hrs += 24;  // took 1 day
    final_date--;     // gave one day
  }
  final_mins = (input_mins - current_min);
  if (final_mins < 0) {
    final_mins += 60;  // took 1 hour
    final_hrs--;       // gave 1 hour
  }
  final_time = ((final_date * 24 * 60) + (final_hrs * 60) + final_mins); //simple time calculator in mins
}
void loop() {


  unsigned long currentMillis;
  currentMillis = millis();//records time since loop is running 
  int escapedTime = (currentMillis - startMillis) / 60000;// total time past since it executed
  if ((escapedTime >= final_time) && !logic) //checks if the program's execution time has reached the target time
  {

    if (stoploop) {
      return; // if stop loop == true then whole void loop ends here
    }

    for (int i = 0; i < 3; i++) {
      digitalWrite(Led[i], HIGH); //Powers up the Leds in Digital pins
    }
    for (int i = 0; i < 3; i++) {
      analogWrite(LED[i], 255); //Powers up the Leds in Pwm pins
    }
    digitalWrite(BuzzPin, HIGH);// Powers
    if (ifChecker <= 2) //Number of snooze checker
    {
      if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.decodedRawData != 0) {
          ifChecker++; //Snooze count +1 per signal received 
          digitalWrite(BuzzPin, LOW); // power output for buzzer is off
          for (int i = 0; i < 3; i++) {
            digitalWrite(Led[i], LOW);
          }
          for (int i = 0; i < 3; i++) {
            analogWrite(LED[i], 0);
          }

          delay(600000); //10 mins delay 
        }
        IrReceiver.resume(); //resumes the search for ir signals till received(program would work regradless of this)
      }
    }
    int reading = digitalRead(Switch); //readings from switch (0 and 1 or LOW and HIGH)
    if (reading != lastSwitchState) {
      lastDebounceTime = millis(); //resets the time when btn is pressed(IF READING IS DIFFERENT FROM LAST TIME(LAST ITERATION))
    }
    if ((millis() - lastDebounceTime) > debounceDelay)// if the time passed(btn pressed - since last signal(last signal is 0)) i greater than default delay
    // basically filtering the noise or checking stablity of btn's input
    {
      if (reading != SwitchState) {
        SwitchState = reading;  //if reading isn't equal to SwitchState from last loop(assuming default) so it would update SwitchState as it receives High Input from Switch to Low(basically high just reverse)

        if (SwitchState == LOW && digitalRead(ObsAV) == LOW) // if Switch is pressed SwitchState would be LOW(opposite of input but works same) 
        // if both Obstacle avoidance sensor and Switch detects input 
        {
          digitalWrite(BuzzPin, LOW); //Powers off the Buzzer
          first_condition = true; //To make sure that 2nd part of loop works
        }
        if (SwitchState == LOW || digitalRead(ObsAV) == LOW)
        // if SwitchState detects input or Obstacle Avoidance Sensor detects input 
        {
          do {
            digitalWrite(BuzzPin, HIGH);

            for (int i = 0; i < 3; i++) {
              digitalWrite(Led[i], HIGH);
            }
            for (int i = 0; i < 3; i++) {
              analogWrite(LED[i], 255);
            }
            delay(150);
            for (int i = 0; i < 3; i++) {
              digitalWrite(Led[i], LOW);
            }
            for (int i = 0; i < 3; i++) {
              analogWrite(LED[i], 0);
            }
            delay(150);
            //basically blinking leds and buzzer goin high
          } while ((digitalRead(ObsAV) == LOW && SwitchState == HIGH) || (digitalRead(ObsAV) == HIGH && SwitchState == LOW)); // while both don't detects input at same time
          digitalWrite(BuzzPin, LOW);
          first_condition = true; // making sure 2nd part runs
        } else {
          for (int i = 0; i < 3; i++) {
            digitalWrite(Led[i], LOW);
          }
          for (int i = 0; i < 3; i++) {
            analogWrite(LED[i], 0);
          }
        }
      }
    }
    lastSwitchState = reading; //updating LastSwitchState so it could filter out and detects new reading
    // }
    if (first_condition == true) {
      unsigned long startTime = millis();
      int sum = 0;
      int numReadings = 1000; //number of inputs from LDR(Light Dependent resistor) in a loop, basically number of samples
      for (int i = 0; i < numReadings; i++)
      //Would run the loop until i is equal to numReadings/samples 
      {
        sum += analogRead(LDR);// Sum of readings from LDR from 1st time to last time
      }
      int LDRvalue = sum / numReadings;// avg of sum per samples
      while (LDRvalue != 0) 
      //if LDR detects light it would be above than 0(depends on LDR)
      //if ur LDR sensor works opposite just make it (LDRvalue == 0)
    
      {
        digitalWrite(BuzzPin, HIGH);
        for (int i = 0; i < 3; i++) {
          analogWrite(LED[i], 255);
        }
        delay(1000); // delay of 1sec btwn led blinking
        for (int i = 0; i < 3; i++) {
          digitalWrite(Led[i], HIGH);
        }
        for (int i = 0; i < 3; i++) {
          analogWrite(LED[i], 0);
        }
        digitalWrite(BuzzPin, LOW);
        for (int i = 0; i < 3; i++) {
          digitalWrite(Led[i], LOW);
        }
        delay(1000);
        sum = 0;
        for (int i = 0; i < numReadings; i++) {
          sum += analogRead(LDR);
          delay(0);
        }
        delay(150);
        LDRvalue = sum / numReadings;
        // gonna check LDRvalue and keep buzzer and leds blinking till LDR receives Value different than 0
      }
      digitalWrite(BuzzPin, LOW); // it'll work when ldr value would be diff than 0 and it'll get outta loop so it'll stop powering buzzerPin 
      stoploop = true; // loop gonna stop and program ends here
      //hope you like it ;))
    }
  }
}
