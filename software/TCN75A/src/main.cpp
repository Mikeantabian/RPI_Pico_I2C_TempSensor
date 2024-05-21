#include "../inc/TempSensor.hpp"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "../inc/button.hpp"
#include "../inc/led.hpp"
#include "pico/multicore.h"
#include <cstdint>

/**
 * @brief Pico Second Core
 *
 * This function sets what the second Pico board core should do.
 * In this case, it is constantly verifying the state of the
 * Alert pin and changes the Alert LED accordingly.
 *
 * @return void
 */
void core1(){
    //create an LED object for the alert
    LED alertLED(17);
    
    while(true){
        //Blink Red Alert LED if the alert is ON
        if(TempSensor::AlertState){
            alertLED.changeState(1);
        } else if (!TempSensor::AlertState) {
            alertLED.changeState(0);
        }
    }
}


int main(){
    stdio_init_all();
    
    //Set up the TempSensor object
    //Constructor Arguments: 
    //TempSensor(i2c_inst_t *i2c, int sda, int scl, int freq,int redLED, int greenLED)
    TempSensor TCN(i2c1, 14, 15, 400 * 1000, 17, 16, 0);
    
    // Initialize buttons
    button buttons[] = {button(2, TCN), button(3, TCN), button(4, TCN), button(5, TCN), button(6, TCN), button(7, TCN)};

    multicore_launch_core1(core1);

    while(true){
        TCN.MainMenu();
        sleep_ms(700);

    }
    return 0;
}