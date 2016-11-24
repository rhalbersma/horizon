################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patterns/one_keeps_two.cpp \
../patterns/two_keeps_three.cpp 

C_SRCS += \
../patterns/one_keeps_two.c \
../patterns/two_keeps_three.c 

OBJS += \
./patterns/one_keeps_two.o \
./patterns/two_keeps_three.o 

C_DEPS += \
./patterns/one_keeps_two.d \
./patterns/two_keeps_three.d 

CPP_DEPS += \
./patterns/one_keeps_two.d \
./patterns/two_keeps_three.d 


# Each subdirectory must supply rules for building sources it contributes
patterns/%.o: ../patterns/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

patterns/%.o: ../patterns/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -w -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


