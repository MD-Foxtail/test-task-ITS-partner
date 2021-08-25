#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <ctime>

#ifndef LOGGER
#define LOGGER

enum class logLevel { Debug, Info, Warn, Error, Fatal};
std::string logLevelToStr(logLevel level);
std::string logLevelToStr(int level);
int logLevelToInt(logLevel level);
logLevel intToLogLevel(int level);

class package
{
private:
	std::shared_ptr<std::string> Package;
	int messageCounter[5];
public:
	package(bool isNullptr = false, int MAX_PACKAGE_SIZE = 50000);
	package& operator= (const package& temp);
	void setNull(bool isNullptr = false, int MAX_PACKAGE_SIZE = 50000);
	bool isNull();
	void addMessage(logLevel level, std::string message);
	std::shared_ptr<std::string> getPackage();
	int getMessageCounter(logLevel level);
	int getMessageCounter(int level);
};

class logger
{
private:
	int MAX_PACKAGE_SIZE;
	std::string filePath;
	std::string computerName;

	std::thread logThr;
	package waitingPackage;
	package readyPackage;
	bool isRunning = true;
	std::condition_variable ready;
	std::mutex m;

	package getRP();
	void setRP();
public:

	logger(std::string _computerName, std::string _filePath, int _MAX_PACKAGE_SIZE = 50000);
	~logger();
	void loggerOut();
	void add(logLevel level, std::string message);
};

std::string getTime();
void to2(std::ostringstream& temp, int num);

#define LOG_START(computerName, filePath) logger Logger(computerName, filePath);
#define LOG_START_M(computerName, filePath, _MAX_PACKAGE_SIZE) logger Logger(computerName, filePath, _MAX_PACKAGE_SIZE);

#define LOG_CONNECT extern logger Logger;

#define LOG_DEBUG(message) Logger.add(logLevel::Debug, message);
#define LOG_INFO(message) Logger.add(logLevel::Info, message);
#define LOG_WARN(message) Logger.add(logLevel::Warn, message);
#define LOG_ERROR(message) Logger.add(logLevel::Error, message);
#define LOG_FATAL(message) Logger.add(logLevel::Fatal, message);

#endif
