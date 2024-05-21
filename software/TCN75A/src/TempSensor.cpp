#include "../inc/TempSensor.hpp"
#include "hardware/i2c.h"
#include <cstdint>
#include <cstdio>
#include <string>


//4 different options for the Register pointers
const uint8_t TEMP_REG = 0x00;
const uint8_t CONFIG_REG = 0x01;
const uint8_t HYST_TEMP_REG = 0x02;
const uint8_t SET_TEMP_REG = 0x03;

//Gpio pin used for the alert
bool TempSensor::AlertState = false;
int TempSensor::pulseCount = 0;


/**
 * @brief TempSensor Constructor
 *
 * Constructor initializes TempSensor objects and variables.
 *
 * @param i2c the i2c instance to use since RPI Pico has i2c0 and i2c1
 * @param sda Serial Data Line gpio pin number
 * @param scl Serial Clock Line gpio pin number
 * @param freq baudrate of the I2C
 * @param redLED the gpio pin number of the red LED
 * @param greenLED the gpio pin number of the green LED
 * @param alert the gpio pin number of the alert
 *
 */
TempSensor::TempSensor(i2c_inst_t *i2c, int sda, int scl, int freq,int redLED, int greenLED, int alert): 
I2C_PIN(i2c), SDA_PIN(sda), SCL_PIN(scl), BAUD_RATE(freq), red_led(redLED), green_led(greenLED), Alert_pin(alert){
    stdio_init_all();
    proj_init();
    initialiseAlert();
    sensor_addr = bus_scan();
}

/**
 * @brief Initialize I2C pins
 *
 * This function simply initializes the pins used
 * by the I2C by setting their function and adding pull up resistors.
 *
 * @return void.
 */
