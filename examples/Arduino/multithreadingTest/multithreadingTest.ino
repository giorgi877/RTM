
//Dependencies.
#include "RTM.h"

//Static variables.
static uint8_t ledState = LOW;

void printThread(void)
{
 Serial.println("printThread");
}

void blinkThread(void)
{
  ledState =~ ledState;
  digitalWrite(LED_BUILTIN, ledState); 
}

void setup() {
  //Init uart interface.
  Serial.begin(9600);
  //Print out sketch purpose name.
  Serial.println("Multithreading test");

  //Led init.
  pinMode(LED_BUILTIN, OUTPUT);
  //Led init state.
  digitalWrite(LED_BUILTIN, LOW); 
   
  //Not necessary, timer already is initialized.
   RTM_thread_init();
  
   //Create thread fro printing string every 3 second.
   RTM_thread_create(NULL, 3000, printThread);
   //Create thread fro LED blink every 1 second.
   RTM_thread_create(NULL, 1000, blinkThread);
}

void loop() {
 //Run all threads.
 RTM_thread_handler();
}