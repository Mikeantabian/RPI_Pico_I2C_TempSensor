#include "../inc/button.hpp"
#include "hardware/gpio.h"
#include "inc/TempSensor.hpp"
#include "pico/time.h"
#include <cstdint>



bool button::state = true;
unsigned long button::time = 0;
const int button::delayTime = 250;
uint8_t button::buttonNum = 0;

//This is the first gpio pin used for buttons. The rest follow one by one after this one
const int button::startingPin = 2;

// Define the static member variable to hold the reference to the TempSensor object
TempSensor* button::pSensor = nullptr;

/**
 * @brief Button Constructor
 *
 * Constructor initializes button objects and variables
 *
 * @param pin the button gpio pin number
 * @param temp_sensor A reference to a TempSensor object; used in the callback function
 *
 */
button::button(uint8_t pin, TempSensor& temp_sensor) : BTN_PIN(pin), sensor(temp_sensor) {
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);
    generateIRQ(BTN_PIN);
    pSensor = &sensor; // Set the reference to TempSensor object
}

/**
 * @brief Enabled the Interrupt Request for buttons
 *
 * This function simply enabled the interrupt request for
 * the all the button objects created.
 *
 * @param btn the button gpio pin number
 *
 * @return void.
 */
void button::generateIRQ(uint8_t btn){
    //Only one of them needs to be enabled with callback
    if(btn == startingPin){
        gpio_set_irq_enabled_with_callback(btn, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    } else {
        gpio_set_irq_enabled(btn, GPIO_IRQ_EDGE_RISE , true);
    }  
}


/**
 * @brief Interrupt Handler/ISR.
 *
 * This function has debouncing implemented for efficiency.
 * If the Alert sends the interrupt, it will accordingly set the 
 * Alert LED on or off depending on the temperature.
 * If the fifth button is pressed, it will manually turn off the Alert.
 *
 * @param gpio the pin number of the gpio
 * @param events the event(s) that triggered the interrupt(s)
 *
 * @return void.
 */
void button::gpio_callback(unsigned int gpio, uint32_t events){
    if(gpio == pSensor->Alert_pin && TempSensor::pulseCount == 0){
        TempSensor::AlertState = true;
        TempSensor::pulseCount = 1;
    } else if(gpio == pSensor->Alert_pin && TempSensor::pulseCount == 1){
        TempSensor::AlertState = false;
        TempSensor::pulseCount = 0;
    } else {
        if(pSensor){
            unsigned long currentTime = to_ms_since_boot(get_absolute_time());
            if ((currentTime - time) > delayTime) {
                time = currentTime;
                state = !state;

                //perform action after debouncing
                buttonNum = gpio - startingPin;
                uint8_t btnNum = gpio - 2;

                //button 5 turns off alert
                if(buttonNum == 5){
                    TempSensor::AlertState = false;
                }

                pSensor->processMainMenu(buttonNum + '0');
            } else {
                
            }
        }  
    }

}
