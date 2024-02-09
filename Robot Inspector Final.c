// IDENTIFY ENABLE PINS
//Arduino
const uint16_t en_A = 4;
const uint16_t en_B = 7;
//Motor A
const uint16_t forwardDrivePinA  = 6;  // Pin 2 of L293
const uint16_t reverseDrivePinA  = 9;  // Pin 7 of L293
//Motor B 
const uint16_t forwardDrivePinB  = 3;  // Pin 10 of L293
const uint16_t reverseDrivePinB  = 5; // Pin 15 of L293

// sample rate in ms
const uint16_t sample_rate = 100;
static uint16_t duty_cycle = 255;

//declare integers
int forward = 0;
int reverse = 0;

//IR Remote
#include <IRremote.hpp>
int RECV_PIN = 10;

//void set-up is to perform any setup required before program
//This will run only one time.
void setup(){
    //CONFIGURE PINS AS OUTPUT OR INPUT TO ARDUINO
    //set output pins to L293
    pinMode(en_A, OUTPUT);
    pinMode(en_B, OUTPUT);
  	//set input pin from IR Remote
  	pinMode(RECV_PIN, INPUT);
    //Set ~ pins as outputs to L293
    //MOTOR CONTROL
    //Motor A: motorpin1, motorpin2
    //Motor B: motorpin3, motorpin4
    pinMode(forwardDrivePinA, OUTPUT);
    pinMode(reverseDrivePinA, OUTPUT);
    pinMode(forwardDrivePinB, OUTPUT);
    pinMode(reverseDrivePinB, OUTPUT);
 
    //To establish communication between Arduino and computer
  	Serial.begin(9600);
  
    //Bypass buffer as EN is always 1 (HIGH)
    digitalWrite(en_A, HIGH);
    digitalWrite(en_B, HIGH);
 	
  	//IR
  	Serial.println("Enabling IRin");
  	IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  	Serial.println("Enabled IRin");
  }

void set_duty(uint16_t percent) {
  	// Translate duty cycle to PWM analog value
  	duty_cycle = static_cast<uint16_t>(255 * percent/100);
	//Serial.print("Duty: ");
  	//Serial.println(duty_cycle);
}

void drive_forward() {
    analogWrite(forwardDrivePinA, duty_cycle);
  	analogWrite(forwardDrivePinB, duty_cycle);
}

void drive_reverse() {
    analogWrite(reverseDrivePinA, duty_cycle);
  	analogWrite(reverseDrivePinB, duty_cycle);
}

void stop() {
 	digitalWrite(en_A, LOW);
    digitalWrite(en_B, LOW);
}

//This loop will run continuously
void loop(){
    if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.decodedRawData == 4177968896 && forward == LOW){
      forward = HIGH;
    } else if (IrReceiver.decodedIRData.decodedRawData == 4177968896 && forward == HIGH){
      forward = LOW;
    } else if (IrReceiver.decodedIRData.decodedRawData == 4211392256 && reverse == LOW){
      reverse = HIGH;
    } else if (IrReceiver.decodedIRData.decodedRawData == 4211392256 && reverse == HIGH){
      reverse = LOW;
    }
  	//Print to Serial Monitor
  	Serial.println(IrReceiver.decodedIRData.decodedRawData);
  	IrReceiver.resume();
 	}
  
  	// debounce
  	delay(sample_rate);
  
  	// event handler
    if (forward == HIGH) {
      	set_duty(100);
    	drive_forward();
    } else if (reverse == HIGH) {
      	set_duty(100);
     	drive_reverse(); 
    } else {
     	// set motor speed to 0
     	set_duty(0);
     	analogWrite(reverseDrivePinA, duty_cycle);
     	analogWrite(forwardDrivePinA, duty_cycle);
     	analogWrite(reverseDrivePinB, duty_cycle);
     	analogWrite(forwardDrivePinB, duty_cycle);
    }
  	
}

