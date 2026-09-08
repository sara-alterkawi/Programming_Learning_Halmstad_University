#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              Display                
// *****************************************************************************
void Set_Display(){ //Pin configuration function for display
  //To return the display status
  *AT91C_PMC_PCER =  (1<<13)|(1<<14); //Enable Pmc for port C
  *AT91C_PIOC_PER = 0x3fc ; //Enable pio for pin 2,3,4,5,7,8 and 9
  *AT91C_PIOC_PPUDR = 0x03fc; //Disabling the enternal resistor
  *AT91C_PIOC_ODR = 0x3fc; //Making as input
  *AT91C_PIOC_PER = 0x3fc ; //Enable pio for pin 2,3,4,5,7,8 and 9
  *AT91C_PIOC_PPUDR = 0x3fc ; //Disabling the enternal resistor
  *AT91C_PIOC_OER = 0x3fc ; //Set pins 2,3,4,5,7,8,and 9 of Port C as output
  *AT91C_PIOC_PER = 0xff001 ; //Enable the PIO controller for pins 12 to 18 and 21 to 23 of Port C
  *AT91C_PIOC_PPUDR = 0xff001 ; //Disable the pull-up resistor for pins 12 to 18 and 21 to 23 of Port C
  *AT91C_PIOC_OER = 0xff001 ; //Set pins 12 to 18 and 21 to 23 of Port C as output
}

void Delay(unsigned int Value){ //To make a delay for a specific time
  for (int i = 0; i < Value; i++)
    asm("nop"); //Placeholder to create a delay in the loop
}

unsigned char Read_Status_Display(void){
  unsigned char Temp; 
  *AT91C_PIOC_PER = 0x3fc ; //Enable pio for pins
  *AT91C_PIOC_PPUDR = 0x3fc; //Disabling the enternal resistor
  *AT91C_PIOC_ODR = 0x3fc; //Making as input
  *AT91C_PIOC_SODR = (1<<13); //dir
  *AT91C_PIOC_CODR = (1<<12); //Enable output
  *AT91C_PIOC_SODR = (1<<14); //C/D
  *AT91C_PIOC_CODR = (1<<15); //Chip select display
  *AT91C_PIOC_CODR = (1<<16); //Read display
  Delay(10); //Make a delay of 10 microseconds
  Temp = (*AT91C_PIOC_PDSR & 0x3fc);
  *AT91C_PIOC_SODR = (1<<15); //Chip select display
  *AT91C_PIOC_SODR = (1<<16); //Read display
  *AT91C_PIOC_SODR = (1<<12); //Enable output
  *AT91C_PIOC_CODR = (1<<13); //dir
  return Temp;
}

void Write_Data_2_Display(unsigned char Data){ //To send tha data to display
  while((~Read_Status_Display() & (0xC)) == 0xC){}//Wait until the display is ready to receive data
  *AT91C_PIOC_CODR = 0x3fc;  //Setting all in/outpins to 0
  *AT91C_PIOC_SODR = (Data<<2);//Shifting to 2 becouse pin 0 and 1 undefined
  *AT91C_PIOC_CODR = (1<<13); //Set direction of the data bus
  *AT91C_PIOC_CODR = (1<<12); //Enable output enables
  *AT91C_PIOC_PER = 0x3fc ;  //Making the pins as output
  *AT91C_PIOC_PPUDR = 0x3fc ; //Disabling their pull-up resistors
  *AT91C_PIOC_OER = 0x3fc ; //Set pins outputs
  *AT91C_PIOC_PER = 0xff001 ; //Enable peripheral control set them as outputs 
  *AT91C_PIOC_PPUDR = 0xff001 ; //Disabling pull-up resistors
  *AT91C_PIOC_OER = 0xff001 ; //Set them as outputs 
  *AT91C_PIOC_CODR = (1<<14); //Set the RS bit to 0 C/D
  *AT91C_PIOC_CODR = (1<<15); //Chip select display
  *AT91C_PIOC_CODR = (1<<17); //Write display
  Delay(10); //Make a delay of 10 microseconds
  *AT91C_PIOC_SODR = (1<<15); //Chip select display
  *AT91C_PIOC_SODR = (1<<17); //Write display
  *AT91C_PIOC_SODR = (1<<12); //Disable output
  *AT91C_PIOC_ODR = 0x3fc; //Making databus as input
}

