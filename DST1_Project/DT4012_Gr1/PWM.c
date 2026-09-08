#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "func.h"
// *****************************************************************************
//                              Servo_PWM                
// *****************************************************************************
double diff = 0;
int angle = 0;

void Init_PWM(void){ //Intialize the servo
  *AT91C_PMC_PCER = (1 << 12); //Enable the clock for the (PCER) module
  *AT91C_PMC_PCER1 = (1<<4); //PMC Peripheral Clock Enable Register
  *AT91C_PIOB_PDR = (1<<17);//PIO Disable Register PIN62
  *AT91C_PIOB_ABMR = (1<<17); //Activate Peripheral B to control  the pin in  REG_PIOB_ABSR
  *AT91C_PWMC_ENA = (1<<1);//Enable PWM-channel in ENA_CHID1
  *AT91C_PWMC_CH1_CMR = 5; //Set Pre-scaler to Master_CLK/32 in PWM_CMRx
}

void PWM_Move(unsigned int degree){//To change the angel
  *AT91C_PWMC_CH1_CPRDR = (unsigned long)(2625*20); //Sets the period of the PWM signal
  double rotation = 2625 * (((degree * 1.933) / 180) + 0.7); //Calculates a value for the CDTY register for PWM channel 1
  *AT91C_PWMC_CH1_CDTYR = (unsigned int) rotation; //Sets the value of the CDTY register for PWM channel
}

void PWM_rotate(double ADC1 , double ADC2){ //To rotate a servo motor to different angles based on the sun position, 
  diff = (ADC2 - ADC1); //Calculate the difference between ADC2 and ADC1
  //Change the servo's position in accordance with the calculated difference
  if (diff >= 0.18){
    PWM_Move(180); //Move the servo 180 degrees
    angle = 180; //Set the angle to 180
    Delay(500);
  }else if (diff >= 0.14){
    PWM_Move(170);
    angle = 170;
    Delay(500);
  }else if (diff >= 0.1){
    PWM_Move(160);
    angle = 160;
    Delay(500);
  }else if (diff >= 0.06){
    PWM_Move(150);
    angle = 150;
    Delay(500);
  }else if (diff >= 0.02){
    PWM_Move(140);
    angle = 140;
    Delay(500);
  }else if (diff >= -0.02){
    PWM_Move(130);
    angle = 130;
    Delay(500);
  }else if (diff >= -0.06){
    PWM_Move(120);
    angle = 120;
    Delay(500);
  }else if (diff >= -0.1){
    PWM_Move(110);
    angle = 110;
    Delay(500);
  }else if (diff >= -0.14){
    PWM_Move(100);
    angle = 100;
    Delay(500);
  }else if (diff >= -0.18){
    PWM_Move(90);
    angle = 90;
    Delay(500);
  }else if (diff >= -0.06){
    PWM_Move(80);
    angle = 80;
    Delay(500);
  }else if (diff >= -0.22){
    PWM_Move(70);
    angle = 70;
    Delay(500);
  }else if (diff >= -0.26){
    PWM_Move(60);
    angle = 60;
    Delay(500);
  }else if (diff >= -0.3){
    PWM_Move(50);
    angle = 50;
    Delay(500);
  }else if (diff >= -0.34){
    PWM_Move(40);
    angle = 40;
    Delay(500);
  }else if (diff >= -0.38){
    PWM_Move(30);
    angle = 30;
    Delay(500);
  }else if (diff >= -0.42){
    PWM_Move(20);
    angle = 20;
    Delay(500);
  }else if (diff >= -0.46){
    PWM_Move(10);
    angle = 10;
    Delay(500);
  }else{
    PWM_Move(0);
    angle = 0;
    Delay(500);
    
  }
}

