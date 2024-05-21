#include "../inc/led.hpp"

/**
 * @brief LED Constructor
 *
 * Constructor initializes LED objects and variables
 *
 * @param pin the LED gpio pin number
 *
 */
LED::LED(uint8_t pin): LED_PIN(pin) {
    //stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}


/**
 * @brief Change LED state
 *
 * Will turn LED ON/OFF depending on param
 *
 * @param led_state LED state to change to
 *
 * @return void.
 */
void LED::changeState(uint8_t led_state){
    gpio_put(LED_PIN,led_state); 
}

/**
 * @brief Blink the LED
 *
 * Will toggle LED ON and OFF once
 *
 * @return void.
 */
void LED::blinkLED(){
    changeState(1); //turn LED on
    sleep_ms(LED_SLEEP_TIME);
    changeState(0); //turn LED off
    sleep_ms(LED_SLEEP_TIME);
}