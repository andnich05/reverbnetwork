#include "ConnectionMatrix.h"
#include "ReverbNetworkDefines.h"
#include <string>


ConnectionMatrix::ConnectionMatrix() {

	// Initialize with fixed size
	resetAllConnections();

	// Mapping for all VST inputs and (one) BaseAPModule inputs
	// E.g.: 0 to 31 => Module inputs; 32 to 37 => VST inputs; 38 => Module output; 39 to 40 => VST ouputs
	// When using an object of this class you don't have to know the mapping

	// E.g. give me the mapped number for VST output 4 => vstOuputs[4]: 26
	/*unsigned short mapNumber = 0;
	for (unsigned short i = 0; i < MAXVSTINPUTS; ++i) {
		vstInputs.push_back(mapNumber);
		++mapNumber;
	}
	for (unsigned short i = 0; i < MAXMODULENUMBER; ++i) {
		sourceModules.push_back(mapNumber);
		++mapNumber;
	}*/
}

ConnectionMatrix::~ConnectionMatrix() {
	
}

// E.g. [AP0][I2] = AP1 => means the output of AP1 is connected to the second input (mapped value) of AP0

void ConnectionMatrix::setModuleToModuleConnection(const unsigned short& sourceModule, const unsigned short& destModule, const unsigned short& destModuleInput) {
	// Check if the input is already connected to a VST input; if so then disconnect it first
	/*if (vstToModuleConnections[destModule][destModuleInput] != -1) {
		vstToModuleConnections[destModule][destModuleInput] = -1;
	}*/
	moduleToModuleConnections[destModule][destModuleInput] = sourceModule;
}

void ConnectionMatrix::setVstToModuleConnection(const unsigned short& vstInput, const unsigned short& destModule, const unsigned short& destModuleInput) {
	// Check if the input is already connected to a module output; if so then disconnect it first
	/*if (moduleToModuleConnections[destModule][destModuleInput] != -1) {
		moduleToModuleConnections[destModule][destModuleInput] = -1;
	}*/
	vstToModuleConnections[destModule][destModuleInput] = vstInput;
}

void ConnectionMatrix::setModuleToVstConnection(const unsigned short& sourceModule, const unsigned short& vstOutput) {
	// Check if the VST output is already connected to a VST input; if so then disconnect it first
	/*if (vstToVstConnections[vstOutput] != -1) {
		vstToVstConnections[vstOutput] = -1;
	}*/
	moduleToVstConnections[vstOutput] = sourceModule;
}

void ConnectionMatrix::setVstToVstConnection(const unsigned short& vstInput, const unsigned short& vstOutput) {
	// Check if the VST output is already connected to a module output; if so then disconnect it first
	/*if (moduleToVstConnections[vstOutput] != -1) {
		moduleToVstConnections[vstOutput] = -1;
	}*/
	vstToVstConnections[vstOutput] = vstInput;
}

void ConnectionMatrix::disconnectModuleInput(const unsigned short& moduleNumber, const unsigned short& moduleInput) {
	moduleToModuleConnections[moduleNumber][moduleInput] = -1;
	vstToModuleConnections[moduleNumber][moduleInput] = -1;
}

void ConnectionMatrix::disconnectVstOutput(const unsigned short& vstOutput) {
	moduleToVstConnections[vstOutput] = -1;
	vstToVstConnections[vstOutput] = -1;
}

void ConnectionMatrix::resetAllConnections() {
	// -1 means there is no connection at this input
	moduleToModuleConnections.resize(MAXMODULENUMBER, std::vector<short>(MAXMODULEINPUTS, -1));
	vstToModuleConnections.resize(MAXMODULENUMBER, std::vector<short>(MAXMODULEINPUTS, -1));
	moduleToVstConnections.resize(MAXVSTOUTPUTS, -1);
	vstToVstConnections.resize(MAXVSTOUTPUTS, -1);
}