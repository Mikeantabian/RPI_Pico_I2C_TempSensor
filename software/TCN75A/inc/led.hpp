#ifndef LED_HPP
#define LED_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"

class LED{
    public:
        LED(uint8_t pin); //Constructor
        void changeState(uint8_t led_state); //change LED state
        void blinkLED(); //Blink LED once
    private:
        uint8_t LED_PIN; // LED gpio pin number
        const int LED_SLEEP_TIME = 250; //delay time between blink
};

#endif