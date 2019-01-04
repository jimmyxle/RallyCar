import spidev
spi = spidev.spiDev()
spi.open(1,0)
spi.max_speed_hz = 125000
running = True
while(running):
	text = raw_input("Enter a command")
	if text == 'w':
		print(spi.xfer( [0x31] ))
	elif text == 'a':
		print(spi.xfer( [0x32] ))
	elif text =='d':
		print(spi.xfer( [0x33] ))
	elif text =='s':
		print(spi.xfer( [0x34] ))
	elif text =='q':
		print(spi.xfer( [0x30] ))
		running = False
	else 
		print(spi.xfer([0x30] ))
		running = False
spi.close()
