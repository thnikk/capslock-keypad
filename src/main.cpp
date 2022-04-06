#include <Arduino.h>
#include <Bounce2.h>
#include <HID-Project.h>
#include <FastLED.h>

Bounce * bounce = new Bounce[3];
CRGBArray<3> leds;

bool pressed[3];
uint8_t pins[] = {2, 3, 1};

void setup() {

    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, 0>(leds, 2);

    for (uint8_t x=0; x<3; x++) {
        pinMode(pins[x], INPUT_PULLUP);
        bounce[x].attach(pins[x]);
        bounce[x].interval(8);
    }

    BootKeyboard.begin();

}

void loop() {


    Serial.print(".");
    for (uint8_t x=0; x<3; x++) {

        if (BootKeyboard.getLeds() & LED_CAPS_LOCK) {
            leds[x] = 0x00FF00;
        } else {
            leds[x] = 0xFF0000;
        }

        bounce[x].update();
        if (bounce[x].read() != pressed[x]) {
            if (!bounce[x].read()) {
                Serial.print("Key ");
                Serial.print(x);
                Serial.println(" pressed.");

                switch(x) {
                    case 0:
                        BootKeyboard.press(KEY_LEFT_CTRL);
                        BootKeyboard.press(KEY_V);
                        break;
                    case 1:
                        BootKeyboard.press(HID_KEYBOARD_CAPS_LOCK);
                        break;
                    case 2:
                        BootKeyboard.press(KEY_LEFT_CTRL);
                        BootKeyboard.press(KEY_C);
                        break;
                }

            } else if (bounce[x].read()) {
                Serial.print("Key ");
                Serial.print(x);
                Serial.print(" released.");

                switch(x) {
                    case 0:
                        BootKeyboard.release(KEY_LEFT_CTRL);
                        BootKeyboard.release(KEY_V);
                        break;
                    case 1:
                        BootKeyboard.release(HID_KEYBOARD_CAPS_LOCK);
                        break;
                    case 2:
                        BootKeyboard.release(KEY_LEFT_CTRL);
                        BootKeyboard.release(KEY_C);
                        break;
                }

            }
            pressed[x] = bounce[x].read();
        }

    }
    FastLED.show();

}
