################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FileInterface.cpp \
../MTCPListener.cpp \
../MThread.cpp \
../Socket.cpp \
../TCPSocket.cpp \
../Tools.cpp \
../UDPSocket.cpp \
../test.cpp 

OBJS += \
./FileInterface.o \
./MTCPListener.o \
./MThread.o \
./Socket.o \
./TCPSocket.o \
./Tools.o \
./UDPSocket.o \
./test.o 

CPP_DEPS += \
./FileInterface.d \
./MTCPListener.d \
./MThread.d \
./Socket.d \
./TCPSocket.d \
./Tools.d \
./UDPSocket.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Ipthread -I"/home/user/workspace/Ex8_200574291/RemoteSystemsTempFiles/SocketUtils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


