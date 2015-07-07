#include "Logging.h"
#include <fstream>
#include <ctime>
#include <mutex>

#ifdef _WIN32
#include <ShlObj.h>
#include <Windows.h>
#endif

bool Logging::writeStartLine = true;

void Logging::addToLog(const std::string& componentName, const std::string& message) {
#ifdef _WIN32
	wchar_t* path = 0;
	// Get AppData folder path
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
	std::wstringstream filePath;
	filePath << path << L"\\ReverbNetwork.log";
	//std::wstringstream directoryPath;
	//directoryPath << path << L"\\HochschuleRheiMain\\ReverbNetwork";
	std::ofstream file;

	std::mutex mutex;
	mutex.lock();
	file.open(filePath.str(), std::ios_base::app | std::ios_base::out);
	if (writeStartLine) {
		file << "-----ReverbNetwork Build: " << __DATE__ << " " << __TIME__ << "-----" << std::endl;
		writeStartLine = false;
	}
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
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

	CoTaskMemFree(static_cast<void*>(path));
#endif
}