/*
* Logging: Log output
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

#include "Logging.h"
#include <fstream>
#include <ctime>
#include <mutex>
#include <sstream>

// Log output only on Windows
#ifdef _WIN32
#include <ShlObj.h>
#include <Windows.h>
#endif

bool Logging::writeStartLine = true;

void Logging::addToLog(const std::string& componentName, const std::string& message) {
#ifdef _WIN32
	wchar_t* path = 0;
	// Get User/AppData/Roaming folder path
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
	std::wstringstream filePath;
	filePath << path << L"\\ReverbNetwork.log"; // Output file name
	std::ofstream file;

	// Lock the output file
	std::mutex mutex;
	mutex.lock();
	file.open(filePath.str(), std::ios_base::app | std::ios_base::out);
	if (writeStartLine) {
		// Write start line at first program start with build data and time
		file << "-----ReverbNetwork Build: " << __DATE__ << " " << __TIME__ << "-----" << std::endl;
		writeStartLine = false;
	}
	time_t t = time(0);   // get current time
	struct tm * now = localtime(&t);
	// Format it
	file << "[" << (now->tm_year + 1900) << '-'
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << " "
		<< now->tm_hour << ":"
		<< now->tm_min << ":"
		<< now->tm_sec << "] "
		<< componentName << ": "
		<< message << std::endl;
	file.close();
	mutex.unlock();

	CoTaskMemFree(static_cast<void*>(path)); // Free memory
#endif
}