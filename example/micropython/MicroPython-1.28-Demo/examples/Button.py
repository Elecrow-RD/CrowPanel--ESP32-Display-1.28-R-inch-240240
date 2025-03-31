from machine import Pin
import time

# Set the GPIO pin number where the button is connected, GPIO 1 is used as an example
button_pin = 1

# Initialize the button, set as input mode, and enable the internal pull-up resistor
button = Pin(button_pin, Pin.IN, Pin.PULL_UP)

# Main loop, here we use polling to check if the button is being held down
while True:
    # Check if the button is pressed (read the level of the GPIO pin)
    if button.value() == 0:
        print("Button is currently pressed")
        # Additional code can be added here to handle the logic when the button is pressed
    time.sleep(0.1)  # Simple debounce delay