gpio号是原理图中标出的，而不是硬件插针的编号
系统默认包含 python-rpi.gpio
import RPi.GPIO as GPIO
访问gpio需要root权限

import RPi.GPIO as GPIO
import time 
led_pin = 24
button_pin = 23
 
GPIO.setwarnings(False)
GPIO.setmode( GPIO.BCM )
GPIO.setup( led_pin,GPIO.OUT )
GPIO.setup( button_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

while True:
  if GPIO.input(button_pin):
     GPIO.output(led_pin,True)
  else :
     GPIO.output(led_pin,False)


--------------
出现频率130kHz上下方波
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(26,GPIO.OUT)
while True:
    GPIO.output(26,True)
    GPIO.output(26,False)
-------------
