from machine import Pin
import time

p0 = Pin(0, Pin.OUT)    # create output pin on GPIO0
p4 = Pin(4, Pin.OUT)    # create output pin on GPIO4
p5 = Pin(5, Pin.OUT)    # create output pin on GPIO5

while 1 :
	p0.high()
	time.sleep(1)
	p0.low()
	p4.high()
	time.sleep(1)
	p4.low()
	p5.high()
	time.sleep(1)
	p5.low()