void TempSensor::proj_init(){
    i2c_init(I2C_PIN, BAUD_RATE);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

/**
 * @brief Initialize Alert pin
 *
 * This function simply initializes the Alert pin
 * by choosing the direction, enabling the interrupt and adding pull up a resistor.
 *
 * @return void
 */
void TempSensor::initialiseAlert(){
    //Initialize alert gpio: direction: input, pull up resistor used
    gpio_init(Alert_pin);
    gpio_set_dir(Alert_pin, GPIO_IN);
    gpio_pull_up(Alert_pin);
    gpio_set_irq_enabled(Alert_pin, GPIO_IRQ_EDGE_FALL, true);
}

/**
 * @brief Reserved Addresses
 *
 * This function reserves addreses from being used.
 *
 * @param addr address to verify if reserved
 *
 * @return bool
 */
bool TempSensor::reserved_address(uint8_t addr){
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}


/**
 * @brief Bus Scan
 *
 * This function prints all the availables addresses the sensor
 * bus has and also the addresses being used are shown with an @.
 *
 * @return uint8_t real_addr: the sensor address
 */
uint8_t TempSensor::bus_scan(){
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    uint8_t real_addr;

    //iterate through all I2C addresses from 0x00 to 0x7F reaching 128
    for(int addr = 0; addr < (1 << 7); addr++){
        //if the address is a multiple of 16 then its the end of the row
        if( addr % 16 == 0){
            printf("%02x", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.
 
        // Skip over any reserved addresses.
        int ret; // will retain the result
        uint8_t rxdata; //receiving buffer location
        if(reserved_address(addr)){
            ret = PICO_ERROR_GENERIC;
        } else {
            ret = i2c_read_blocking(I2C_PIN, addr, &rxdata, 1, false);
        }
        printf(ret < 0 ? "." : "@"); // print . if address not found and @ if found
        printf(addr % 16 == 15 ? "\n" : "  "); // newline if end or row or space if not

        if(ret == 1){
            real_addr = addr; // take the I2C address
        }

    }
    return real_addr;
}

/**
 * @brief Modify Sensor Address
 *
 * Makes sure address was changed by reading the sensor.
 * If address was properly changed on hardware, sets address variable accordingly.
 * Blinks green led if worked, red if it didn't.
 *
 * @param address sensor address to be checked if it was changed
 *
 * @return void
 */
void TempSensor::Modify_DeviceID(int address){
    int ret; // will retain the result
    uint8_t rxdata; //receiving buffer location
    ret = i2c_read_blocking(I2C_PIN, address, &rxdata, 1, false);

    if(ret == 1){
        sensor_addr = address; // take the I2C address
        std::cout << "[ID WAS SUCCESSFULLY CHANGED]" << std::endl;
        for(int i = 0; i < 4; i++){
            green_led.blinkLED();
        }
    } else{
        std::cout << "[ID WAS NOT CHANGED, PLEASE TRY AGAIN]" << std::endl;
        for(int i = 0; i < 4; i++){
            red_led.blinkLED();
        }
    }
}

//******************************************************//
//************REGISTER SETTINGS FUNCTIONS***************//
//******************************************************//

/**
 * This is taken from the professor's project
 * @brief Reads data from a register over I2C.
 *
 * This function reads data from a register over I2C. The register address is
 * sent to the specified I2C address, and the resulting data is read into the
 * provided buffer. The function returns the number of bytes read.
 *
 * @param i2c_inst A pointer to the I2C instance to use for the read.
 * @param addr The I2C address to read from.
 * @param reg The register address to read from.
 * @param buf A pointer to the buffer to store the read data.
 * @param nbytes The number of bytes to read from the register.
 *
 * @return The number of bytes read from the register.
 */
int TempSensor::Read_Reg(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
                            uint8_t *buf, const uint8_t nbytes){
    int num_bytes_read = 0;

    // Check to make sure caller is asking for 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Read data from register(s) over I2C
    i2c_write_blocking(I2C_PIN, addr, &reg, 1, true);
    num_bytes_read = i2c_read_blocking(I2C_PIN, addr, buf, nbytes, false);

    return num_bytes_read;
}

/**
 * This is taken from the professor's project
 * @brief Writes data to a register over I2C.
 *
 * This function writes data to a register over I2C. The register address is
 * appended to the front of the data packet, and the resulting message is sent
 * to the specified I2C address. The function returns the number of bytes
 * written.
 *
 * @param i2c_inst A pointer to the I2C instance to use for the write.
 * @param addr The I2C address to write to.
 * @param reg The register address to write to.
 * @param buf A pointer to the buffer containing the data to write.
 * @param nbytes The number of bytes to write to the register.
 *
 * @return The number of bytes written to the register.
 */
int TempSensor::Write_Reg(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
                uint8_t *buf, const uint8_t nbytes){
                 
    int num_bytes_read = 0;
    uint8_t msg[nbytes + 1];

    // Check to make sure caller is sending 1 or more bytes
    if (nbytes < 1) {
        return 0;
    }

    // Append register address to front of data packet
    msg[0] = reg;
    for (int i = 0; i < nbytes; i++) {
        msg[i + 1] = buf[i];
    }

    // Write data to register(s) over I2C
    num_bytes_read = i2c_write_blocking(I2C_PIN, addr, msg, (nbytes + 1), false);

    return num_bytes_read;
}

//******************************************************//
//***********TEMPERATURE SETTINGS FUNCTIONS*************//
//******************************************************//

/**
 * @brief Read Temp Register
 *
 * Reads from the temperature register from the sensor.
 * This function retreives the raw temperature data
 * before any conversion and saves it as two bytes.
 *
 * @return void
 */
void TempSensor::Raw_Temp_Read(){
    uint8_t buf[2];
    Read_Reg(I2C_PIN, sensor_addr,TEMP_REG, buf, 2);
    // // Combine two bytes into a 16-bit unsigned integer
    raw_temperature = (buf[0] << 8) | buf[1];
    
    integerPart  = buf[0];
    decimalPart = buf[1];
}

/**
 * @brief Convert Raw data to Float
 *
 * Takes the raw temperature value and converts it
 * to a human readable float value. The conversion
 * is in Ceslius.
 *
 * @param integerPart the most significant byte of the temperature data 
 * @param decimalPart the least significant byte of the temperature data
 * 
 * @return float floatValue: new converted temperature value
 */
float TempSensor::fixedToFloat(uint8_t integerPart, uint8_t decimalPart) {
  // Combine the integer and decimal parts into a 16-bit fixed point number
  uint16_t fixedPoint = (static_cast<uint16_t>(integerPart) << 8) | decimalPart;

  // Convert the fixed point number to a float
  float floatValue = static_cast<float>(fixedPoint) / 256.0f;

  return floatValue;
}

//Same as fixedToFloat function, also works but not being used
float TempSensor::convert_raw_temp(int16_t raw_temp) {
    // Extract upper and lower bytes
    int8_t upper_byte = raw_temp >> 8;  // Get the upper byte
    uint8_t lower_byte = raw_temp & 0xFF;  // Get the lower byte
    // Calculate the fractional part
    uint8_t fractional_part = lower_byte;

    // Get the integer part of the temperature
    uint8_t integer_part = static_cast<uint8_t>(upper_byte);
    // Combine the integer and decimal parts into a 16-bit fixed point number
    uint16_t fixedPoint = (integer_part << 8) | fractional_part;

    
    // Calculate the temperature
    temp_C = static_cast<float>(fixedPoint)/256.0f;

    return temp_C;
}

/**
 * @brief Get Temperature in C
 *
 * This function calls all the required functions
 * to retrieve the temperature data from the sensor
 * and convert it to readable format.
 *
 * @return float temp_C: the final converted temperature in Celsius
 */
//Call the Raw_temp_read to get data, then convert it to Celsius
float TempSensor::get_Temp_C(){
    Raw_Temp_Read();
    temp_C = fixedToFloat(integerPart, decimalPart);
    //temp_C = convert_raw_temp(raw_temperature);
    return temp_C;
}

/**
 * @brief Get Temperature in F
 *
 * This function converts the Celsius to Fahrenheit
 *
 * @return float temp_F: the final converted temperature in Fahrenheit
 */
float TempSensor::get_Temp_F(){
    temp_F = (temp_C * 9/5) + 32;
    return temp_F;
}

//******************************************************//
//**********CONFIGURATION SETTINGS FUNCTIONS************//
//******************************************************//

/**
 * @brief Read Config Register
 *
 * This function reads and sends the config register from the sensor
 *
 * @return uint8_t confifValue: the config register
 */
uint8_t TempSensor::readConfigRegister(){
    uint8_t configValue;
    //Let controller know target address
    Read_Reg(I2C_PIN, sensor_addr, CONFIG_REG, &configValue, 1);

    return configValue;
}

/**
 * @brief Write Config Register
 *
 * This function writes to the config register to change it
 * to the settings the user wants to use for the sensor.
 *
 * @param mask the bits being changed in the register
 * @param value the value of the bits to be changed in the register
 *
 * @return void
 */
void TempSensor::modifyConfigRegister(uint8_t mask, uint8_t value){
    //Take in the Config register value
    uint8_t configValue = readConfigRegister();

    //clear the bits that have to be masked/changed to prevent errors
    configValue &= ~mask;
    configValue |= (value & mask);

    //i2c_write_blocking(I2C_PIN, sensor_addr, &configValue, 1, false);
    Write_Reg(I2C_PIN, sensor_addr, CONFIG_REG, &configValue, 1);
}

//******************************************************//
//************HYSTERESIS SETTINGS FUNCTIONS*************//
//******************************************************//

/**
 * @brief Read Hysteresis Register
 *
 * This function reads the hysteresis value from the register
 *
 * @return void
 */
void TempSensor::Read_Hyst_Reg(){
    uint8_t temp[2] = {0,0};
    Read_Reg(I2C_PIN, sensor_addr, HYST_TEMP_REG, temp, 2);
    std::cout << "Minimum Temp Set to: " << fixedToFloat(temp[0], temp[1])
    << std::endl;
    sleep_ms(500);
}

/**
 * @brief Write Hysteresis Register
 *
 * This function write to the hysteresis register and changes the value
 *
 * @return void
 */
void TempSensor::Write_Hyst_Reg(){
    Write_Reg(I2C_PIN, sensor_addr, HYST_TEMP_REG, hyst_limit, 2);
}

//******************************************************//
//************LIMIT SET SETTINGS FUNCTIONS**************//
//******************************************************//

/**
 * @brief Read Set Register
 *
 * This function reads the Set value from the register
 *
 * @return void
 */
void TempSensor::Read_Set_Reg(){
    uint8_t temp[2] = {0,0};
    Read_Reg(I2C_PIN, sensor_addr, SET_TEMP_REG, temp, 2);
    std::cout << "Maximum Temp Set to: " << fixedToFloat(temp[0], temp[1])
    << std::endl;
    sleep_ms(500);
}

/**
 * @brief Write Set Register
 *
 * This function write to the Set register and changes the value
 *
 * @return void
 */
void TempSensor::Write_Set_Reg(){
    Write_Reg(I2C_PIN, sensor_addr, SET_TEMP_REG, set_limit, 2);
}

/**
 * @brief Verify Register
 *
 * This function checks if the proper bits were changed
 * in the register and blinks an LED accordingly.
 *
 * @param mask the bits to mask in the register
 * @param data the value of the bits changed in the register
 *
 * @return void
 */
void TempSensor::VerifyReg(uint8_t mask, uint8_t data){
    uint8_t configReg = readConfigRegister();

    //clear the bits that have to be masked/changed to prevent errors
    configReg &= mask;
    if(configReg == data){
        //if setting change worked, blink green led 3 times
        for(int i =0; i < 3; i++){
            green_led.blinkLED();
        }
    } else {
        //operation failed, blink red led 3 times
        for(int i =0; i < 3; i++){
            red_led.blinkLED();
        }
    }
}


/**
 * @brief Reads user input from the console until Enter is pressed.
 *
 * This function reads user input from the console until the Enter key is pressed.
 * The input is stored in the specified string.
 *
 * @return The user input string.
 */
std::string TempSensor::get_input() {
  std::string input;
  char c;
  while (true) {
    c = getchar(); // Read a character from the console
    if (c == '\n' || c == '\r') { // Check if Enter key is pressed
      break; // Exit the loop
    } else {
      input += c; // Append the character to the input string
    }
  }
  return input;
}


