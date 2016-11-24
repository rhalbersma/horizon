################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../c-files/Drivers.v2.4.cpp \
../c-files/FreePath.v2.4.cpp \
../c-files/Patterns.v2.4.cpp \
../c-files/alphabeta.v2.6.cpp \
../c-files/compact.v2.6.cpp \
../c-files/damend.v2.4.cpp \
../c-files/eval.v2.4.cpp \
../c-files/generate_legal_moves.v2.4.cpp \
../c-files/hash.v2.4.cpp \
../c-files/search_tree.v2.4.cpp \
../c-files/socket_handler.v2.4.cpp \
../c-files/voorpost.cpp \
../c-files/voorpost_procs.cpp 

OBJS += \
./c-files/Drivers.v2.4.o \
./c-files/FreePath.v2.4.o \
./c-files/Patterns.v2.4.o \
./c-files/alphabeta.v2.6.o \
./c-files/compact.v2.6.o \
./c-files/damend.v2.4.o \
./c-files/eval.v2.4.o \
./c-files/generate_legal_moves.v2.4.o \
./c-files/hash.v2.4.o \
./c-files/search_tree.v2.4.o \
./c-files/socket_handler.v2.4.o \
./c-files/voorpost.o \
./c-files/voorpost_procs.o 

CPP_DEPS += \
./c-files/Drivers.v2.4.d \
./c-files/FreePath.v2.4.d \
./c-files/Patterns.v2.4.d \
./c-files/alphabeta.v2.6.d \
./c-files/compact.v2.6.d \
./c-files/damend.v2.4.d \
./c-files/eval.v2.4.d \
./c-files/generate_legal_moves.v2.4.d \
./c-files/hash.v2.4.d \
./c-files/search_tree.v2.4.d \
./c-files/socket_handler.v2.4.d \
./c-files/voorpost.d \
./c-files/voorpost_procs.d 


# Each subdirectory must supply rules for building sources it contributes
c-files/%.o: ../c-files/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -w -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


