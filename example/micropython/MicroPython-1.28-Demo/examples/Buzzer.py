from machine import Pin
import time

# Define the GPIO pin number connected to the buzzer, GPIO 3 is used as an example
buzzer_pin = 3

# Create a Pin object and set it to output mode
buzzer = Pin(buzzer_pin, Pin.OUT)

# Define a function to make the buzzer beep at a certain frequency and for a certain duration
def buzz(freq, duration):
    # Calculate the period time (cycles per second)
    period = 1.0 / freq
    # Calculate the delay between interrupts (in microseconds)
    delay = int(period * 1e6)
    
    # Calculate the end time based on the current time and the duration
    end_time = time.ticks_ms() + duration
    
    # Generate a square wave signal to make the buzzer beep
    while time.ticks_ms() < end_time:
        buzzer.value(1)  # Turn on the buzzer
        time.sleep_us(delay // 2)
        buzzer.value(0)  # Turn off the buzzer
        time.sleep_us(delay // 2)

# Make the buzzer beep at a frequency of 1000Hz for a duration of 2000 milliseconds (2 seconds)
buzz(1000, 2000)