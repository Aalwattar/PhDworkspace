################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mbox_lib.c \
../src/no_int_slave_1.c \
../src/operlib.c \
../src/platform.c 

OBJS += \
./src/mbox_lib.o \
./src/no_int_slave_1.o \
./src/operlib.o \
./src/platform.o 

C_DEPS += \
./src/mbox_lib.d \
./src/no_int_slave_1.d \
./src/operlib.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O3 -c -fmessage-length=0 -I../../standalone_bsp_1/microblaze_1/include -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.00.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

