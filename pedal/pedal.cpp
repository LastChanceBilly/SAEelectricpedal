/*
	Pedal.cpp : a library to control the pedal for an electric car
	maps linear pot to rotary pot during calibration, then run checks to see if map fails
	
	rotary pot: clarostat hrs100swab090
	linear pot: ???
	
	Chris Davies, sept 2018
*/

#include "Arduino.h"
#include "pedal.h"

Pedal::Pedal(int rotaryPin, int linearPin){
	rot = rotaryPin;
	lin = linearPin;
	for(unsigned int i = 0; i < 256; i++) potVal[i] = 0;
}

inline bool Pedal::check(byte rotVal, byte linVal){
	if(potVal[rotVal] < linVal+err && potVal[rotVal] > linVal-err)
		return true;
	return false;
	
}

void Pedal::calibrate(int interruptPin){
	byte currVal;
	while(!digitalRead(interruptPin)){		//fix this! digitalRead is vv inefficient!!
		currVal = analogRead(rot)>>2;
		potVal[currVal] = analogRead(lin)>>2;
		if(mini > currVal) mini = currVal;
		else if(maxi < currVal) maxi = currVal;
	}
}

byte Pedal::read(){
	byte currVal = analogRead(rot)>>2;
	if(check(currVal, (byte)analogRead(lin)>>2))
		return map(currVal, mini, maxi, 0, 255);
	//error handling
}
