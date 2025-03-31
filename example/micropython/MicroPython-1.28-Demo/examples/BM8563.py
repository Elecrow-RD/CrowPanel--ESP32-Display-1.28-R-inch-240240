from machine import Pin, I2C, RTC  # Import necessary modules for Pin control, I2C communication, and RTC
from bm8563rtc import PCF8563  # Import the PCF8563 module for interfacing with the PCF8563 real-time clock
import time  # Import the time module for sleep functions

# Create an I2C object for communication with the PCF8563 module
i2c = I2C(0, sda=Pin(4), scl=Pin(5), freq=400_000)
# Create an instance of the PCF8563 real-time clock module
bm = PCF8563(i2c)
# Create an instance of the machine's RTC module
rtc = RTC()

def Time():
    # Define a list of week days
    week = ['Mon', 'Tues', 'Wednes', 'Thurs', 'Fri', 'Satur', 'Sun']
    
    # Check if the year from the PCF8563 module is not the current year
    if bm.datetime()[0] != 2023:
        # Get the date and time from the machine's RTC module
        date = rtc.datetime()
        # Set the date and time to the bm8563 module
        # Note: The datetime tuple is in the format (year, month, day, hour, minute, second, weekday)
        bm.datetime((date[0], date[1], date[2], date[4], date[5], date[6], date[3]))
        # Wait for 1 second to allow the time to be set
        time.sleep(1)
        
    # Get the current year, month, day, hour, minute, second, and weekday from the bm8563 module
    year = bm.datetime()[0]
    month = bm.datetime()[1]
    mday = bm.datetime()[2]
    hour = bm.datetime()[3]
    minute = bm.datetime()[4]
    second = bm.datetime()[5]
    weekday = bm.datetime()[6]
    
    # Print the current date and time to the serial console
    print(year, month, mday, hour, minute, second, week[weekday])
    # Wait for 1 second before printing the next time update
    time.sleep(1)

# Main loop to continuously print the time
while True:
    Time()