void Write_Command_2_Display(unsigned char Command){ //To send a command to the display
  while((~Read_Status_Display() & (0xC)) == 0xC){} //Wait until Read_Status_Display returns an OK
  *AT91C_PIOC_CODR = 0x3fc;  //Setting all in/outpins to 0
  *AT91C_PIOC_SODR = (Command<<2);//Shifting to 2 becouse pin 0 and 1 undefined
  *AT91C_PIOC_CODR = (1<<13); //Set direction of the data bus
  *AT91C_PIOC_CODR = (1<<12); //Enable output
  *AT91C_PIOC_PER = 0x3fc ;  //Making the pins as output
  *AT91C_PIOC_PPUDR = 0x3fc ; //Disabling their pull-up resistors
  *AT91C_PIOC_OER = 0x3fc ; //Set pins outputs
  *AT91C_PIOC_PER = 0xff001 ; //Enable peripheral control set them as outputs 
  *AT91C_PIOC_PPUDR = 0xff001 ; //Disabling pull-up resistors
  *AT91C_PIOC_OER = 0xff001 ; //Set databus as output
  *AT91C_PIOC_SODR = (1<<14); //Set C/D signal High (1 = Command)
  *AT91C_PIOC_CODR = (1<<15); //Clear chip select display
  *AT91C_PIOC_CODR = (1<<17); //Clear write display
  Delay(10); //Make a delay of 10 microseconds
  *AT91C_PIOC_SODR = (1<<15); //Chip select display
  *AT91C_PIOC_SODR = (1<<17); //Write display
  *AT91C_PIOC_SODR = (1<<12); //Disable output
  *AT91C_PIOC_ODR = 0x3fc; // Making as input
}

void Init_Display(void){ //Clear Reset display
  *AT91C_PIOD_CODR = (1<<0); //Clear Reset display
  Delay(10); //Make a delay of 10 microseconds
  *AT91C_PIOD_SODR = (1<<0); //Set Reset display
  Write_Data_2_Display(0x00); //Write two bytes of data (0x00) to the display
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40); //Set text home address
  Write_Data_2_Display(0x00); //Set the starting position for text display on the screen
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x1e); //Set start position for graphic display on the screen
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); //Set text area
  Write_Data_2_Display(0x1e); //Set the size of the text display area on the screen
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); //Set graphic area
  Write_Command_2_Display(0x80); //Text mode
  Write_Command_2_Display(0x94); //Text on graphic off
}

void clearDisplay(){ //Clear the display by sending clear characters to the display to fill the whole screen
  Write_Data_2_Display(0x0); 
  Write_Command_2_Display(0x24); //Set address to the start of the text area
  for(int i = 0; i < (480); i++){ //loop to write data to the display
    Write_Data_2_Display(0x0); //To write data and increment the display address pointer 
    Write_Command_2_Display(0xC0); //Data Write and Increment ADP
  }
  Write_Data_2_Display(0x0); 
  Write_Data_2_Display(0x0);
  Write_Command_2_Display(0x24); //Set the display address to the start of the text area
}

void displayRow(unsigned int row){ //Sets cursor and adress pointers
  unsigned int pointer = 30*row; //To calculates the address pointer for the given row
  char LSB = pointer & 0xFF; //Extracts the least significant byte of the pointer address
  char MSB = pointer >> 8; //Extracts the most significant byte of the pointer address by shifting the pointer to the right by 8 bits
  Write_Data_2_Display(LSB); //Send the least significant byte of the address pointer to the display
  Write_Data_2_Display(MSB); //Send the most significant byte of the address pointer to the display
  Write_Command_2_Display(0x24); //Set the address pointer
}

void displySpaces(int space){ //Puting space between char on displays
  for (int i = 0; i < space; i++){ //Loop which will execute space number of times
    Write_Data_2_Display(0); //Send the value 0 to the display
    Write_Command_2_Display(0xc0); //Send the command
  }
}

void printWord(char* s, int n){ //To print word on screen
  for(int i = 0; i < n; i += 1){ //Loop over the characters of the string
    if(s[i] == ' ') //Current character is a space
      Write_Data_2_Display(0); //Send a "0" (ASCII code 32) to the display as a space
    else
      Write_Data_2_Display(s[i] - 'a' + 0x41);  //To convert from aski to display code and send data
    Write_Command_2_Display(0xC0); //Move cursor to next character position
  }
}

