################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/EmulatedDevices.cpp \
../src/EmulatedHw.cpp \
../src/GLUTUtilities.cpp \
../src/Globals.cpp \
../src/HomerEmulator.cpp \
../src/main.cpp 

OBJS += \
./src/EmulatedDevices.o \
./src/EmulatedHw.o \
./src/GLUTUtilities.o \
./src/Globals.o \
./src/HomerEmulator.o \
./src/main.o 

CPP_DEPS += \
./src/EmulatedDevices.d \
./src/EmulatedHw.d \
./src/GLUTUtilities.d \
./src/Globals.d \
./src/HomerEmulator.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


