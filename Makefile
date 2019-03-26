vpath %.c src
vpath %.h inc

PLATFORM=BBG
SOURCE=DEVANSH
TARGET=proj

ifeq ($(PLATFORM),BBG)
	ifeq ($(SOURCE),DEVANSH)
		CC=/home/devansh3/Broot/buildroot/output/host/usr/bin/arm-linux-gcc
	else
		CC=/home/souvik/Buildroot/buildroot/output/host/usr/bin/arm-linux-gcc
	endif
else
	CC=gcc
endif

OBJ_DEPS = main.o i2c.o temperature.o lightSensor.o message.o logger.o
LIB_DEPS = i2c.h temperature.h lightSensor.h message.h logger.h

all:		$(OBJ_DEPS)
		$(CC) -o $(TARGET) $(OBJ_DEPS) -pthread -lrt -lm
		
ifeq ($(PLATFORM),BBG)
	scp $(TARGET) root@10.0.0.219:/usr/bin
	ssh root@10.0.0.219 "cd /usr/bin && ./$(TARGET)"
else
	./$(TARGET)
endif

main.o:		main.c $(LIB_DEPS)
		$(CC) -c main.c -pthread

i2c.o:		i2c.c i2c.h
		$(CC) -c src/i2c.c

temperature.o:	temperature.c temperature.h i2c.h
		$(CC) -c src/temperature.c -pthread

lightSensor.o:	lightSensor.c lightSensor.h i2c.h
		$(CC) -c src/lightSensor.c -lm

message.o:	message.c message.h temperature.h
		$(CC) -c src/message.c -lrt

logger.o:	logger.c logger.h
		$(CC) -c src/logger.c
	
clean:
		rm -f *.o
		rm -f proj

