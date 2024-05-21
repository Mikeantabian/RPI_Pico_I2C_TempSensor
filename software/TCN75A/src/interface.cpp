#include "../inc/TempSensor.hpp"
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>

/*
optional ANSII escape codes:
("\033[2J"):  Clear terminal screen 
("\033[H"):  Set cursor to home 
("\x1B[?25l"):  Hide cursor 
*/



void TempSensor::ANSI_Codes(){
    //std::cout << "\033[H\033[2J" << std::endl; 
    std::cout << "\e[1;1H\e[2J" << std::endl;
}

//******************************************************//
//****************INITIAL MENU FUNCTIONS****************//
//******************************************************//

/**
 * @brief Main Menu options
 *
 * This function prints all the main menu 
 * options and waits for the user input
 *
 */
void TempSensor::MainMenu(){
    
    ANSI_Codes();
    std::cout <<" _____ ____ _   _ _____ ____    _    "<< std::endl;
    std::cout <<"|_   _/ ___| \\ | |___  | ___|  / \\   "<< std::endl;
    std::cout <<"  | || |   |  \\| |  / /|___ \\ / _ \\  "<< std::endl;
    std::cout <<"  | || |___| |\\  | / /  ___) / ___ \\ "<< std::endl;
    std::cout <<"  |_| \\____|_| \\_|/_/  |____/_/   \\_\\ "<< std::endl;
    std::cout << "        |       MAIN MENU      |" << std::endl;
    std::cout << "    [0] |    Scan addresses    |" << std::endl;
    std::cout << "    [1] |      CONFIG Menu     |" << std::endl;
    std::cout << "    [2] |       Device ID      |" << std::endl;
    std::cout << "    [3] |      Alert Menu      |" << std::endl;
    std::cout << "    [4] |       Temp Menu      |" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: " << std::endl;
    //Take the user input
    std::cin >> menu_choice;
    // Process the user's choice
    processMainMenu(menu_choice);
}

/**
 * @brief Device ID Menu options
 *
 * This function prints all the Device ID menu 
 * options and waits for the user input
 *
 */
void TempSensor::DeviceID_Menu(){
    ANSI_Codes();
    std::cout << "Change Device ID" << std::endl;
    std::cout << "[0] 0x48" << std::endl;
    std::cout << "[1] 0x49" << std::endl;
    std::cout << "[2] 0x4A" << std::endl;
    std::cout << "[3] 0x4B" << std::endl;
    std::cout << "[4] 0x4C" << std::endl;
    std::cout << "[5] 0x4D" << std::endl;
    std::cout << "[6] 0x4E" << std::endl;
    std::cout << "[7] 0x4F" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: " << std::endl;
    //Take the user input
    std::cin >> ID_choice;

    // // Process the user's choice
    processDeviceIDMenu(ID_choice);
}

/**
 * @brief Alert Configuration Menu options
 *
 * This function prints all the Alert Configuration menu 
 * options and waits for the user input
 *
 */
void TempSensor::AlertConfig_Menu(){
    ANSI_Codes();
    std::cout << "[0] Alert Enable/Disable" << std::endl;
    std::cout << "[1] Set Set Limit" << std::endl;
    std::cout << "[2] Set Hyst Limit" << std::endl;
    std::cout << "[3] Show Set Limit" << std::endl;
    std::cout << "[4] Show Hyst Limit" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: " << std::endl;
    //Take the user input
    std::cin >> Alert_choice;
    
    // // Process the user's choice
    processAlertMenu(Alert_choice);
}

/**
 * @brief Configuration settings Menu options
 *
 * This function prints all the Configuration settings menu 
 * options and waits for the user input
 *
 */
void TempSensor::Config_Menu(){
    ANSI_Codes();
    std::cout << "[0] SHUTDOWN Setting" << std::endl;
    std::cout << "[1] Comparator/Interrupt Select" << std::endl;
    std::cout << "[2] ALERT POLARITY" << std::endl;
    std::cout << "[3] FAULT QUEUE" << std::endl;
    std::cout << "[4] ADC RES" << std::endl;
    std::cout << "[5] ONE-SHOT" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: " << std::endl;
    //Take the user input
    std::cin >> config_choice;

    // // Process the user's choice
    processConfigMenu(config_choice);
}

/**
 * @brief Prints Temperature
 *
 * This function prints The Temperature in Real time
 * using Celsius and Fahrenheit units.
 * At the moment I have it return to main menu automatically
 * because that makes it update the temperature much more frequently.
 *
 */
