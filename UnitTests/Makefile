
vpath %.c ../src
vpath %.h ../inc

unittest: unit_test.o lightSensor.o i2c.o
	  $(CC) -o unittest unit_test.o lightSensor.o i2c.o -pthread -lcunit -lm

unit_test.o: unit_test.c
	     $(CC) -c unit_test.c -pthread

lightSensor.o: lightSensor.c lightSensor.h i2c.h
	$(CC) -c ../src/lightSensor.c -lm

i2c.o:		i2c.c i2c.h
		$(CC) -c ../src/i2c.c

clean:
	rm unittest *.o *.xml
