/*
 ============================================================================
 Name        : HelloWorldLog4cplus.cpp
 Author      : Daniele Colombo
 Version     :
 Copyright   : GPL 2
 Description : Hello World in C++,
 ============================================================================
 */

#include <iostream>
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

using namespace std;
using namespace log4cplus;

int logall() {
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	LOG4CPLUS_TRACE(logger, "printMessages()");
	LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
	LOG4CPLUS_INFO(logger, "This is a INFO message");
	LOG4CPLUS_WARN(logger, "This is a WARN message");
	LOG4CPLUS_ERROR(logger, "This is a ERROR message");
	LOG4CPLUS_FATAL(logger, "This is a FATAL message");

}
int main(void) {
	cout << "Hello World" << endl; /* prints Hello World */
    initialize();
    BasicConfigurator config;
    config.configure();
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("main"));
	logger.setLogLevel(DEBUG_LOG_LEVEL);
	logall();
	return 0;
}
