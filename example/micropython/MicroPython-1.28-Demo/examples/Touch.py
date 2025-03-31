from machine import Pin, I2C  # Import the Pin and I2C classes from the machine module.
from cst816d import Touch_CST816D  # Import the Touch_CST816D class from the cst816d module.

import time  # Import the time module for delay functions.

# Create an instance of the Touch_CST816D class, initializing in point mode (mode=1).
Touch = Touch_CST816D(mode=1)

# Set the mode attribute of the Touch object to 1, indicating point mode.
Touch.Mode = 1

# Call the Set_Mode method to configure the touch chip in the specified mode.
Touch.Set_Mode(Touch.Mode)

# Main loop that keeps the program running.
while True:
    # Call the get_point method to read the current touch point coordinates.
    Touch.get_point()
    
    # Print the X and Y coordinates of the touch point.
    print("X Point: {}, Y Point: {}".format(Touch.X_point, Touch.Y_point))
    
    # Delay the loop for 100 milliseconds to reduce CPU usage and wait for the next reading.
    time.sleep_ms(100)  # Use sleep_ms for a shorter delay.