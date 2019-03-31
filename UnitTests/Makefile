
vpath %.c ../src
vpath %.h ../inc

PLATFORM=BBG
TARGET=unittest

ifeq ($(PLATFORM),BBG)
	ifeq ($(SOURCE),DEV)
		CC=/home/devansh3/Broot/buildroot/output/host/usr/bin/arm-linux-gcc
	else
		CC=/home/souvik/Buildroot/buildroot/output/host/usr/bin/arm-linux-gcc			
	endif
	
else
	CC=gcc
endif

all: unit_test.o lightSensor.o i2c.o
	  $(CC) -o $(TARGET) unit_test.o lightSensor.o i2c.o -pthread -lm

ifeq ($(PLATFORM),BBG)
	scp $(TARGET) root@10.0.0.219:/usr/bin
else
	./$(TARGET)
endif

unit_test.o: unit_test.c
	     $(CC) -c unit_test.c -pthread

lightSensor.o: lightSensor.c lightSensor.h i2c.h
	$(CC) -c ../src/lightSensor.c -lm

i2c.o:		i2c.c i2c.h
		$(CC) -c ../src/i2c.c

clean:
	rm unittest *.o
