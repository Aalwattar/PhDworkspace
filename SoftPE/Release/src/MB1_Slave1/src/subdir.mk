################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MB1_Slave1/src/mbox_lib.c \
../src/MB1_Slave1/src/no_int_slave_1.c \
../src/MB1_Slave1/src/operlib.c \
../src/MB1_Slave1/src/platform.c 

OBJS += \
./src/MB1_Slave1/src/mbox_lib.o \
./src/MB1_Slave1/src/no_int_slave_1.o \
./src/MB1_Slave1/src/operlib.o \
./src/MB1_Slave1/src/platform.o 

C_DEPS += \
./src/MB1_Slave1/src/mbox_lib.d \
./src/MB1_Slave1/src/no_int_slave_1.d \
./src/MB1_Slave1/src/operlib.d \
./src/MB1_Slave1/src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/MB1_Slave1/src/%.o: ../src/MB1_Slave1/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O3 -c -fmessage-length=0 -I../../standalone_bsp_1/microblaze_1/include -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.00.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


