#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              Main_Menu                
// *****************************************************************************
char scrCase = 'f'; //The first screen

void mainMenu(){
  if(scrCase == 'f'){ //The first screen means the user is on the welcome screen
    if(button == 12){ //If the user pressed "#"
      scrCase = 'm'; //Go to main menu
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }
  }else if(scrCase == 'm'){ //The main menu
    if (button == 1){ //If user pressed 1 from menu
      numbChange = 0;
      timeSet = 0;
      scrCase = 't'; //It should go to time and date display
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }else if(button == 2){ //If user pressed 2 from menu
      recordCheck = 1; //It begin to record temperature
    }else if(button == 3){//If user pressed 3 from menu
      scrCase = 'r';//To show records during the week days
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }else if(button == 4){//If user pressed 4 from menu
      numbChange = 0;
      configLimits = 0;//To show temperature limits
      scrCase = 'l';
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }else if(button == 5){//If user pressed 6 from menu
      scrCase = 'g';//To show light records during one day
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }else if(button == 6){//If user pressed 5 from menu
      if(fastMode){ //To start or stop fast-mode
        fastMode = 0;
      }else{
        fastMode = 1;
      }
      Delay(5000000);
    }
    
  }else if (scrCase == 't'){ //In case we are on time and date screen
    if (button == 10){//If * were pressed
      scrCase = 'm';//Go back to main menu
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }else if(button == 12){ //If # were pressed
      if(timeSet){ //Modify or set the time and date
        numbChange += 1; //To move to the next digit editing
        if(numbChange == 2 || numbChange == 5 || numbChange == 12 || numbChange == 15){
          numbChange += 1;
        }
        if(numbChange > 17){ //If editing are finished show the time
          numbChange = 0;
          timeSet = 0;
        }
      }else{
        timeSet = 1;
      }
      Delay(5000000);
    }else if(button > 0 && timeSet == 1){ //Condition to take value for time and date from keypad
      setTime(button);
      Delay(5000000);
    }
  }else if (scrCase == 'r'){ //In case we are on records screen
    if (button == 10){ //If * were pressed
      scrCase = 'm'; //Go back to main menu
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }
    weekData(0);
  }else if (scrCase=='l'){ //In case we are on temperature limits screen
    if (button == 10){ //If * were pressed
      scrCase = 'm'; //Go back to main menu
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }else if(button == 12){ //If # were pressed
      if(configLimits){ //Modify temperature limits
        numbChange += 1;
        if(numbChange > 4){ //If editing are finished
          numbChange = 0;
          configLimits = 0;
        }
      }else{
        configLimits = 1;
      }
      Delay(5000000);
    }else if(button > 0 && configLimits == 1){ //Condition to take value for time and date from keypad
      setLimits(button);
      Delay(5000000);
    }
  }else if (scrCase == 'g'){ //In case we are on light records screen
    if (button == 10){ //If * were pressed
      scrCase = 'm'; //Go back to main menu
      displayRow(0);
      clearDisplay();
      Delay(5000000);
    }
    weekData(0);
  }
  
  if(scrCase == 'f'){ //The first screen "welcome screen" content
    displayRow(0); //On first row
    printWord("Welcome to Smart Greenhouse", 27);
    
    displayRow(2); //On third row
    printWord("Created by Gr1", 14);
    
    displayRow(4); //On fifth row
    printWord("Rahaf & Sara", 12);
    
    displayRow(15); //On the last row
    Write_Command_2_Display(0xc0);
    printWord("Press # for Menu access", 23);
    
  }else if(scrCase == 'm'){ //In case we are on main menu screen
    displayRow(0); //Show main menu options
    Write_Command_2_Display(0xc0);
    printWord(" Smart Greenhouse", 17);    
    
    displayRow(2); //Show main menu options
    Write_Command_2_Display(0xc0);
    printWord(" Please press a number below:", 29);
    
    displayRow(4); //On fifth row
    Write_Data_2_Display(0x11);
    Write_Command_2_Display(0xc0);
    printWord(".Time and date display", 22);
    
    displayRow(6); //On seventh row
    Write_Data_2_Display(0x12);
    Write_Command_2_Display(0xc0);
    if(recordCheck == 1){
      printWord(".Temperature recording is on", 28);
    }else{
      printWord(".Start temperature recording", 28);
    }
    
    displayRow(8); //On nineth row
    Write_Data_2_Display(0x13);
    Write_Command_2_Display(0xc0);
    printWord(".View temperature records", 25);
    
    displayRow(10);
    Write_Data_2_Display(0x14);
    Write_Command_2_Display(0xc0);
    printWord(".View temperature & limits", 26);
    
    displayRow(12);
    Write_Data_2_Display(0x15);
    Write_Command_2_Display(0xc0);
    printWord(".View lightning records", 23);
    
    displayRow(14);
    Write_Data_2_Display(0x16);
    Write_Command_2_Display(0xc0);
    if (fastMode){
      printWord(".Stop fast mode ", 16);
    }else{
      printWord(".Start fast mode", 16);
    }
  }else if(scrCase == 't'){
    printTime();
  }else if(scrCase == 'r'){
    printData(); 
  }else if(scrCase == 'l'){
    printLimit();
  }else if(scrCase == 'g'){
    printLight();
  }
}