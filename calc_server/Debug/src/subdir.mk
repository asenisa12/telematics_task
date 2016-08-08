################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/message.pb-c.o \
../src/message_utils.o 

C_SRCS += \
../src/main.c \
../src/message.pb-c.c \
../src/message_utils.c \
../src/server.c 

OBJS += \
./src/main.o \
./src/message.pb-c.o \
./src/message_utils.o \
./src/server.o 

C_DEPS += \
./src/main.d \
./src/message.pb-c.d \
./src/message_utils.d \
./src/server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/home/devteam/workspace/telematics_task/headers -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


