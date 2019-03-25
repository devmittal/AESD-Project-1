vpath %.c src
vpath %.h inc

proj: 	lightSensor.o i2c.o
	arm-linux-gcc -pthread -o proj lightSensor.o i2c.o

lightSensor.o: lightSensor.c lightSensor.h i2c.h
	arm-linux-gcc -pthread -c src/lightSensor.c

i2c.o: i2c.c i2c.h
	arm-linux-gcc -pthread -c src/i2c.c

clean:
	rm -f *.o
	rm -f proj

