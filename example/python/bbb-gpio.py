""IDE9,控制继电器
import Adafruit_BBIO.GPIO as GPIO
import time
GPIO.setup("P9_22", GPIO.OUT)
while True:
    GPIO.output("P9_22", GPIO.HIGH)
    time.sleep(5)
    GPIO.output("P9_22", GPIO.LOW)
    time.sleep(5)
