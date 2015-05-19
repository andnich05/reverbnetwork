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
	for (unsigned short i = 0; i < MAXVSTINPUTS; ++i) {
		vstInputMapToMapped[i] = MAXMODULENUMBER + i;
		/*FILE* pFile = fopen("E:\\logVst.txt", "a");
		fprintf(pFile, "y(n): %s\n", std::to_string(vstInputMapToMapped[i]).c_str());
		fclose(pFile);*/
	}
	for (unsigned short i = 0; i < MAXVSTINPUTS; ++i) {
		vstInputMapFromMapped[MAXMODULENUMBER + i] = i;
	}
}

ConnectionMatrix::~ConnectionMatrix() {
	
}

// E.g. [AP0][I2] = AP1 => means the output of AP1 is connected to the second input (mapped value) of AP0

void ConnectionMatrix::setModuleToModuleConnection(const unsigned short& sourceModule, const unsigned short& destModule, const unsigned short& destModuleInput) {
	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "module to moudle");
	fprintf(pFile, "y(n): %s\n", std::to_string(sourceModule).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(destModule).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(destModuleInput).c_str());
	fclose(pFile);*/


	moduleInputConnections[destModule][destModuleInput] = sourceModule;
}

void ConnectionMatrix::setVstToModuleConnection(const unsigned short& vstInput, const unsigned short& destModule, const unsigned short& destModuleInput) {
	// Check if the input is already connected to a module output; if so then disconnect it first
	/*if (moduleToModuleConnections[destModule][destModuleInput] != -1) {
		moduleToModuleConnections[destModule][destModuleInput] = -1;
	}*/

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "vst to module");
	fprintf(pFile, "y(n): %s\n", std::to_string(vstInput).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(destModule).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(destModuleInput).c_str());
	fclose(pFile);*/

	moduleInputConnections[destModule][destModuleInput] = vstInputMapToMapped[vstInput];
}

void ConnectionMatrix::setModuleToVstConnection(const unsigned short& sourceModule, const unsigned short& vstOutput) {
	// Check if the VST output is already connected to a VST input; if so then disconnect it first
	/*if (vstToVstConnections[vstOutput] != -1) {
		vstToVstConnections[vstOutput] = -1;
	}*/

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "module to vst");
	fprintf(pFile, "y(n): %s\n", std::to_string(sourceModule).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(vstOutput).c_str());
	fclose(pFile);*/

	vstOutputConnections[vstOutput] = sourceModule;
}

void ConnectionMatrix::setVstToVstConnection(const unsigned short& vstInput, const unsigned short& vstOutput) {
	// Check if the VST output is already connected to a module output; if so then disconnect it first
	/*if (moduleToVstConnections[vstOutput] != -1) {
		moduleToVstConnections[vstOutput] = -1;
	}*/

	/*FILE* pFile = fopen("E:\\logVst.txt", "a");
	fprintf(pFile, "y(n): %s\n", "vst to vst");
	fprintf(pFile, "y(n): %s\n", std::to_string(vstInput).c_str());
	fprintf(pFile, "y(n): %s\n", std::to_string(vstOutput).c_str());
	fclose(pFile);*/

	vstOutputConnections[vstOutput] = vstInputMapToMapped[vstInput];
}

void ConnectionMatrix::disconnectModuleInput(const unsigned short& moduleNumber, const unsigned short& moduleInput) {
	moduleInputConnections[moduleNumber][moduleInput] = -1;
}

void ConnectionMatrix::disconnectVstOutput(const unsigned short& vstOutput) {
	vstOutputConnections[vstOutput] = -1;
}

void ConnectionMatrix::resetAllConnections() {
	// -1 means there is no connection at this input
	moduleInputConnections.clear();
	vstOutputConnections.clear();
	moduleInputConnections.resize(MAXMODULENUMBER, std::vector<short>(MAXMODULEINPUTS, -1));
	vstOutputConnections.resize(MAXVSTOUTPUTS, -1);
}