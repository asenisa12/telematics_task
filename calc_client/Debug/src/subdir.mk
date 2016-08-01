################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/client.c \
../src/main.c 

OBJS += \
./src/client.o \
./src/main.o 

C_DEPS += \
./src/client.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/devteam/workspace/telematics_task/calc_client/src" -include/home/devteam/workspace/telematics_task/headers/message_utils.h -include/home/devteam/workspace/telematics_task/headers/message.pb-c.h -include"/home/devteam/workspace/telematics_task/headers/message_utils.h" -include"/home/devteam/workspace/telematics_task/headers/message.pb-c.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


