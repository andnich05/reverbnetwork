#ifndef CONNECTIONMATRIX_H
#define CONNECTIONMATRIX_H

#include <vector>
#include <map>

class ConnectionMatrix
{
public:
	ConnectionMatrix();
	~ConnectionMatrix();

	// Get a reference to a connection vector
	inline const std::vector<std::vector<short>>& getModuleToModuleConnections() { return moduleToModuleConnections; };
	inline const std::vector<std::vector<short>>& getVstToModuleConnections() { return vstToModuleConnections; };
	inline const std::vector<short>& getModuleToVstConnections() { return moduleToVstConnections; };
	inline const std::vector<short>& getVstToVstConnections() { return vstToVstConnections; };

	// Establish a connection between an output of a module and the specified input of another module
	void setModuleToModuleConnection(const unsigned short& sourceModule, const unsigned short& destModule, const unsigned short& destModuleInput);

	// Establish a connection between a VST input and the specified input of another module
	void setVstToModuleConnection(const unsigned short& vstInput, const unsigned short& destModule, const unsigned short& destModuleInput);

	// Establish a connection between the output of a module and a VST output
	void setModuleToVstConnection(const unsigned short& sourceModule, const unsigned short& vstOutput);

	// Establish a connection between a VST input and a VST output
	void setVstToVstConnection(const unsigned short& vstInput, const unsigned short& vstOutput);

	// Reset all connections, all vector elements are set to -1
	void resetAllConnections();

private:
	// First array dimension holds the modules, second holds the inputs of that module
	// Each module can be connected to every other module, so the total number of inputs is MAXMODULENUMBER-1
	// The array value is the index of the inputSamples array (= module output) to which the module input is connected
	// E.g. [AP0][I2] = AP1 => means the output of AP1 is connected to the second input of 
	std::vector<std::vector<short>> moduleToModuleConnections;

	std::vector<std::vector<short>> vstToModuleConnections;

	// Holds to which module output or VST input a VST output is connected
	std::vector<short> moduleToVstConnections;

	std::vector<short> vstToVstConnections;

	// Mapped values for VST inputs and the output of all modules
	//std::vector<unsigned short> vstInputs;
	//std::vector<unsigned short> sourceModules;
};

#endif // CONNECTIONMATRIX_H