src = cc
output=$(src)/build

c: compile

all: compile upload

compile:
	# compile for 8MHz clock
	arduino-cli compile --build-path $(output) --warnings all --fqbn arduino:avr:pro:cpu=8MHzatmega328 -v $(src)

compile_16mhz:
	# compile for 16MHz clock
	arduino-cli compile --build-path $(output) --warnings all --fqbn arduino:avr:nano:cpu=atmega328old -v $(src)

upload:
	arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328old $(src)

rupload:
	scp $(output)/$(src).ino.hex pi@192.168.178.56:/home/pi/hex
	ssh pi@192.168.178.56 sudo avrdude -p atmega328p -c linuxgpio -U flash:w:hex/$(src).ino.hex:i


doc: doc/pt1000thermometer.tex
	cd doc && latexmk -xelatex pt1000thermometer.tex && cd ..

# compile
# internal 8MHz clock, no CKDIV8
# sudo avrdude -p atmega328p -c linuxgpio -U lfuse:w:0xE2:m

# compile_16mhz
# THIS IS SETTING OF ARDUINO: external clock 16MHz
# sudo avrdude -p atmega328p -c linuxgpio -U lfuse:w:0xC6:m

# external clock
# sudo avrdude -p atmega328p -c linuxgpio -U lfuse:w:0x46:m

# internal 8MHz clock
# sudo avrdude -p atmega328p -c linuxgpio -U lfuse:w:0x62:m
