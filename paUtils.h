#ifndef UTILS_HPP

#define UTILS_HPP

#include <iostream>
#include <cstdlib>

void printDeviceInfo(PaDeviceInfo &d)
{
	std::cout << "---------------------------------------" << std::endl;
	std::cout << d.name << std::endl;
	std::cout << "---------------------------------------" << std::endl;

	std::cout << "Device Name: " << d.name << std::endl;
	std::cout << "Max Input Channels: " << d.maxInputChannels << std::endl;	
	std::cout << "Max Output Channels: " << d.maxOutputChannels << std::endl;	
	std::cout << "Default low input latency: " << d.defaultLowInputLatency << std::endl;
	std::cout << "Default low output latency: " << d.defaultLowOutputLatency << std::endl;
	std::cout << "Default high input latency: " << d.defaultHighInputLatency << std::endl;
	std::cout << "Default high output latency: " << d.defaultHighOutputLatency << std::endl;
	std::cout << "Default Sample-rate: " << d.defaultSampleRate << std::endl;
	std::cout << "-------------------------" << std::endl;
}

void PaErrorHandler()
{
    printf("Port Audio Error.aborting.\n");
	exit(-1);
}
#endif //UTILS_HPP
