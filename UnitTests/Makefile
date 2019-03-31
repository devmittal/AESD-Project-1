
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

OBJ_DEPS = unit_test.o lightSensor.o i2c.o temperature.o logger.o message.o

all: $(OBJ_DEPS)
	  $(CC) -o $(TARGET) $(OBJ_DEPS) -pthread -lm

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

temperature.o: temperature.c temperature.h i2c.h
		$(CC) -c ../src/temperature.c

logger.o:	logger.c logger.h
		$(CC) -c ../src/logger.c

message.o:	message.c message.h temperature.h lightSensor.h
		$(CC) -c ../src/message.c -lrt

clean:
	rm -f unittest *.o
