vpath %.c src
vpath %.h inc

PLATFORM=BBG
TARGET=proj
TARGET_CLIENT=clientprocess 

ifeq ($(PLATFORM),BBG)
	ifeq ($(SOURCE),DEV)
		CC=/home/devansh3/Broot/buildroot/output/host/usr/bin/arm-linux-gcc
	else
		CC=/home/souvik/Buildroot/buildroot/output/host/usr/bin/arm-linux-gcc			
	endif
	
else
	CC=gcc
endif

OBJ_DEPS = main.o i2c.o temperature.o lightSensor.o message.o logger.o remoteTask.o led.o
LIB_DEPS = i2c.h temperature.h lightSensor.h message.h logger.h remoteTask.h led.h

client: clientprocess.o message.o
		$(CC) -o $(TARGET_CLIENT) clientprocess.o message.o -lrt 

ifeq ($(PLATFORM),BBG)
	scp $(TARGET_CLIENT) root@10.0.0.219:/usr/bin
else
	./$(TARGET_CLIENT)
endif

proj:		$(OBJ_DEPS)
		$(CC) -o $(TARGET) $(OBJ_DEPS) -pthread -lrt -lm
		@echo
		@echo "							*************************************"
		@echo "							* Built Successfully With No Errors *"
		@echo "							*************************************"
		@echo
		
ifeq ($(PLATFORM),BBG)
	scp $(TARGET) root@10.0.0.219:/usr/bin
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

message.o:	message.c message.h temperature.h lightSensor.h
		$(CC) -c src/message.c -lrt

logger.o:	logger.c logger.h
		$(CC) -c src/logger.c

remoteTask.o:	remoteTask.c remoteTask.h
			$(CC) -c src/remoteTask.c 

led.o:	led.c led.h
		$(CC) -c src/led.c

clientprocess.o: clientprocess.c clientprocess.h message.h
				$(CC) -c clientprocess.c -lrt 
	
clean:
		rm -f *.o
		rm -f proj
		rm -f clientprocess

