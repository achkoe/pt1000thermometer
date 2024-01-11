src = cc

c: compile

all: compile upload

compile:
	arduino-cli compile --warnings all --fqbn arduino:avr:nano:cpu=atmega328old -v $(src)

upload:
	arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328old $(src)

doc: doc/pt1000thermometer.tex
	cd doc && latexmk -xelatex pt1000thermometer.tex && cd ..