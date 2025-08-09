import network

ap = network.WLAN(network.AP_IF) # create access-point interface
ap.active(True)         # activate the interface
ap.config(essid="Turnip", authmode=network.AUTH_WPA_WPA2_PSK, password="12345678")