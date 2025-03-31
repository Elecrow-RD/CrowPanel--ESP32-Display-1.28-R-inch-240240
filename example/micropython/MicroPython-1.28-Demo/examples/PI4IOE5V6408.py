from machine import I2C, Pin
from pi4ioe5v6408ztaex import PI4IOE5V6408 
import time

# Initialize the I2C bus with the specified pins and frequency
i2c = I2C(0, sda=Pin(4), scl=Pin(5), freq=400_000)

# Create an instance of the PI4IOE5V6408 class with the I2C bus
io_expander = PI4IOE5V6408(i2c)

# Write to pins to test the functionality
# Set pin 0 (P1) high
io_expander.write_pin(0, True)
# Set pin 2 (P3) high
io_expander.write_pin(2, True)
time.sleep(1)  # Wait for 1 second
# Set pin 0 (P1) low
io_expander.write_pin(0, False)
# Set pin 2 (P3) low
io_expander.write_pin(2, False)
time.sleep(1)  # Wait for 1 second