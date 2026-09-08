// *****************************************************************************
//                              ADC_Ligh_Sensor                
// *****************************************************************************
extern double ADC1, ADC2;
extern int ADCFlag;
extern unsigned int ledLight, sunLight, darkness, minutesOfLighting;
void Init_ADC();
void ADC_Handler();
void houseLight();
void printLight();
// *****************************************************************************
//                              Display                
// *****************************************************************************
void Set_Display();
void Delay(unsigned int Value);
unsigned char Read_Status_Display(void);
void Write_Data_2_Display(unsigned char Data);
void Write_Command_2_Display(unsigned char Command);
void Init_Display(void);
void clearDisplay();
void displayRow (unsigned int row);
void displySpaces(int space);
void printWord(char* s, int n);
// *****************************************************************************
//                              Keypad                
// *****************************************************************************
void Set_Keypad();
void disable_keypad();
int func();
void Print_2_Display(int Value);
// *****************************************************************************
//                              Linked_List                
// *****************************************************************************
struct LinkedList{
  unsigned int minutes, hours;
  double temperature;
  struct LinkedList *next;
};
struct LinkedList *newRecord(unsigned int m, unsigned int h, double tempRead);
void insertFirst(struct LinkedList **first, struct LinkedList *el);
void freeList(struct LinkedList **first);
double sum(struct LinkedList *first);
double size(struct LinkedList *first);
struct LinkedList *getMin(struct LinkedList *first);
struct LinkedList *getMax(struct LinkedList *first);
// *****************************************************************************
//                              Main                
// *****************************************************************************
extern unsigned int button;
// *****************************************************************************
//                              Main_Menu                
// *****************************************************************************
extern char scrCase;
void mainMenu();
// *****************************************************************************
//                              Servo_PWM                
// *****************************************************************************
extern double diff;
extern int angle;
void Init_PWM(void);
void PWM_Move(unsigned int degree);
void PWM_rotate(double ADC1 , double ADC2);
// *****************************************************************************
//                              Temp_sensor                
// *****************************************************************************
extern unsigned int dayNumber, recordCheck, configLimits;
extern unsigned tempFlag, numbChange;
extern double temprature;
extern int resetCounter;
extern unsigned int upperLimit, lowerLimit;
void Init_Temp();
void signalReseting();
void signalReading();
void TC0_Handler();
double Read_Temperature();
void temperatureReading();
void linkedListRecords();
void weekData();
void printData();
void setLimits(unsigned int button);
void printLimit();
void alarmCheck();
void printAlarm();
// *****************************************************************************
//                              Time_Date                
// *****************************************************************************
extern unsigned int recordCounter, fastMode, counter, numbChange, timeSet;
extern unsigned int day, month, year;
extern unsigned int hour, minute, second;
unsigned int monthDays(unsigned int numbMonth, unsigned int numbYear);
void printTime();
void setTime(unsigned int button);
