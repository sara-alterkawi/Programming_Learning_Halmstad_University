#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
#include <stdlib.h>
// *****************************************************************************
//                              Temp_sensor                
// *****************************************************************************
struct NodeDate *data[7];//To store the minimum, maximum, and average temperature data for 7 days, with the corresponding timestamps
unsigned int dayNumber = 0; //To keep track of the current day
unsigned tempFlag = 1; //To indicate whether a new temperature reading has been taken
double temprature; //To store the current temperature reading
int resetCounter = 0; //To count the number of times the system has been reset
unsigned int recordCheck = 0; //To determine whether new temperature data should be recorded
struct LinkedList *currentTime; //To keep track of the current time
unsigned int upperLimit = 25, lowerLimit = 20; //Upper and lower temperature limits for the system
unsigned int configLimits = 0; //To determine whether the temperature limits have been configured

struct NodeDate { //To define node with datae data(min max avg temperature time-stamp)
  unsigned int month, day, year;
  unsigned int tempRecordSum, tempNRecords; //To calvulate the records sum, the records number
  unsigned int hourForMin, minuteForMin; //Time in minute and hour for the minimum temperature
  double minData; //To record minimum temperature
  double maxData; //To record maximum temperature
  unsigned int hourForMax, minuteForMax; //Time in minute and hour for the maximum temperature
  double avgData;  //To calculate the avarage   
};

void Init_Temp(){ //Initializes the temperature sensor.
  *AT91C_PMC_PCER = ((1<<12)|(1<<27)); //Enable PMC for pin 2 and TC0
  *AT91C_TC0_CCR = 0x5;//Enable counter and make a sw_reset in TC0_CCR0
  *AT91C_TC0_CMR = (*AT91C_TC0_CMR& 0xFFF0FFF8) | (0x0)|(1<<17) |(1<<18); //Set the clock source to MCK/8
  *AT91C_PIOB_PER = (1<<25); //Enable the pin 2
  *AT91C_PIOB_OER = (1<<25); // Make pin 2 as output.
  *AT91C_PIOB_SODR = (1<<25); //Set pin 2 high.
  *AT91C_PIOB_CODR = (1<<25); //Set pin 2 low.
  Delay(9966); //Delay before setting pin 2 high again
  *AT91C_PIOB_SODR = (1<<25); //Set pin2 as high;
  //Create init puls
  NVIC_ClearPendingIRQ(TC0_IRQn); //Clears any pending interrupts on TC0
  NVIC_SetPriority(TC0_IRQn, 0); //NVIC_Setpriority(TC0_IRQn,0);
  NVIC_EnableIRQ(TC0_IRQn); //Enable the interrupt (NVIC) with the inline declared function
  *AT91C_TC0_SR; //Clear any pending interrupts
  for(int i = 0; i < 7; i ++){ //Initialize the temperature data for seven days
    data[i] = NULL;
  }
  data[dayNumber] = (struct NodeDate*)malloc(sizeof(struct NodeDate)); //Allocates memory for a struct NodeDate
  data[dayNumber]->maxData = 0; //Sets the maximum temperature
  data[dayNumber]->minData = 100; //Sets the minimum temperature  
  data[dayNumber]->tempRecordSum = 0; //Sets the sum of temperature records for the current day
  data[dayNumber]->tempNRecords = 0; //Sets the number of temperature records for the current
}

void signalReseting(){ //Set up the pin connected to the temperature sensor to start a new measurement
  *AT91C_PIOB_OER = AT91C_PIO_PB25; //Make pin 2 as output
  *AT91C_PIOB_SODR = AT91C_PIO_PB25; //Set it high
  *AT91C_PIOB_CODR = AT91C_PIO_PB25; //Set it lows
}

void signalReading(){ //Sets up the pin connected to the temperature sensor to read the temperature after a brief Delay(25).
  *AT91C_PIOB_SODR = AT91C_PIO_PB25; //(1<<25) //set pin2 as 1
  *AT91C_PIOB_CODR = AT91C_PIO_PB25;  //set pin2 as 0
  Delay(25); //Make a delay of 25 microseconds
  *AT91C_PIOB_SODR = AT91C_PIO_PB25; //Set pin2 as output
  *AT91C_PIOB_ODR = AT91C_PIO_PB25; //Set pins2 as input
  *AT91C_TC0_CCR = (1<<2); //make a sw_reset in TC0_CCR0
  *AT91C_TC0_SR; //Clear old interrupt by read status reg
  *AT91C_TC0_IER = (1<<6);//Enable interrupt for LDRBS
}

void TC0_Handler(){ //Interrupt handler
  *AT91C_TC0_IDR = (1<<6);//Disable interrupt LDRBS
  tempFlag = 4; //Set a global flag
}

