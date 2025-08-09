# main.py -- put your code here!
from machine import Pin
import time
while 1:
    p2 = Pin(2,Pin.OUT)
    p2.value(1)
    time.sleep(1)
    p2.value(0)