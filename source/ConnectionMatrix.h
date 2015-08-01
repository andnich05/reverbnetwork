#ifndef CONNECTIONMATRIX_H
#define CONNECTIONMATRIX_H

#include <vector>
#include <unordered_map>

// !!! The connection matrix is used only for connecting modules/VST inputs to VST outputs at the moment
// Everything else is always connected and is controlable through the gain factor (0.0 => no connection)

class ConnectionMatrix
{
public:
	ConnectionMatrix();
	~ConnectionMatrix();

	// Get a reference to a connection vector
	inline const std::vector<std::vector<short>>& getModuleInputConnections() { return moduleInputConnections; };
	inline const std::vector<short>& getVstOutputConnections() { return vstOutputConnections; };

	// Establish a connection between an output of a module and the specified input of another module
	void setModuleToModuleConnection(const unsigned short& sourceModule, const unsigned short& destModule, const unsigned short& destModuleInput);

	// Establish a connection between a VST input and the specified input of another module
	void setVstToModuleConnection(const unsigned short& vstInput, const unsigned short& destModule, const unsigned short& destModuleInput);

	// Establish a connection between the output of a module and a VST output
	void setModuleToVstConnection(const unsigned short& sourceModule, const unsigned short& vstOutput);

	// Establish a connection between a VST input and a VST output
	void setVstToVstConnection(const unsigned short& vstInput, const unsigned short& vstOutput);

	// Disconnect a module input
	void disconnectModuleInput(const unsigned short& moduleNumber, const unsigned short& moduleInput);

	// Disconnect a VST output
	void disconnectVstOutput(const unsigned short& vstOutput);

	// Reset all connections, all vector elements are set to -1
	void resetAllConnections();

	// Map VST input
	inline short& unmapVstInput(short& mappedVstInput) { mappedVstInput = vstInputMapFromMapped[mappedVstInput]; return mappedVstInput; }

private:
	// First array dimension holds the modules, second holds the inputs of that module
	// Each module can be connected to any other module, so the total number of inputs is MAXMODULENUMBER-1
	// The array value is the index of the inputSamples array (= module output) to which the module input is connected
	// E.g. [AP0][I2] = AP1 => means the output of AP1 is connected to the second input of AP0
	// Vector value: 0 to MAXMODULENUMBER-1 => Module outputs; MAXMODULENUMBER to MAXMODULENUMBER+MAXVSTINPUTS-1 => VST inputs; -1 => Not connected
	std::vector<std::vector<short>> moduleInputConnections;
	std::vector<short> vstOutputConnections;

	// Mapping for the VST inputs which come after the module outputs
	std::unordered_map<unsigned short, unsigned short> vstInputMapToMapped;
	std::unordered_map<unsigned short, unsigned short> vstInputMapFromMapped;

	// Mapped values for VST inputs and the output of all modules
	//std::vector<unsigned short> vstInputs;
	//std::vector<unsigned short> sourceModules;
};

#endif // CONNECTIONMATRIX_H