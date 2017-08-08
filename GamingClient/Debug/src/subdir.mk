################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TCPMSNClient.cpp \
../src/TCPMSNClientThread.cpp \
../src/TCPMessangerClientCLI.cpp \
../src/TCPMessangerClientTest.cpp 

OBJS += \
./src/TCPMSNClient.o \
./src/TCPMSNClientThread.o \
./src/TCPMessangerClientCLI.o \
./src/TCPMessangerClientTest.o 

CPP_DEPS += \
./src/TCPMSNClient.d \
./src/TCPMSNClientThread.d \
./src/TCPMessangerClientCLI.d \
./src/TCPMessangerClientTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/workspace/Ex8_200574291/RemoteSystemsTempFiles/SocketUtils" -I"/home/user/workspace/Ex8_200574291/RemoteSystemsTempFiles/GamingClient" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