double Read_Temperature(){ //Calculates the temperature from the raw data read by the microcontroller.
  int A = *AT91C_TC0_RA ; //Read data from the microcontroller's temperature sensor 
  int B = *AT91C_TC0_RB; 
  int D = B - A; //Calculates the difference between the two data readings
  double tmp = (D / (42 * 5)) - 273.15; //Calculates the temperature in Celsius
  temprature = tmp; 
  return tmp;
}

void temperatureReading(){ //Controls the temperature reading process.
  if(tempFlag == 1){
    signalReseting();
    tempFlag = 2;
    
  }else if(tempFlag == 2){
    resetCounter ++;
    if(resetCounter == 15){ //To finish temprature reading     
      resetCounter = 0;
      signalReading();
      tempFlag = 3;
    }
    
  }else if(tempFlag == 4){
    Read_Temperature();
    alarmCheck();
    tempFlag = 1;
    if(recordCheck){
      struct LinkedList * newRead = newRecord(minute,hour,temprature);
      if(!newRead){
        linkedListRecords();
        newRead = newRecord(minute,hour,temprature);
      }
      insertFirst(&currentTime, newRead);		
    }
  }
}

void linkedListRecords(){ //To store the temperature record
  if(currentTime == NULL)
    return;
  
  data[dayNumber]->day = day; //Set the day of the current day's record
  data[dayNumber]->month = month; //Set the month of the current day's record
  data[dayNumber]->year = year; //Set the year of the current day's record
  
  struct LinkedList *node = getMin(currentTime); //Create new node with data(minute, hour and temperature "temprature")
  if(data[dayNumber]->minData > node->temperature){ //Condition to find minimum temprature and assigned it in the new node
    data[dayNumber]->minData = node->temperature; //Assign the minimum temprature
    data[dayNumber]->minuteForMin = node->minutes; //Assign the minute for the minimum temprature
    data[dayNumber]->hourForMin = node->hours; //Assign the hour for the minimum temprature
  }
  
  node = getMax(currentTime); //Points to the maximum temperature reading in the current time
  if(data[dayNumber]->maxData < node->temperature){ //Condition to find maximum temprature and assigned it in the new node
    data[dayNumber]->maxData = node->temperature; //Assign the maximum temprature
    data[dayNumber]->minuteForMax = node->minutes; //Assign the minute for the maximum temprature
    data[dayNumber]->hourForMax = node->hours; //Assign the hour for the maximum temprature
  }
  
  data[dayNumber]->tempRecordSum += sum(currentTime); //Update the sum of temperature records for the day
  data[dayNumber]->tempNRecords += size(currentTime); //Update the number of temperature records for the day 
  data[dayNumber]->avgData = data[dayNumber]->tempRecordSum / data[dayNumber]->tempNRecords; //Calculates the average temperature reading for the day
  freeList(&currentTime); //free the memory
}

void weekData(unsigned int newDay){ //To record week data
  linkedListRecords(); //Create a linked list of temperature records for the current day
  if(newDay){ 
    dayNumber ++;
    if(dayNumber == 7) //If the dayNumber reached 7 (new week has started)
      dayNumber = 0;
  }
  if(data[dayNumber] == NULL){ 
    data[dayNumber] = (struct NodeDate*)malloc(sizeof(struct NodeDate)); //Initializes the data by allocating memory for a new NodeDate struct
    data[dayNumber] -> maxData = 0; //Sets the maximum temperature 
    data[dayNumber] -> minData = 100; //Sets the minimum temperature 
    data[dayNumber] -> tempRecordSum = 0; //Sets the sum of temperature records for the current day
    data[dayNumber] -> tempNRecords = 0; //Sets the number of temperature records for the current
  }
}

void printData(){ //To print week data to display
  int ten, unit;
  
  displayRow(0); //On first row
  printWord("Day date MIN Time MAX Time AVG", 30);
  
  for(int day = 0; day < 7; day++){
    if(data[day] == NULL){
      continue;
    }
    
    displayRow(day*2+2);//Row place to show all week data and one row empty between them
    
    //Date
    //Day
    ten = data[day]->day/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);   
    
    unit = data[day]->day%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x0f);
    Write_Command_2_Display(0xc0);
    
    //Month
    ten= data[day]->month/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = data[day]->month%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x0f);
    Write_Command_2_Display(0xc0);
    
    //Year
    ten = (data[day]->year / 10) % 10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = data[day]->year%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0);
    Write_Command_2_Display(0xc0);
    Write_Command_2_Display(0xc0);
    
    //Minimum temperature 
    ten =(unsigned int) data[day] -> minData/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> minData)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0);
    Write_Command_2_Display(0xc0);
    
    //Minimum temperature time
    //Hours
    ten= data[day] -> hourForMin/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> hourForMin)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x1a);
    Write_Command_2_Display(0xc0);
    
    //Minutes
    ten= data[day] -> minuteForMin/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> minuteForMin)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0);
    Write_Command_2_Display(0xc0);
    
    //Maximum temperature
    ten = (unsigned int) data[day] -> maxData/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> maxData)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0);
    Write_Command_2_Display(0xc0);
    
    //Maximum temperature time
    //Hours
    ten = (unsigned int) data[day] -> hourForMax/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> hourForMax)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0x1a);
    Write_Command_2_Display(0xc0);
    
    //Minutes
    ten= (unsigned int) data[day] -> minuteForMax/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> minuteForMax)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
    Write_Data_2_Display(0);
    Write_Command_2_Display(0xc0);
    
    //Aarage temperature
    ten = (unsigned int) data[day] -> avgData/10;
    Write_Data_2_Display((char)(ten +0x10));
    Write_Command_2_Display(0xc0);
    
    unit = ((unsigned int)data[day] -> avgData)%10;
    Write_Data_2_Display((char)(unit+0x10));
    Write_Command_2_Display(0xc0);
  }
  
  displayRow(15); //On the last row
  Write_Command_2_Display(0xc0);
  printWord("Press * to go back", 18); 
}

