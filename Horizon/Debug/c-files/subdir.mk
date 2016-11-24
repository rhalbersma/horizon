################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../c-files/Drivers.v2.4.cpp \
../c-files/FreePath.v2.4.cpp \
../c-files/OpeningBook.v2.4.cpp \
../c-files/Patterns.v2.4.cpp \
../c-files/alphabeta.v2.5.cpp \
../c-files/damend.v2.4.cpp \
../c-files/eval.v2.4.cpp \
../c-files/generate_legal_moves.v2.4.cpp \
../c-files/hash.v2.4.cpp \
../c-files/increase_depth.cpp \
../c-files/opening_book_self_play.cpp \
../c-files/search_tree.v2.4.cpp \
../c-files/socket_handler.v2.4.cpp \
../c-files/voorpost.cpp \
../c-files/voorpost_procs.cpp 

OBJS += \
./c-files/Drivers.v2.4.o \
./c-files/FreePath.v2.4.o \
./c-files/OpeningBook.v2.4.o \
./c-files/Patterns.v2.4.o \
./c-files/alphabeta.v2.5.o \
./c-files/damend.v2.4.o \
./c-files/eval.v2.4.o \
./c-files/generate_legal_moves.v2.4.o \
./c-files/hash.v2.4.o \
./c-files/increase_depth.o \
./c-files/opening_book_self_play.o \
./c-files/search_tree.v2.4.o \
./c-files/socket_handler.v2.4.o \
./c-files/voorpost.o \
./c-files/voorpost_procs.o 

CPP_DEPS += \
./c-files/Drivers.v2.4.d \
./c-files/FreePath.v2.4.d \
./c-files/OpeningBook.v2.4.d \
./c-files/Patterns.v2.4.d \
./c-files/alphabeta.v2.5.d \
./c-files/damend.v2.4.d \
./c-files/eval.v2.4.d \
./c-files/generate_legal_moves.v2.4.d \
./c-files/hash.v2.4.d \
./c-files/increase_depth.d \
./c-files/opening_book_self_play.d \
./c-files/search_tree.v2.4.d \
./c-files/socket_handler.v2.4.d \
./c-files/voorpost.d \
./c-files/voorpost_procs.d 


# Each subdirectory must supply rules for building sources it contributes
c-files/%.o: ../c-files/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -pthread -Wuninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


