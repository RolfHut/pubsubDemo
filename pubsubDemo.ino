/*
Demo code to show pub-sub system
*/

const int analogPin = A0;
const int digitalPin = D1;
const int LEDPin = D7;

String subScribeTopic = "JoyOfCoding/react";
String publishTopic = "JoyOfCoding/shoutshout";

int threshold = 2024;
bool aboveThreshold = false;
int LEDstatus = HIGH;
int sensorValue = 0;

Timer measurementTimer(10000,uploadMeasurement);

void setup() {
    //Declare what each pin is doing
    pinMode(analogPin,INPUT);
    pinMode(digitalPin,OUTPUT);
    pinMode(LEDPin,OUTPUT);
    
    //subscribe to topic
    Particle.subscribe(subScribeTopic,switchLED);
    
    //make variable sensorValue available in the cloud
    Particle.variable("measurement",sensorValue);
    
    //make function callable from the cloud
    Particle.function("alarm",ringAlarm);
    
    //start the timer that calls "uploadMeasurement every 10 seconds"
    measurementTimer.start();

}

void loop() {
    //do measurement
    sensorValue = analogRead(analogPin);
    //wait 10 milliseconds, because why not.
    delay(10);
}

void switchLED(const char *event, const char *data){
    //switch the LEDstatus
    if (LEDstatus == HIGH) {
        LEDstatus = LOW;
    } else {
        LEDstatus = HIGH;
    }
    //and also tell it to the LED itself
    digitalWrite(LEDPin,LEDstatus);
    
}

void uploadMeasurement(){
    Particle.publish(publishTopic,(String) sensorValue);
}

int ringAlarm(String command){
    if ((command.toInt() > 0) & (command.toInt() < 10)){
        digitalWrite(digitalPin,HIGH);
        delay(command.toInt() * 1000);
        digitalWrite(digitalPin,LOW);
        return 1;
    } else {
        return -1;
    }
}
