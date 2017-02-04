################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../homerd-src/Display.cpp \
../homerd-src/GpioPin.cpp \
../homerd-src/KeyButton.cpp \
../homerd-src/KeyPanel.cpp \
../homerd-src/MenuDisplayVisitor.cpp \
../homerd-src/MenuLeaf.cpp \
../homerd-src/MenuMoveVisitor.cpp \
../homerd-src/Observer.cpp \
../homerd-src/SensorManager.cpp \
../homerd-src/SubMenu.cpp \
../homerd-src/Sysinfo.cpp \
../homerd-src/Winstar.cpp 

OBJS += \
./homerd-src/Display.o \
./homerd-src/GpioPin.o \
./homerd-src/KeyButton.o \
./homerd-src/KeyPanel.o \
./homerd-src/MenuDisplayVisitor.o \
./homerd-src/MenuLeaf.o \
./homerd-src/MenuMoveVisitor.o \
./homerd-src/Observer.o \
./homerd-src/SensorManager.o \
./homerd-src/SubMenu.o \
./homerd-src/Sysinfo.o \
./homerd-src/Winstar.o 

CPP_DEPS += \
./homerd-src/Display.d \
./homerd-src/GpioPin.d \
./homerd-src/KeyButton.d \
./homerd-src/KeyPanel.d \
./homerd-src/MenuDisplayVisitor.d \
./homerd-src/MenuLeaf.d \
./homerd-src/MenuMoveVisitor.d \
./homerd-src/Observer.d \
./homerd-src/SensorManager.d \
./homerd-src/SubMenu.d \
./homerd-src/Sysinfo.d \
./homerd-src/Winstar.d 


# Each subdirectory must supply rules for building sources it contributes
homerd-src/%.o: ../homerd-src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


