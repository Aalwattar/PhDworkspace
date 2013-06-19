################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DFG.c \
../src/ICAP_PR.c \
../src/data.c \
../src/dfgTemplate.c \
../src/draft.c \
../src/math.c \
../src/mbox_lib.c \
../src/no_int_main_0.c \
../src/platform.c \
../src/queue.c \
../src/schedulers.c \
../src/timer.c 

OBJS += \
./src/DFG.o \
./src/ICAP_PR.o \
./src/data.o \
./src/dfgTemplate.o \
./src/draft.o \
./src/math.o \
./src/mbox_lib.o \
./src/no_int_main_0.o \
./src/platform.o \
./src/queue.o \
./src/schedulers.o \
./src/timer.o 

C_DEPS += \
./src/DFG.d \
./src/ICAP_PR.d \
./src/data.d \
./src/dfgTemplate.d \
./src/draft.d \
./src/math.d \
./src/mbox_lib.d \
./src/no_int_main_0.d \
./src/platform.d \
./src/queue.d \
./src/schedulers.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../standalone_bsp_0/microblaze_0/include -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.00.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


