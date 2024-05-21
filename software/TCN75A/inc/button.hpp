#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstdint>
#include <iostream>
#include "TempSensor.hpp"

class button{
    public:
        button(uint8_t btn, TempSensor& temp_sensor); //constructor
        void generateIRQ(uint8_t btn); // Enable IRQ
        
        //debouncing variables
        static bool state; 
        static unsigned long time;
        static const int delayTime;
        
        static uint8_t buttonNum; //The number of the button pressed

        //Setup gpio callback function
        static void gpio_callback(uint gpio, uint32_t events);

    private:
        const uint8_t BTN_PIN; //button gpio pin
        TempSensor& sensor; // Reference to TempSensor object
        static TempSensor* pSensor;
        const static int startingPin; //first button pin used
};

#endif