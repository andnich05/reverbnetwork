#ifndef LOGGING_H
#define LOGGING_H

#include <sstream>

class Logging {
public:
	static void addToLog(const std::string& componentName, const std::string& message);

private:
	static bool writeStartLine;
};


#endif // LOGGING_H