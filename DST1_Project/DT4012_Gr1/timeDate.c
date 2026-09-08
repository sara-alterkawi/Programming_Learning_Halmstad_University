#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              Time_Date                
// *****************************************************************************
unsigned int recordCounter = 0; //To keep track of the number of records
unsigned int fastMode = 0 ; //To determine whether the system is in fast mode or not
unsigned int counter = 0;
unsigned int day = 14; //The current day of the month
unsigned int month = 04; //The current month
unsigned int year = 2023; //The current year
unsigned int hour = 00; //The current hour
unsigned int minute = 00; //The current minute
unsigned int second = 00; //The current second
unsigned int numbChange = 0; //To keep track of which part of the date/time is being edited
unsigned int timeSet = 0; //To determine whether the time has been set or not

void SysTick_Handler(){ //Periodic interrupts handler at a fixed interval
  recordCounter++; //To determine when to trigger readings
  
  if(fastMode == 0){ //Check whether fastMode is enabled or not
    if(recordCounter >= 1800000){ //30 * 60000 = 108000 m.s
      recordCounter = 0;
      tempFlag = 1;
    }
  }else{
    if(recordCounter >= 1000){ //1 second = 1000 ms
      recordCounter = 0;
      tempFlag = 1;
    }
  }
  
  counter += 1;
  if(counter >= 1800 || fastMode){ // 30m * 60 = 1800 s
    counter = 0;
    second ++;
    if (second == 60){ //60 second = 1 minute
      second = 0;
      minute++;
      houseLight();
      if(minute == 60){ //60 minute = 1 hour
        minute = 0; 
        hour ++;
        if (hour == 24){ //24 hour = day
          weekData(1);
          hour = 0; 
          minutesOfLighting = 0;
          ledLight = 0;
          sunLight = 0;
          darkness = 0;
          day ++;
          if (day > monthDays(month, year)){ 
            day = 1; 
            month ++;
            if(month > 12){
              month = 1; 
              year ++;
            }
          }
        }
      }
    }
  }
}

unsigned int monthDays(unsigned int numbMonth, unsigned int numbYear){ //To determine the number of days in a given month
  switch(numbMonth){
  case 1: //January
    return 31;
  case 2: //February
    return 29;
  case 3: //March
    return 31;
  case 4: //April
    return 30;
  case 5: //May
    return 31;
  case 6: //June
    return 30;
  case 7: //July
    return 31;
  case 8: //August
    return 31;
  case 9: //September
    return 30;
  case 10: //October
    return 31;
  case 11: //November
    return 30;
  case 12: //December
    return 31;
  default:
    return 0; //Or return an error
  }
}

