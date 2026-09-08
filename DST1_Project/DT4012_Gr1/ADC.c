#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              ADC_Ligh_Sensor                
// *****************************************************************************
double ADC1, ADC2; //To store the ADC values
int ADCFlag; //Flag for the interrupt
unsigned int ledLight; //To keep track of the number of times the LED light was turned on
unsigned int sunLight; //To keep track of the number of times the sun light was detected
unsigned int darkness; //To keep track of the number of darkness
unsigned int minutesOfLighting; //To keep track of the number of minutes the lighting system has been on

void Init_ADC() { //Initializes the ADC peripheral
  *AT91C_PMC_PCER1 = (1<<4)|(1<<5); // Enable clock ADC
  *AT91C_PMC_PCER = (1<<11); //Enabling the PMC for PIOA
  *AT91C_PIOA_PER = (1<<4) | (1<<5); //Enabling pins 
  
  *AT91C_PIOA_OER = (1<<4) | (1<<5); //Set Port A pins 4 and 5 as output
  *AT91C_PIOA_PPUDR = (1<<4)|(1<<5); //Disable pull up register
  *AT91C_ADCC_MR &= (~(0xFF00)); //Clear prescaler
  *AT91C_ADCC_MR = (2<<8); //Set prescaler to 2
  *AT91C_ADCC_CHER = 0x6; //Enable channel 1 and 2
  
  *AT91C_ADCC_CR = 0X2; //Start ADC conversion (ADC control register)
  *AT91C_ADCC_SR; //Read the Status Register
  NVIC_ClearPendingIRQ(ADC_IRQn); //Clear the pending interrupt flag
  NVIC_SetPriority(ADC_IRQn, 1); //Set the priority of the ADC IRQ to 1
  NVIC_EnableIRQ(ADC_IRQn); //Enable the ADC IRQ in the NVIC
  *AT91C_ADCC_IER = (3<<1); //Enable interrupt for channels 1 and 2
}

void ADC_Handler(){ //Interrupt handler for the ADC
  int statusRegister = *AT91C_ADCC_SR; //Reads the status register
  
  if((statusRegister & 0X2) == 0X2){ //Checks if the bit indicating end of conversion for channel 1 is set in the status register
    ADC1 = (*AT91C_ADCC_CDR1 & 0XFFF) * (3.3 / 0xFFF); //Read the converted value from channel 1's data register
  }
  
  if((statusRegister & 0X4) == 0X4){ //Checks if the bit indicating end of conversion for channel 2 is set in the status register
    ADC2 = (*AT91C_ADCC_CDR2 & 0XFFF) * (3.3 / 0xFFF); //Reads the converted value from channel 2's data register
  }
  
  ADCFlag = 1; //Indicate that a conversion has been completed
  *AT91C_ADCC_IDR = 0X6; //Disables the interrupt for both channel 1 and channel 2
}

void houseLight(){ //To control the lighting system.
  minutesOfLighting++;
  
  if(minutesOfLighting <= 960){ //16 * 60 = 960 minutes 16 hours of lighting
    if(ADC1 + ADC2 < 3.0){
      *AT91C_PIOD_CODR = AT91C_PIO_PD9; //Set led off
      sunLight++;
      
    }else{
      *AT91C_PIOD_SODR = AT91C_PIO_PD9; //Set led on
      ledLight++;
    }
  }else{
    PWM_Move(180); //Move the servo 180 degrees //Set the shades on      
    *AT91C_PIOD_CODR = AT91C_PIO_PD9; //Set led off
    darkness++;
  }
}

void printLight(){ //Displays the current day light, darkness and sun position on a display
  //Hours of Sunlight 
  int tenSunLight = ((sunLight/60)/10);
  int unitSunLight = (sunLight/60)%10;
  
  int minTenSunLight = ((sunLight%60)/10);
  int minUnitSunLight = (sunLight%60)%10;
  
  displayRow(0); //On first row
  Write_Command_2_Display(0x00);
  printWord("Hours of Sunlight : ", 20);
  
  Write_Data_2_Display(tenSunLight + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(unitSunLight + 0x10); 
  Write_Command_2_Display(0xC0);
  
  Write_Command_2_Display(0x00); 
  printWord(":", 1);
  
  Write_Data_2_Display(minTenSunLight + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(minUnitSunLight + 0x10); 
  Write_Command_2_Display(0xC0);
     
  //Hours of LED-light
  int tenLedLight = ((ledLight/60)/10);
  int unitLedLight = (ledLight/60)%10;
  
  int minTenLedLight = ((ledLight%60)/10);
  int minUnitLedLight = (ledLight%60)%10;
  
  displayRow(2); //On third row
  Write_Command_2_Display(0x00);
  printWord("Hours of LED-light: ", 20);
  
  Write_Data_2_Display(tenLedLight + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(unitLedLight + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Command_2_Display(0x00); 
  printWord(":", 1);
  
  Write_Data_2_Display(minTenLedLight + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(minUnitLedLight + 0x10); 
  Write_Command_2_Display(0xC0);
  
  //Hours of Lightning
  int Lightning = ledLight + sunLight;
  int tenLighting = ((Lightning/60)/10);
  int unitLighting = (Lightning/60)%10;
  
  int minTenLighting = ((Lightning%60)/10);
  int minUnitLighting = (Lightning%60)%10;
  
  displayRow(4); //On fifth row
  Write_Command_2_Display(0x00);
  printWord("Hours of Lightning: ", 20);
  
  Write_Data_2_Display(tenLighting + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(unitLighting + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Command_2_Display(0x00); 
  printWord(":", 1);
  
  Write_Data_2_Display(minTenLighting + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(minUnitLighting + 0x10); 
  Write_Command_2_Display(0xC0);
  
  //Hours of darkness
  int tenDarkness = ((darkness/60)/10);
  int unitDarkness = (darkness/60)%10;
  
  int minTenDarkness = ((darkness%60)/10);
  int minUnitDarkness = (darkness%60)%10;
    
  displayRow(6); //On seventh row
  Write_Command_2_Display(0x00);
  printWord("Hours of Darkness : ", 20);
  
  Write_Data_2_Display(tenDarkness + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(unitDarkness + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Command_2_Display(0x00); 
  printWord(":", 1);
  
  Write_Data_2_Display(minTenDarkness + 0x10);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(minUnitDarkness + 0x10); 
  Write_Command_2_Display(0xC0);  
   
  //Sun position in degree
  int hunSunPosi = ((angle%1000)/100);
  int tenSunPosi = ((angle%100)/10);
  int unitSunPosi = (angle%10);
  
  displayRow(8); //On eighth row
  Write_Command_2_Display(0x00);
  printWord("Sun position in degree: ", 24);
  
  Write_Data_2_Display(hunSunPosi + 16); 
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(tenSunPosi + 16);
  Write_Command_2_Display(0xC0);
  
  Write_Data_2_Display(unitSunPosi + 16);
  Write_Command_2_Display(0xC0);
  
   if(minutesOfLighting <= 960){
    displayRow(10);
    Write_Command_2_Display(0x00);
    printWord("The shades is off", 17);

  }
   else{
    displayRow(10);
    Write_Command_2_Display(0x00);
    printWord("The shades is on ", 17);
   }
    
  displayRow(15);//On the last row
  Write_Command_2_Display(0xc0);
  printWord("Press * to go back", 18);
}


