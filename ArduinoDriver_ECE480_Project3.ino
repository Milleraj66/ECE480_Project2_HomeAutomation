
/*
  FileName: ArduinoDriverProject3.ino
  Author: Arthur J. Miller & Dillon Smolinski
  Date:   03-01-2016
  Purpose: Winter 2016, Cal Poly Pomona, ECE 480, Project 3
  Outline: 
    1. Program will wait for user sound input (clap)
    2. After a clap is heard the user has INTERVAL amount of time to clap 1,2, or 3 times
    3. Each clap will turn on an indicator light allowing user to know what option is being chosen
    4. Depending on the number of claps different signals will be sent
      a. 1 clap: signal led 1 (toggle on or off)
      b. 2 clap: signal led 2 (toggle on or off)
      c. 3 clap: send seriel signal "Weather" over usb
        * a python program will be waiting for this signal, and will pull weather data file from 
          weather underground and display on gui
  Hardware: 
    1. Adafruit Electret Microphone Amplifier with Adjustable Gain
    2. Arduion Uno
    3. 5 leds
    4. 5 330ohm resistors   
*/

// The indicator leds will turn on in series and stay on for each INTERVAL
const int indicator1=11; //Indicates one clap on pin11
const int indicator2=12; //Indicates two clap on pin12
const int indicator3=13; //Indicates three clap on pin13
// These leds will represent signals to various applications
const int signal1=2; //Indicates one clap on pin11
const int signal2=3; //Indicates two clap on pin12
// Setup for amount of time we will listen for user input claps
unsigned long timer; // the timer
unsigned long INTERVAL = 3000; // the repeat interval
// Other variables
int sample; //the variable that will hold the value read from the microphone each time
int claps; // # of claps in INTERVAL of time -> determines action signal
const int threshold= 600;//the microphone threshold sound level at which the LED will turn on
const int microphonePin= 0; //the microphone positive terminal will connect to analog pin A0 to be read


void setup() {
  claps = 0; // Intialize # claps to zero
  timer = millis(); // start timer
  pinMode (indicator1, OUTPUT);//sets digital pin 13 as output
  pinMode (indicator2, OUTPUT);//sets digital pin 13 as output
  pinMode (indicator3, OUTPUT);//sets digital pin 13 as output
  pinMode (signal1, OUTPUT);//sets digital pin 13 as output
  pinMode (signal2, OUTPUT);//sets digital pin 13 as output
  Serial.begin(9600); //sets the baud rate at 9600 so we can check the values the microphone is obtaining on the Serial Monitor
}//End setup()

void loop(){
  sample= analogRead(microphonePin); //the arduino takes continuous readings from the microphone 
  //**** Send seriel signal and reset counter every INTERVAL # of mili seconds
    if ( (claps!=0) && ((millis()-timer) > INTERVAL) ) {
    // timed out
    switch(claps){
      // send signals depending on # of claps
      case 1:
        digitalWrite (signal1, !digitalRead(signal1)); // Toggle signal1
        break;
      case 2:
        digitalWrite (signal2, !digitalRead(signal2)); // Toggle signal2
        break;
      case 3:
        Serial.println("Weather");  // prints hello with ending line break
        break;
      default:
        break;
    }//END switch-case
    //**** Reset for next listen INTERVAL
    // Reset indicator leds
    digitalWrite (indicator1, LOW);
    digitalWrite (indicator2, LOW); 
    digitalWrite (indicator3, LOW);  
    // Reset timer logic
    timer += INTERVAL;// reset timer by moving it along to the next interval 
    claps = 0; // reset counter of clap times
   }//END if()
  
  //**** Listen for clap -> {turn Led on and incriment counter if clap}
  if (sample > threshold){
    claps++;  
    switch(claps){
      case 1:
        digitalWrite (indicator1, HIGH); //if the reading is greater than the threshold value, LED turns on
        timer = millis(); // keeps timer synced
        break;
      case 2:
        digitalWrite (indicator2, HIGH); //if the reading is greater than the threshold value, LED turns on
        break;
      case 3:
        digitalWrite (indicator3, HIGH); //if the reading is greater than the threshold value, LED turns on
        break;
      default:
        claps = 0;
        digitalWrite (indicator1, LOW);
        digitalWrite (indicator2, LOW); 
        digitalWrite (indicator3, LOW);  
        break; 
    }//END switch-case
    delay(200); // Wont work without slight delay between claps
   }//End if()

}//End loop()


