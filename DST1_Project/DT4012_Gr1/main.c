#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              Main                
// *****************************************************************************
unsigned int button;
int main(){
  SystemInit(); //Initializes the system clocks
  Init_Temp(); //Initializes the temperature sensor  
  SysTick_Config((uint32_t)(SystemCoreClock*0.001)); //Initializes the system timer and sets the timer interval to 1 millisecond
  Init_PWM(); //Initializes the servo
  
  *AT91C_PMC_PCER = (1<<14) | (1<<13); //Enable Pmc for port D and C
  
  *AT91C_PIOD_PER = AT91C_PIO_PD3; //Enables pin 3 on port D for light (alarm-light) Pin3
  *AT91C_PIOD_OER = AT91C_PIO_PD3; //Sets pin 3 on port D as an output
  *AT91C_PIOD_CODR = AT91C_PIO_PD3; //Turns off the light on pin 3 on port D
  
  *AT91C_PIOD_PER = AT91C_PIO_PD9; //Enables pin 9 on port D for light (plant-light) Pin9
  *AT91C_PIOD_OER = AT91C_PIO_PD9; //Sets pin 9 on port D as an output
  *AT91C_PIOD_CODR = AT91C_PIO_PD9; //Turns off the light on pin 9 on port D
  
  *AT91C_PIOD_PER =AT91C_PIO_PB2; //Enables pin 2 on port B for use as a button input
  *AT91C_PIOD_OER = AT91C_PIO_PB2; //Set pin 2 on port B as an output pin for the button input
  *AT91C_PIOD_SODR = AT91C_PIO_PB2; //Enables the pull-up resistor for pin 2 on port B
  
  Init_Display(); //Initializes the display
  clearDisplay(); //Clears the display
  
  while(1){ //Start an infinite loop
    temperatureReading(); //Read the temperture
    Init_ADC(); //Initializes the analog-to-digital converter (ADC)
    PWM_rotate(ADC1 , ADC2); //Rotates the PWM duty cycle based on the ADC values of two channels
    button = func(); //Reads the input value of the button
    mainMenu(); //Displays the main menu on the display
  }
  //return 0; //Exits the program
}

