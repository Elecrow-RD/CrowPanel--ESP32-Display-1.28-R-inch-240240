from rotary import Rotary  # Import the Rotary class from the rotary module.
import utime as time  # Import the utime module and alias it as 'time' for timing functions.

# Create a Rotary object with the pin numbers for the rotary encoder and the interrupt pin.
rotary = Rotary(19, 18, 8)

# Initialize a variable to store the current value of the rotary encoder.
val = 0

# Define a callback function that will be called when the rotary encoder is changed.
def rotary_changed(change):
    global val  # Declare 'val' as global to modify its value inside this function.
    # Check the type of change and update 'val' accordingly.
    if change == Rotary.ROT_CW:  # If the rotary was turned clockwise.
        val = val + 1  # Increment the value.
        print(val)  # Print the new value.
    elif change == Rotary.ROT_CCW:  # If the rotary was turned counter-clockwise.
        val = val - 1  # Decrement the value.
        print(val)  # Print the new value.
    elif change == Rotary.SW_PRESS:  # If the rotary switch was pressed.
        print('PRESS')  # Print 'PRESS'.
    elif change == Rotary.SW_RELEASE:  # If the rotary switch was released.
        print('RELEASE')  # Print 'RELEASE'.

# Add the 'rotary_changed' function as a handler for rotary events.
rotary.add_handler(rotary_changed)

# Main loop that keeps the program running.
while True:
    # Pause the loop for 100 milliseconds to reduce the frequency of execution and lower CPU usage.
    time.sleep(0.1)