void setLimits(unsigned int button){ //To Change temperature limits
  if(button == 11) //If the 0 is pressed
    button = 0;
  if (numbChange == 0){
    upperLimit = upperLimit - ((upperLimit / 10) * 10) + (button * 10);
  }else if (numbChange == 1){
    upperLimit = upperLimit - (upperLimit % 10) + button;
    //
  }else if (numbChange == 2){
    lowerLimit = lowerLimit - ((lowerLimit / 10) * 10) + (button * 10);
  }else if (numbChange == 3){
    lowerLimit = lowerLimit - (lowerLimit % 10) + button;
  }
  numbChange ++;
  if(numbChange > 3){
    numbChange = 0;
    configLimits = 0;
  }
}

void printLimit(){ //Print limit to display
  int ten, unit;
  
  displayRow(0); //On first row
  printWord("Current temprature:", 19);
  Write_Command_2_Display(0xc0);
  
  ten = (int) (temprature)/10;
  Write_Data_2_Display((char)(ten + 0x10));
  Write_Command_2_Display(0xc0);
  
  unit = (unsigned int) (temprature)% 10;
  Write_Data_2_Display((char)(unit + 0x10));
  Write_Command_2_Display(0xc0);
  printWord("C", 1);
  
  
  displayRow(2); //On third row
  printWord("Upper limit:", 12);
  Write_Command_2_Display(0xc0);
  
  ten = upperLimit/10;
  Write_Data_2_Display((char)(ten + 0x10));
  Write_Command_2_Display(0xc0);
  
  unit = upperLimit%10;
  Write_Data_2_Display((char)(unit + 0x10));
  Write_Command_2_Display(0xc0);
  
  displayRow(4); //On fifth
  printWord("Lower limit:", 12);
  Write_Command_2_Display(0xc0);
  
  ten = lowerLimit/10;
  Write_Data_2_Display((char)(ten + 0x10));
  Write_Command_2_Display(0xc0);
  
  unit = lowerLimit%10;
  Write_Data_2_Display((char)(unit + 0x10));
  Write_Command_2_Display(0xc0);
  
  displayRow(13);
  Write_Command_2_Display(0xc0);
  printWord(" Press # to set limits", 22);
  
  displayRow(15); //On the last row
  Write_Command_2_Display(0xc0);
  printWord(" Press * to go back", 19);
  
  if (configLimits == 1){ //To move the curser to the number want to edit
    if (numbChange <= 1){
      displayRow(5); //To put the curser under the Lower limit row
      displySpaces(15); //To bigin edit temperature after the text
      displayRow(3); //To put the curser under the Upper limit row
      displySpaces(13 + numbChange);
    }else{
      displayRow(3); //To put the curser under the Upper limit row
      displySpaces(15); //To bigin edit temperature after the text
      displayRow(5); //To put the curser under the Lower limit row
      displySpaces(11 + numbChange);
    }
    Write_Data_2_Display(0x3E);
    Write_Command_2_Display(0xc0);
  }else{
    displayRow(5);
    displySpaces(15);
    displayRow(3);
    displySpaces(15);
  }
}

void alarmCheck(){ //Set alarm if the temperature is out of range 
  if(temprature < lowerLimit || temprature > upperLimit){
    *AT91C_PIOD_SODR = AT91C_PIO_PD3; //Turn the led on set output data register    
  }
  else if(button == 10){
    *AT91C_PIOD_CODR = AT91C_PIO_PD3; //Turn the led off  clear output data
  }
}

void printAlarm(){
  
  displayRow(0);
  clearDisplay();  
  Write_Command_2_Display(0x00);
  printWord("Warning !!!", 11);
  
  displayRow(2); 
  Write_Command_2_Display(0x00);
  printWord("Temperature is out of range", 27);
  
  
  displayRow(15); //On the last row
  Write_Command_2_Display(0xc0);
  printWord(" Press * to go to main menu", 27);
  if(button == 10){
    scrCase = 'm';//Go back to main menu
    displayRow(0);
    clearDisplay();
    Delay(5000000);
  }
}