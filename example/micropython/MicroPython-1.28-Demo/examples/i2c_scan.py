from machine import Pin, I2C
import time

# Set up the I2C pins, using the default I2C pins for ESP32
sda = Pin(4)  # SDA pin
scl = Pin(5)  # SCL pin

# Initialize the I2C bus
i2c = I2C(0, scl=scl, sda=sda)

# Scan for devices on the I2C bus
print("Scanning I2C bus...")

# Iterate through the possible I2C address range
for addr in range(127):
    try:
        # Attempt to read a byte from the device at the current address
        i2c.readfrom(addr, 1)
        print("Found device at address:", hex(addr))
    except:
        # If an exception occurs, it means the device is not present at this address
        pass

print("Scan complete.")