void TempSensor::Temperature_Read_Menu(){
    char returnMenu;
    
    ANSI_Codes();
    std::cout << "REAL TIME TEMPERATURE" << std::endl;
    printf("\n\n");

    std::cout << "   Temp C    |    Temp F   " << std::endl;
    std::cout << "-------------+-------------\n" << std::endl;
    printf("   %0.4f   |    %0.4f    \n", get_Temp_C(), get_Temp_F());
    std::cout << "\n[x] Return to main\n" << std::endl;
    sleep_ms(500); //delay the return to main menu
}

/**
 * @brief Process Main Menu
 *
 * This function handles the choice picked by the
 * user in the main menu options.
 * Depending on the choice, it will send the user to 
 * a different menu where they can continue.
 *
 */
void TempSensor::processMainMenu(const char& choice) {
    switch (choice) {
        case '0':
            // Handle option 0
            bus_scan();
            break;
        case '1':
            // Handle option 1
            Config_Menu();
            break;
        case '2':
            // Handle option 2
            DeviceID_Menu();
            break;
        case '3':
            // Handle option 3
            AlertConfig_Menu();
            break;
        case '4':
            // Handle option 4
            Temperature_Read_Menu();
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Device ID
 *
 * This function handles the choice picked by the
 * user in the Device ID menu options.
 * Depending on the choice, it will change the 
 * sensor address if it is viable.
 *
 */
void TempSensor::processDeviceIDMenu(const char& choice) {
    switch (choice) {
        case '0':
            // Handle option 0
            Modify_DeviceID(0x48);
            break;
        case '1':
            // Handle option 1
            Modify_DeviceID(0x49);
            break;
        case '2':
            // Handle option 2
            Modify_DeviceID(0x4A);
            break;
        case '3':
            // Handle option 3
            Modify_DeviceID(0x4B);
            break;
        case '4':
            // Handle option 4
            Modify_DeviceID(0x4C);
            break;
        case '5':
            // Handle option 5
            Modify_DeviceID(0x4D);
            break;
        case '6':
            // Handle option 6
            Modify_DeviceID(0x4E);
            break;
        case '7':
            // Handle option 7
            Modify_DeviceID(0x4F);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Alert Menu
 *
 * This function handles the choice picked by the
 * user in the Alert menu options.
 * Depending on the choice, it will either set the MAX | MIN
 * limits, or it will read them and print them.
 *
 */
void TempSensor::processAlertMenu(const char& choice) {
    std::string input;
    int temperature_tenths = 0;
    uint8_t msb;
    uint8_t lsb;
    switch (choice) {
        case '0':
            // Handle option 0
            break;
        case '1':
            // Handle option 1
            std::cout << "Enter a MAX Temp limit of up to 1 decimal place" << std::endl;
            //take the user inputted temperature
            input = get_input();
            
            //Parse the input string into an integer representing temperature in tenths of a degree Celsius
            for (int i = 0; i < input.length(); ++i) {
                if (isdigit(input[i])) {
                    temperature_tenths = temperature_tenths * 10 + (input[i] - '0');
                } else if(input[i] == '.'){
                    continue;
                } else {
                    std::cout << "Invalid input! Please try again" << std::endl;
                }
            }
            
            std::cout << "Limit set to: " << input << std::endl;
            // Calculate MSB and LSB
            msb = temperature_tenths / 10;  // Extract tens and units digits
            lsb = (temperature_tenths % 10 == 5) ? 0x80 : 0;  // Extract tenths digit
            set_limit[0] = msb;
            set_limit[1] = lsb;
            Write_Set_Reg();
            break;
        case '2':
            // Handle option 2
            std::cout << "Enter a MIN Temp limit of up to 1 decimal place" << std::endl;
            //take the user inputted temperature
            input = get_input();
            
            // Parse the input string into an integer representing temperature in tenths of a degree Celsius
            for (int i = 0; i < input.length(); ++i) {
                if (isdigit(input[i])) {
                    temperature_tenths = temperature_tenths * 10 + (input[i] - '0');
                }
            }
            
            std::cout << "Limit set to: " << input << std::endl;
            // Calculate MSB and LSB
            msb = temperature_tenths / 10;  // Extract tens and units digits
            lsb = (temperature_tenths % 10 == 5) ? 0x80 : 0;  // Extract tenths digit
            hyst_limit[0] = msb;
            hyst_limit[1] = lsb;
            Write_Hyst_Reg();
            break;
        case '3':
            // Handle option 3
            Read_Set_Reg();
            break;
        case '4':
            // Handle option 4
            Read_Hyst_Reg();
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Config Settings Menu
 *
 * This function handles the choice picked by the
 * user in the Config Settings menu options.
 * Depending on the choice, it will send the user to 
 * a different Setting option menu
 *
 */
void TempSensor::processConfigMenu(const char& choice) {
    switch (choice) {
        case '0':
            // Handle option 0
            Shutdown_Menu();
            break;
        case '1':
            // Handle option 1
            COMP_INT_Menu();
            break;
        case '2':
            // Handle option 2
            Alert_Polarity_Menu();
            break;
        case '3':
            // Handle option 3
            FAULT_QUEUE_Menu();
            break;
        case '4':
            // Handle option 4
            ADC_RES();
            break;
        case '5':
            // Handle option 5
            One_Shot_Menu();
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

//******************************************************//
//***********CONFIGURATION SETTINGS FUNCTIONS***********//
//******************************************************//

/**
 * @brief Shutdown Menu options
 *
 * This function prints all the menu 
 * options and waits for the user input
 *
 */
void TempSensor::Shutdown_Menu(){
    ANSI_Codes();
    std::cout << "SHUTDOWN Setting" << std::endl;
    std::cout << "[0] Disable Shutdown" << std::endl;
    std::cout << "[1] Enable Shutdown" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: ";
    std::cin >> Shutdown_choice;
    // Process the user's choice
    processShutdown(Shutdown_choice);
}

/**
 * @brief Comp/Int Menu options
 *
 * This function prints all the menu 
 * options and waits for the user input
 *
 */
void TempSensor::COMP_INT_Menu(){
    ANSI_Codes();
    std::cout << "COMP/INT Select" << std::endl;
    std::cout << "[0] Comparator Mode" << std::endl;
    std::cout << "[1] Interrupt Mode" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: ";
    std::cin >> Comp_Int_Choice;
    // Process the user's choice
    processCompInt(Comp_Int_Choice);
}

/**
 * @brief Alert Polarity Menu options
 *
 * This function prints all the menu 
 * options and waits for the user input
 *
 */
void TempSensor::Alert_Polarity_Menu(){
    ANSI_Codes();
    std::cout << "Alert Polarity" << std::endl;
    std::cout << "[0] Active low" << std::endl;
    std::cout << "[1] Active High" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: ";
    std::cin >> Polarity_choice;
    // Process the user's choice
    processPolarity(Polarity_choice);
}

/**
 * @brief Fault_Queue Menu options
 *
 * This function prints all the menu 
 * options and waits for the user input
 *
 */
void TempSensor::FAULT_QUEUE_Menu(){
    ANSI_Codes();
    std::cout << "Fault Queue" << std::endl;
    std::cout << "[0] 00" << std::endl;
    std::cout << "[1] 01" << std::endl;
    std::cout << "[2] 10" << std::endl;
    std::cout << "[3] 11" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: ";
    std::cin >> Fault_choice;
    // Process the user's choice
    processFaultQ(Fault_choice);
}

/**
 * @brief ADC Resolution Menu options
 *
 * This function prints all the menu 
 * options and waits for the user input
 *
 */
void TempSensor::ADC_RES(){
    ANSI_Codes();
    std::cout << "ADC Resolution" << std::endl;
    std::cout << "[0] 9 bit or 0.5C" << std::endl;
    std::cout << "[1] 10 bit or 0.25C" << std::endl;
    std::cout << "[2] 11 bit or 0.125C" << std::endl;
    std::cout << "[3] 12 bit or 0.0625C" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: ";
    std::cin >> ADC_choice;
    // Process the user's choice
    processResolution(ADC_choice);
}

/**
 * @brief One Shot Menu options
 *
 * This function prints all the menu 
 * options and waits for the user input
 *
 */
void TempSensor::One_Shot_Menu(){
    ANSI_Codes();
    std::cout << "One SHOT Setting" << std::endl;
    std::cout << "[0] Disable" << std::endl;
    std::cout << "[1] Enable" << std::endl;
    std::cout << "[x] Return to Main Menu" << std::endl;

    // Prompt user for selection
    std::cout << "Enter your choice: ";
    std::cin >> OneShot_choice;
    // Process the user's choice
    processOneShot(OneShot_choice);
}

/**
 * @brief Process ADC Resolution choice
 *
 * This function will set the appropriate bits in
 * the config register to change the proper setting.
 * The Verify Reg makes sure the proper bits were
 * correctly changed.
 *
 */
void TempSensor::processResolution(const char& choice) {
    uint8_t mask = 0b01100000;  // Mask for ADC resolution bits in the configuration register
    
    switch (choice) {
        case '0':
            // Set ADC resolution to 9 bits or 0.5 decimal value
            modifyConfigRegister(mask, 0b00000000);
            std::cout << "Changed Resolution to 9 bits" << std::endl;
            VerifyReg(mask, 0b00000000);
            break;
        case '1':
            // Set ADC resolution to 10 bits or 0.25 decimal value
            modifyConfigRegister(mask, 0b00100000);
            std::cout << "Changed Resolution to 10 bits" << std::endl;
            VerifyReg(mask, 0b00100000);
            break;
        case '2':
            // Set ADC resolution to 11 bits or 0.125 decimal value
            modifyConfigRegister(mask, 0b01000000);
            std::cout << "Changed Resolution to 11 bits" << std::endl;
            VerifyReg(mask, 0b01000000);
            break;
        case '3':
            // Set ADC resolution to 12 bits or 0.0625 decimal value
            modifyConfigRegister(mask, 0b01100000);
            std::cout << "Changed Resolution to 12 bits" << std::endl;
            VerifyReg(mask, 0b01100000);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Shutdown choice
 *
 * This function will set the appropriate bits in
 * the config register to change the proper setting.
 * The Verify Reg makes sure the proper bits were
 * correctly changed.
 *
 */
void TempSensor::processShutdown(const char& choice){
    // Mask for ADC resolution bits in the configuration register
    uint8_t mask = 0b00000001;  

    switch (choice) {
        case '0':
            // default: Disabled
            modifyConfigRegister(mask, 0b00000000);
            std::cout << "Shutdown Disabled" << std::endl;
            VerifyReg(mask, 0b00000000);
            break;
        case '1':
            // Shutdown Enabled
            modifyConfigRegister(mask, 0b00000001);
            std::cout << "Shutdown enabled" << std::endl;
            VerifyReg(mask, 0b00000001);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Comparator/Interrupt choice
 *
 * This function will set the appropriate bits in
 * the config register to change the proper setting.
 * The Verify Reg makes sure the proper bits were
 * correctly changed.
 *
 */
void TempSensor::processCompInt(const char& choice){
    // Mask for ADC resolution bits in the configuration register
    uint8_t mask = 0b00000010;  

    switch (choice) {
        case '0':
            // Default: set Alert to comparator mode
            modifyConfigRegister(mask, 0b00000000);
            std::cout << "Comparator Mode" << std::endl;
            VerifyReg(mask, 0b00000000);
            break;
        case '1':
            // Set Alert to Interrupt mode
            modifyConfigRegister(mask, 0b00000010);
            std::cout << "Interrupt Mode" << std::endl;
            VerifyReg(mask, 0b00000010);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Polarity choice
 *
 * This function will set the appropriate bits in
 * the config register to change the proper setting.
 * The Verify Reg makes sure the proper bits were
 * correctly changed.
 *
 */
void TempSensor::processPolarity(const char& choice){
    // Mask for ADC resolution bits in the configuration register
    uint8_t mask = 0b00000100;  

    switch (choice) {
        case '0':
            // Default: active low 
            modifyConfigRegister(mask, 0b00000000);
            std::cout << "Active low" << std::endl;
            VerifyReg(mask, 0b00000000);
            break;
        case '1':
            // Active high
            modifyConfigRegister(mask, 0b00000100);
            std::cout << "Active high" << std::endl;
            VerifyReg(mask, 0b00000100);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process Fault Queue choice
 *
 * This function will set the appropriate bits in
 * the config register to change the proper setting.
 * The Verify Reg makes sure the proper bits were
 * correctly changed.
 *
 */
void TempSensor::processFaultQ(const char& choice){
    uint8_t mask = 0b00011000;  // Mask for ADC resolution bits in the configuration register
    
    switch (choice) {
        case '0':
            // Default: a fault queue of 1
            modifyConfigRegister(mask, 0b00000000);
            std::cout << "1 queue" << std::endl;
            VerifyReg(mask, 0b00000000);
            break;
        case '1':
            // Fault queue of 2
            modifyConfigRegister(mask, 0b00001000);
            std::cout << "2 queue" << std::endl;
            VerifyReg(mask, 0b00001000);
            break;
        case '2':
            // Fault queue of 4
            modifyConfigRegister(mask, 0b00010000);
            std::cout << "4 queue" << std::endl;
            VerifyReg(mask, 0b00010000);
            break;
        case '3':
            // Fault queue of 6
            modifyConfigRegister(mask, 0b00011000);
            std::cout << "6 queue" << std::endl;
            VerifyReg(mask, 0b00011000);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}

/**
 * @brief Process One Shot choice
 *
 * This function will set the appropriate bits in
 * the config register to change the proper setting.
 * The Verify Reg makes sure the proper bits were
 * correctly changed.
 *
 */
void TempSensor::processOneShot(const char& choice){
    // Mask for ADC resolution bits in the configuration register
    uint8_t mask = 0b10000000;  

    switch (choice) {
        case '0':
            // Default: Disable
            modifyConfigRegister(mask, 0b00000000);
            std::cout << "Disabled OneShot" << std::endl;
            VerifyReg(mask, 0b00000000);
            break;
        case '1':
            // Enable
            modifyConfigRegister(mask, 0b10000000);
            std::cout << "Enabled Oneshot" << std::endl;
            VerifyReg(mask, 0b10000000);
            break;
        case 'x':
        case 'X':
            // Handle exit option
            MainMenu();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}