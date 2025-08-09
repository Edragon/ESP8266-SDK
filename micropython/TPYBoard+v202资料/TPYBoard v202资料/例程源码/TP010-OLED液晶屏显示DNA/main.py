# main.py -- put your code here!
import machine
from machine import Pin,I2C,SPI
import ssd1306
import math
import time

spi = SPI(baudrate=10000000, polarity=1, phase=0, sck=Pin(14,Pin.OUT), mosi=Pin(13,Pin.OUT), miso=Pin(12))
display = ssd1306.SSD1306_SPI(128, 64, spi, Pin(5),Pin(4), Pin(16))

led_blue = machine.Pin(2, Pin.OUT)  # 设置 GPIO2 为输出
led_blue.high()

try:
  display.poweron()
  display.init_display()

  display.text('TPYBoard V202',1,1)
  display.text('Hi, TurnipSmart',1,16)
  display.text('I Love You',1,31)
  display.text('This is DNA!!',1,46)
  display.show()
  time.sleep(3)
  display.fill(0)
  #显示DNA
  for x in range(0, 128):
    display.pixel(x, 32+int(math.cos(x/64*math.pi)*30 +2), 1)
    display.pixel(x, 32+int(math.cos((x+64)/64*math.pi)*30+2), 1)
  display.show()
except Exception as ex:
  led_blue.low()
  print('Unexpected error: {0}'.format(ex))
  display.poweroff()