void printTime(){ //Displays the current date and time on a display
  int ten, unit, hun, tho; //Date display as DD/MM/YYYY 
  
  displayRow(0); //On first row
  printWord("Current time and date is:", 25);
  
  displayRow(2); //To show the date on display
  ten = day/10; //To show DD
  Write_Data_2_Display((char)(ten +0x10));
  Write_Command_2_Display(0xc0);
  
  unit = day%10; //(%10 10 dicimal digits)
  Write_Data_2_Display((char)(unit+0x10));//To convert to BCD
  Write_Command_2_Display(0xc0);
  Write_Data_2_Display(0x0f);
  Write_Command_2_Display(0xc0);
  
  ten= month/10; //To show MM(Month)
  Write_Data_2_Display((char)(ten +0x10));
  Write_Command_2_Display(0xc0);
  
  unit = month%10;
  Write_Data_2_Display((char)(unit+0x10));
  Write_Command_2_Display(0xc0);
  Write_Data_2_Display(0x0f);
  Write_Command_2_Display(0xc0);
  
  tho= (year/1000);
  Write_Data_2_Display((char)(tho + 0x10));
  Write_Command_2_Display(0xc0);
  
  hun = (year/100)%10;
  Write_Data_2_Display((char)(hun + 0x10));
  Write_Command_2_Display(0xc0);
  Write_Data_2_Display(0x0f);
  
  ten= (year/10)%10;
  Write_Data_2_Display((char)(ten + 0x10));
  Write_Command_2_Display(0xc0);
  
  unit = year%10;
  Write_Data_2_Display((char)(unit + 0x10));
  Write_Command_2_Display(0xc0);
  Write_Data_2_Display(0x0f);
  
  displayRow(4); //To show the time on display
  ten = hour/10; //To show hh(Hour)
  Write_Data_2_Display((char)(ten +0x10));
  Write_Command_2_Display(0xc0);
  unit = hour%10;
  Write_Data_2_Display((char)(unit+0x10));
  Write_Command_2_Display(0xc0);
  Write_Data_2_Display(0x1a);
  Write_Command_2_Display(0xc0);
  
  ten = minute/10; //To show mm(Minute)
  Write_Data_2_Display((char)(ten +0x10));
  Write_Command_2_Display(0xc0);
  unit = minute%10;
  Write_Data_2_Display((char)(unit+0x10));
  Write_Command_2_Display(0xc0);
  Write_Data_2_Display(0x1a);
  Write_Command_2_Display(0xc0);
  
  ten = second/10; //To show ss(Second)
  Write_Data_2_Display((char)(ten +0x10));
  Write_Command_2_Display(0xc0);
  unit = second%10;
  Write_Data_2_Display((char)(unit+0x10));
  Write_Command_2_Display(0xc0);
  
  displayRow(13);
  Write_Command_2_Display(0xc0);
  printWord("Press # to set the time", 23);
  
  displayRow(15); //On the last row
  Write_Command_2_Display(0xc0);
  printWord("Press * to go back", 18);
  
  if (timeSet == 1){ //To move the curser to the number want to edit
    if (numbChange < 10){
      displayRow(5); //To put the arrow under the row
      displySpaces(8); // put the cursor on spesific place
      displayRow(3);
      displySpaces(numbChange);
    }else{
      displayRow(3);
      displySpaces(10);
      displayRow(5);
      displySpaces(numbChange - 10);
    }
    Write_Data_2_Display(0x3E);
    Write_Command_2_Display(0xc0);
  }else{
    displayRow(2);
    displySpaces(10);
    displayRow(5);
    displySpaces(8);
  }
}

void setTime(unsigned int button){//To set the date and time
  if(button == 11) //If the button 0 is pressed
    button = 0;
  //Begin edit day
  if (numbChange == 0){//If button # is pressed 
    day = day - ((day/10)*10) + (button*10); 
  }else if (numbChange == 1){
    day = day - (day%10) + button;
    //2 //Begin edit month
  }else if (numbChange == 3){
    month = month - ((month/10)*10) + (button*10); 
  }else if (numbChange == 4){
    month = month - (month%10) + button;
    //5 //Begin edit year
  }else if (numbChange == 6){
    year = year - ((year/1000)*1000) + (button*1000); 
  }else if (numbChange == 7){
    year = year - (((year/100)%10)*100) + (button*100);
  }else if (numbChange == 8){
    year = year - (((year/10)%10)*10) + (button*10);
  }else if (numbChange == 9){
    year = year - (year%10) + button;
    //Move to edit time 
    //Begin edit hour
  }else if (numbChange == 10){
    hour = hour - ((hour/10)*10) + (button*10); 
  }else if (numbChange == 11){
    hour = hour - (hour%10) + button;
    //12 //Begin edit minute
  }else if (numbChange == 13){
    minute = minute - ((minute/10)*10) + (button*10);
  }else if (numbChange == 14){
    minute = minute - (minute%10) + button;
    //15 //Begin edit second
  }else if (numbChange == 16){
    second = second - ((second/10)*10) + (button*10);
  }else if (numbChange == 17){
    second = second - (second%10) + button;
  }
  numbChange++;
  if(numbChange == 2 || numbChange == 5 || numbChange == 12 || numbChange == 15)
    numbChange += 1;//To move to the next digit editing
  if(numbChange > 17){//If editing are finished show the time 
    numbChange = 0;
    timeSet = 0;
  }
}

