#ifndef LOGGING_H
#define LOGGING_H

#include <string>

class Logging {
public:
	// Add a text line to the Log
	static void addToLog(const std::string& componentName, const std::string& message);

private:
	static bool writeStartLine; // Write a start line with build info at program start
};


#endif // LOGGING_H