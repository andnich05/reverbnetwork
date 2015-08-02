/*
* QuantizerModule: Requantize the input signal to a new bitdepth
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

#ifndef QUANTIZERMODULE_H
#define QUANTIZERMODULE_H

class QuantizerModule
{
public:
	QuantizerModule(unsigned int quantization);
	~QuantizerModule();

	// Process sample by reference
	void processSample(double& sample) const;
	// Set the quantization in bits
	void setQuantization(const double& q);

private:
	// Calculate the appropriate factor depending on the quantization method
	void calculateFactor();

	unsigned int bitsToReset; // MAXQUANTIZATION - quantization
	long int mask; // (min.)32 bit mask
	double factor; // Right factor for the right quantization method

};

#endif // QUANTIZERMODULE_H