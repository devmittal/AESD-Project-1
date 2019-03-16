all: 	i2c.c
	arm-linux-gcc -pthread i2c.c -o i2c_program -lrt


clean:
	rm -f *.o
	rm -f i2c

