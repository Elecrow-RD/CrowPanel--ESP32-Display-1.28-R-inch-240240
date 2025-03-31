#include "I2C_BM8563.h"

#define I2C_SDA 4  // Define the SDA pin for I2C communication
#define I2C_SCL 5  // Define the SCL pin for I2C communication

I2C_BM8563 rtc(I2C_BM8563_DEFAULT_ADDRESS, Wire);  // Create an RTC object using the default I2C address and Wire library
I2C_BM8563_DateTypeDef dateStruct;  // Define a structure to hold the date information
I2C_BM8563_TimeTypeDef timeStruct;  // Define a structure to hold the time information

// Function to initialize the RTC
void RTC_init() {
  rtc.begin();  // Begin communication with the RTC
  // The following lines are commented out and provided as an example
  // of how to set a custom time and date if needed:
  /*
  I2C_BM8563_TimeTypeDef timeStruct;  // Define a time structure
  timeStruct.hours   = 11;    // Set the hour (0 - 23)
  timeStruct.minutes = 59;   // Set the minute (0 - 59)
  timeStruct.seconds = 0;    // Set the second (0 - 59)
  rtc.setTime(&timeStruct);  // Set the time in the RTC
  */
  /*
  I2C_BM8563_DateTypeDef dateStruct;  // Define a date structure
  dateStruct.weekDay = 3;    // Set the weekday (0 - 6, where 0 is Sunday)
  dateStruct.month   = 1;    // Set the month (1 - 12)
  dateStruct.date    = 24;   // Set the day of the month (1 - 31)
  dateStruct.year    = 2024; // Set the year
  rtc.setDate(&dateStruct);  // Set the date in the RTC
  */
}

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud rate
  Wire.begin(I2C_SDA, I2C_SCL);  // Initialize the I2C bus with defined SDA and SCL pins
  RTC_init();  // Call the RTC initialization function
}

void loop() {
  rtc.getDate(&dateStruct);  // Get the current date from the RTC
  rtc.getTime(&timeStruct);  // Get the current time from the RTC
  // Print the current time in a formatted manner
  Serial.printf("The update time is: %02d:%02d:%02d\n",
                timeStruct.hours,
                timeStruct.minutes,
                timeStruct.seconds
               );
  delay(1000);  // Wait for 1 second before updating the time again
}
