################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../canop.c \
../client.c \
../main.c \
../serialport.c \
../server.c \
../udp_server.c 

O_SRCS += \
../canop.o \
../main.o \
../serialport.o 

OBJS += \
./canop.o \
./client.o \
./main.o \
./serialport.o \
./server.o \
./udp_server.o 

C_DEPS += \
./canop.d \
./client.d \
./main.d \
./serialport.d \
./server.d \
./udp_server.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	/home/yhx/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


