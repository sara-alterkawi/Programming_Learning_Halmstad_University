#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              Keypad                
// *****************************************************************************
int column[3] = {AT91C_PIO_PC8, AT91C_PIO_PC9, AT91C_PIO_PC7}; //Array of columns pins 40,41,39
int row[4] = {AT91C_PIO_PC5, AT91C_PIO_PC2, AT91C_PIO_PC3, AT91C_PIO_PC4};//Array of rows pins 37,34,35,36    

void Set_Keypad(){ //Sets up the pins on the microcontroller to interface
  *AT91C_PIOD_CODR = AT91C_PIO_PC2; //Clear Output Data Register  PC2 (34)
  *AT91C_PIOC_CODR = AT91C_PIO_PC7 + AT91C_PIO_PC8 + AT91C_PIO_PC9; //Clear the output data register (39,40,41)     
  *AT91C_PIOC_OER = AT91C_PIO_PC7 + AT91C_PIO_PC8 + AT91C_PIO_PC9; // Setting pins PC7,8,9 (39,40,41) as output
  *AT91C_PIOC_ODR = (AT91C_PIO_PC2 + AT91C_PIO_PC3 + AT91C_PIO_PC4 + AT91C_PIO_PC5); //Set pins PC2,3,4,5 (34-37) as input
  *AT91C_PIOC_SODR = AT91C_PIO_PC7 + AT91C_PIO_PC8 + AT91C_PIO_PC9 + AT91C_PIO_PC13 + AT91C_PIO_PC12; //Set Output Data Register PC7,8,9,13,12 (39,40,41,50,51)
}

void disable_keypad(){//Disables the keypad chip to not receive input from the keypad
  *AT91C_PIOD_SODR = AT91C_PIO_PD2; //Setting gate as zero 
}

int func(){
  Set_Keypad();
  for (int c = 0; c < (sizeof(column) / sizeof(column[0])); c++){ //loop to iterate over each column of the keypad
    *AT91C_PIOC_CODR = column[c]; //Set current column pin to 0
    for (int r = 0; r < (sizeof(row) / sizeof(row[0])); r++){ //Nest loop to iterate over each row of the keypad
      if ((~*AT91C_PIOC_PDSR & row[r]) == row[r]){ //Checks if the current row pin is shorted to the currently selected column pin
        disable_keypad(); //Disables the keypad to prevent further input 
        *AT91C_PIOC_SODR = column[c]; //Sets the current column pin back to 1
        return (c + 1) + (r * 3); //Returns the index of the button
      }
    }
    *AT91C_PIOC_SODR = column[c]; //Sets the current column pin back to 1
  }
  disable_keypad(); //Disables the keypad to prevent further input
  return 0;
}

void Print_2_Display(int Value){ //To print keypad on display
  switch (Value) {
  case 10:
    Value = 0x0A; //To view * on the screen.
    break;
  case 11:
    Value = 0x10; //To view 0 on the screen.
    break;
  case 12:
    Value = 0x03; //To view # on the screen.
    break;
  default:
    Value = (0x10 + Value); //The numbers 1-9 will be displayed
    break;
  }
    Write_Data_2_Display(Value); //Send the data
    Write_Command_2_Display(0xC4); //Send  the command: 
}
