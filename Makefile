vpath %.c src
vpath %.h inc

proj: 	lightSensor.o i2c.o
	gcc -pthread -o proj lightSensor.o i2c.o -lm

lightSensor.o: lightSensor.c lightSensor.h i2c.h
	gcc -pthread -c src/lightSensor.c -lm

i2c.o: i2c.c i2c.h
	gcc -pthread -c src/i2c.c

clean:
	rm -f *.o
	rm -f proj

