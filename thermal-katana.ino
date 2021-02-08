#include "FastLED.h"

#define NUM_LEDS 144
#define DATA_PIN 5

#define BUTTON_PIN 4
#define DEBOUNCE_DELAY 50 

// The color of the blade
CRGB color = CRGB(255, 48, 0);

int areLedsOn = LOW;    // the current state of the LEDs

// Variables for button debouncing
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

CRGB leds[NUM_LEDS];

void onAnimation() {
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = color;
  }

  FastLED.show();
}

void offAnimation() {
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = CRGB::Black;
  }

  FastLED.show();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // Startup animation
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
      leds[dot] = color;
      FastLED.show();
      leds[dot] = CRGB::Black;
      delay(2);
  }

  // Turn all LEDs off
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = CRGB::Black;
  }

  FastLED.show();
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LEDs if the new button state is HIGH
      if (buttonState == HIGH) {
        if (areLedsOn) {
          offAnimation();
          areLedsOn = LOW;
        } else {
          onAnimation();
          areLedsOn = HIGH;
        }
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
