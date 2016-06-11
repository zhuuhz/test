CFLAGS=-I .
CROSS-COMPILE=arm-linux-gcc
OBJ_FILES= main.o canop.o serialport.o

Radar.elf:$(OBJ_FILES)
	$(CROSS-COMPILE) -lpthread -o radar -lm $(OBJ_FILES)

%.o:%.c
	$(CROSS-COMPILE) $(CFLAGS) -o $@ -c $^
	
	
.PHONY:clean
clean:
	rm -rf *.o *.c~

