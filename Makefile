vpath %.c src
vpath %.h inc

#ifdef Devansh
CC = /home/devansh3/Broot/buildroot/output/host/usr/bin/arm-linux-gcc
#else
CC = /home/souvik/Buildroot/buildroot/output/host/usr/bin/arm-linux-gcc
#endif

proj: 	temperature.o i2c.o
	$(CC) -pthread -o proj temperature.o i2c.o -lm

temperature.o: temperature.c temperature.h i2c.h
	$(CC) -pthread -c src/temperature.c -lm

i2c.o: i2c.c i2c.h
	$(CC) -pthread -c src/i2c.c

clean:
	rm -f *.o
	rm -f proj

