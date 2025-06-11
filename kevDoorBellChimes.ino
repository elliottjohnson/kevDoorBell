// Kevin's doorbell

// Copyright 2025 Elliott Johnson <elliott@elliottjohnson.net>
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this 
//   software and associated documentation files (the "Software"), to deal in the Software 
//   without restriction, including without limitation the rights to use, copy, modify, 
//   merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
//   permit persons to whom the Software is furnished to do so, subject to the following 
//   conditions:
//
//    The above copyright notice and this permission notice shall be included in all 
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
//    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Here are some configurable items
const int quarterNoteDuration = 1000; // The length in ms for a quarternote. 1000ms = 60 bpm.
const int halfNoteDuration = quarterNoteDuration * 2; // The length in ms for a half note.
const int noteTriggerDuration = 10; // How long the solinoid engages in ms.
const int doorBellReadDelay = 125; // The number of ms the doorbell needs to be held for a ring.
const int loopDelay = 100; // How many ms we wait before checking for another doorBell ring.

// Define some top level stuff. 
const int doorBellPin = 2; // The input pin to receive 5v from the door bell switch.
const int doorBellClosedState = LOW; // A value to represent a closed state.
const int doorBellOpenState = HIGH;  // A value to represent an open state.
int doorBellState = doorBellOpenState;  // A variable to capture the current doorbell state. 

 // I chose the following pins because they are non-PWM capable.  Reserving PWM pins for other uses.
const int gSharpPin = 7; // The output pin to trigger a G#
const int fSharpPin = 8; // The output pin to trigger a F#
const int eNaturalPin = 12; // The output pin to trigger a E
const int bNaturalPin = 13; // The output pint to trigger a B
const int chimeStrikeState = HIGH; // The state to output when striking the chime.
const int chimeRelaxState = LOW; // The state to output when not striking the chime.

// Setup is called when the arduino starts up.
void setup() {
  pinMode( doorBellPin, INPUT_PULLUP ); // setup the door bell pin w/ internal pull up resistor.  
  pinMode( gSharpPin, OUTPUT ); // setup the pin to output G#
  pinMode( fSharpPin, OUTPUT ); // setup the pin to output F#
  pinMode( eNaturalPin, OUTPUT ); // setup the pin to output E
  pinMode( bNaturalPin, OUTPUT ); // setup the pin to output B
  digitalWrite( gSharpPin, chimeRelaxState ); // silence everything
  digitalWrite( fSharpPin, chimeRelaxState ); 
  digitalWrite( eNaturalPin, chimeRelaxState );
  digitalWrite( bNaturalPin, chimeRelaxState );
}

// Once setup() finishes, the loop begins.
void loop() {
  checkDoorBell(); // Checks the door bell to see if it's pressed.
  delay( loopDelay ); // Wait some time before checking again.. Not sure if this is good or not.
}

// Checks the doorbell state and rings the bell if rung, otherwise it just returns to loop().
void checkDoorBell() {
  readDoorBellState(); // Read the current state of the door bell momentary switch.

  if ( doorBellState == doorBellClosedState ) { // If the doorBell Switch closes
    delay( doorBellReadDelay ); // wait for a bit to see if the switch is still held.
    readDoorBellState(); // re-read the door bell state.
    // Test to see if are still being held.
    if ( doorBellState == doorBellClosedState ) { // If its still closed then ring dem bells.
      ringWestminsterChimes(); // Currently only Westminster Chimes.
    }
  }
  doorBellState = doorBellOpenState; // reset the doorBellState before we exit.
}

// Reads the state of the doorbell button and sets the global variable doorBellState from the GPIO doorBellPin.
void readDoorBellState () {
  doorBellState = doorBellOpenState; // Reset the doorbell state, which in a pull down switch is HIGH
  doorBellState = digitalRead( doorBellPin ); // Read in the current state.
}

// Rings each one of the strings with the correct duration.
void ringWestminsterChimes () {
  ringChime( gSharpPin, quarterNoteDuration );     // G# 1/4 note
  ringChime( eNaturalPin, quarterNoteDuration );   // E  1/4 note
  ringChime( fSharpPin, quarterNoteDuration );     // F# 1/4 note
  ringChime( bNaturalPin, halfNoteDuration );      // B  1/2 note
  ringChime( bNaturalPin, quarterNoteDuration );   // B  1/4 note
  ringChime( fSharpPin, quarterNoteDuration );     // F# 1/4 note
  ringChime( gSharpPin, quarterNoteDuration );     // G# 1/4 note
  ringChime( eNaturalPin, halfNoteDuration );      // E  1/2 note
}

// Does the "heavy lifting" of pinging a solenoid for duration.
void ringChime( int pin, int duration ) {
  digitalWrite( pin, chimeStrikeState );      // Hit the solinoid
  delay( noteTriggerDuration );               // Wait some amount of time
  digitalWrite( pin, chimeRelaxState );       // Release the solinoid.
  // Wait the note duration before returning.  This lets the note ring out.
  if ( duration > noteTriggerDuration ) {
    delay( duration - noteTriggerDuration );
  } else {
    delay( duration );
  }
} 
