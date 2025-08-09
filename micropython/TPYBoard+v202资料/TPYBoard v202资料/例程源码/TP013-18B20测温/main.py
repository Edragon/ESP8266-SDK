import time, ds18x20
import onewire
from machine import Pin
ow = onewire.OneWire(Pin(0))
ds = ds18x20.DS18X20(ow)
roms = ds.scan()
ds.convert_temp()
time.sleep_ms(750)
for rom in roms:
    print(ds.read_temp(rom))