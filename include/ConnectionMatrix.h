/*
* ConnectionMatrix: Connection management between all modules/Vst I/O
*
* Copyright (C) 2015  Andrej Nichelmann
*                     Klaus Michael Indlekofer
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
	void setModuleToModuleConnection(unsigned short sourceModule, unsigned short destModule, unsigned short destModuleInput);

	// Establish a connection between a VST input and the specified input of another module
	void setVstToModuleConnection(unsigned short vstInput, unsigned short destModule, unsigned short destModuleInput);

	// Establish a connection between the output of a module and a VST output
	void setModuleToVstConnection(unsigned short sourceModule, unsigned short vstOutput);

	// Establish a connection between a VST input and a VST output
	void setVstToVstConnection(unsigned short vstInput, unsigned short vstOutput);

	// Disconnect a module input
	void disconnectModuleInput(unsigned short moduleNumber, unsigned short moduleInput);

	// Disconnect a VST output
	void disconnectVstOutput(unsigned short vstOutput);

	// Reset all connections, all vector elements are set to -1
	void resetAllConnections();

	// Map VST input
	inline short unmapVstInput(short mappedVstInput) { mappedVstInput = vstInputMapFromMapped[mappedVstInput]; return mappedVstInput; }

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
