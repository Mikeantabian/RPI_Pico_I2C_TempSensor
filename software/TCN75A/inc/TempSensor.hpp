#ifndef TEMPSENSOR_HPP
#define TEMPSENSOR_HPP

#include <cstdint>
#include <stdio.h>
#include <string>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "led.hpp"



class TempSensor{
    public:
        TempSensor(i2c_inst_t *i2c, int sda, int scl, int freq, int redLED, int greenLED, int alert); //constructor
        void proj_init(); //initialize I2C
        void initialiseAlert(); //Initialize the Alert Gpio to handle the alerts and send an interrupt

        uint8_t bus_scan(); //bus scan will retreive temp sensor address
        bool reserved_address(uint8_t addr); // addresses to be ingnored by I2C
        
        //Read and Write from any register
        int Read_Reg(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
        uint8_t *buf, const uint8_t nbytes);
        int Write_Reg(i2c_inst_t *i2c_inst, const uint8_t addr, const uint8_t reg,
        uint8_t *buf, const uint8_t nbytes);
        
        //Temperature register functions
        //convert sensor data to readable temp
        float convert_raw_temp(int16_t raw_temp); 
        
        void Raw_Temp_Read(); // get the Raw sensor temp
        float get_Temp_C();//return the converted temp in Celsius
        float get_Temp_F(); //return the converted temp in Fahreneheit
        void displayTemp(); // display the temperature on the terminal

        //Configure register functions
        //Methods to configure settings
        uint8_t readConfigRegister(); // read the config register 
        void modifyConfigRegister(uint8_t mask, uint8_t value); // modify the config register to change settings
        float fixedToFloat(uint8_t integerPart, uint8_t decimalPart);

        //Hysteresis register functions
        //Read and write the minimum temp limit
        void Read_Hyst_Reg();
        void Write_Hyst_Reg();

        //Set register functions
        //Read and write the maximum temp limit
        void Read_Set_Reg();
        void Write_Set_Reg();

        //Changing the sensor address without rebooting
        void Modify_DeviceID(int address);

        //Function to verify config register was properly configured
        void VerifyReg(uint8_t mask, uint8_t data);

        //Function to read user input lines for temp limits
        std::string get_input();

        //Alert pin state
        static bool AlertState;
        static int pulseCount;
        //Alert gpio
        const uint8_t Alert_pin;
        //static const uint8_t Alert_pin;
        
        //******************************************//
        //***********INTERFACE FUNCTIONS************//
        //******************************************//

        //Menu functions
        void MainMenu();
        void ANSI_Codes();
        void DeviceID_Menu();
        void AlertConfig_Menu();
        void Temperature_Read_Menu();
        void Config_Menu();

        //Menu choice handlers
        void processMainMenu(const char& choice);
        void processDeviceIDMenu(const char& choice);
        void processAlertMenu(const char& choice);
        void processConfigMenu(const char& choice);
        

        //Config Setting Menus
        void ADC_RES();
        void Shutdown_Menu();
        void COMP_INT_Menu();
        void Alert_Polarity_Menu();
        void FAULT_QUEUE_Menu();
        void One_Shot_Menu();

        //Config Setting Handlers
        void processResolution(const char& choice);
        void processShutdown(const char& choice);
        void processCompInt(const char& choice);
        void processPolarity(const char& choice);
        void processFaultQ(const char& choice);
        void processOneShot(const char& choice);


        void TestingMsg();
    private:
        const int SDA_PIN, SCL_PIN, BAUD_RATE;
        i2c_inst_t *I2C_PIN;
        uint8_t sensor_addr;
        uint16_t raw_temperature; 
        float temp_C, temp_F;
        //uint8_t buf[2];
        uint8_t integerPart, decimalPart;
        uint8_t hyst_limit[2];
        uint8_t set_limit[2];


        //LED objects
        LED red_led;
        LED green_led;

        //Interface members:
        char menu_choice, Alert_choice, Temp_choice, 
        config_choice, ADC_choice, Shutdown_choice, Polarity_choice,
        Comp_Int_Choice, Fault_choice, OneShot_choice, ID_choice;
};

#endif