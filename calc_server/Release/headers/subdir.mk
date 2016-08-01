################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/devteam/workspace/telematics_task/headers/message.pb-c.c \
/home/devteam/workspace/telematics_task/headers/message_utils.c 

OBJS += \
./headers/message.pb-c.o \
./headers/message_utils.o 

C_DEPS += \
./headers/message.pb-c.d \
./headers/message_utils.d 


# Each subdirectory must supply rules for building sources it contributes
headers/message.pb-c.o: /home/devteam/workspace/telematics_task/headers/message.pb-c.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

headers/message_utils.o: /home/devteam/workspace/telematics_task/headers/message_utils.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


