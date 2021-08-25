#include "logger.h"

std::string logLevelToStr(logLevel level)
{
	if (level == logLevel::Debug)
		return "DEBUG";
	else if (level == logLevel::Info)
		return "INFO";
	else if (level == logLevel::Warn)
		return "WARN";
	else if (level == logLevel::Error)
		return "ERROR";
	else if (level == logLevel::Fatal)
		return "FATAL";
	return "";
}

std::string logLevelToStr(int level)
{
	if (level == 0)
		return "DEBUG";
	else if (level == 1)
		return "INFO";
	else if (level == 2)
		return "WARN";
	else if (level == 3)
		return "ERROR";
	else if (level == 4)
		return "FATAL";
	return "";
}

int logLevelToInt(logLevel level)
{
	if (level == logLevel::Debug)
		return 0;
	else if (level == logLevel::Info)
		return 1;
	else if (level == logLevel::Warn)
		return 2;
	else if (level == logLevel::Error)
		return 3;
	else if (level == logLevel::Fatal)
		return 4;
	return 0;
}

logLevel intToLogLevel(int level)
{
	if (level == 0)
		return logLevel::Debug;
	else if (level == 1)
		return logLevel::Info;
	else if (level == 2)
		return logLevel::Warn;
	else if (level == 3)
		return logLevel::Error;
	else if (level == 4)
		return logLevel::Fatal;
	return logLevel::Debug;
}


package::package(bool isNullptr, int MAX_PACKAGE_SIZE)
{
	setNull(isNullptr, MAX_PACKAGE_SIZE);
}

void package::setNull(bool isNullptr, int MAX_PACKAGE_SIZE)
{
	if (isNullptr)
		Package = nullptr;
	else
	{
		Package = std::make_shared <std::string>("");
		Package->reserve(MAX_PACKAGE_SIZE);
	}
	for (int i = 0; i < 5; i++)
		messageCounter[i] = 0;
}

package& package::operator= (const package& temp)
{
	if (this == &temp)
		return *this;
	Package = temp.Package;
	for (int i = 0; i < 5; i++)
		messageCounter[i] = temp.messageCounter[i];
	return *this;
}


bool package::isNull()
{
	if (Package == nullptr || *Package == "")
		return true;
	else return false;
}

void package::addMessage(logLevel level, std::string message)
{
	messageCounter[logLevelToInt(level)]++;
	*Package += message;
}

std::shared_ptr<std::string> package::getPackage()
{
	return Package;
}

int package::getMessageCounter(logLevel level)
{
	return messageCounter[logLevelToInt(level)];
}

int package::getMessageCounter(int level)
{
	if (level >= 0 && level < 5)
		return messageCounter[level];
	else return 0;
}



package logger::getRP()
{
	std::unique_lock<std::mutex> ul(m);
	ready.wait(ul, [=]() { return !readyPackage.isNull() || !isRunning; });
	package temp = readyPackage;
	readyPackage.setNull(true);
	return temp;
}

void logger::setRP()
{
	std::lock_guard<std::mutex> lg(m);
	readyPackage = waitingPackage;
	ready.notify_one();
}

logger::logger(std::string _computerName, std::string _filePath, int _MAX_PACKAGE_SIZE)
{
	computerName = _computerName;
	filePath = _filePath;
	MAX_PACKAGE_SIZE = _MAX_PACKAGE_SIZE;
	(*waitingPackage.getPackage()).reserve(MAX_PACKAGE_SIZE);
	logThr = std::thread(&logger::loggerOut, this);
}

logger::~logger()
{
	{
		std::lock_guard<std::mutex> lg(m);
		if (!waitingPackage.isNull())
			readyPackage = waitingPackage;
		isRunning = false;
	}
	ready.notify_all();
	logThr.join();
}

void logger::loggerOut()
{
	while (isRunning || !readyPackage.isNull())
	{
		package temp = getRP();
		if (temp.isNull())
			continue;
		std::ofstream fout(filePath, std::ios::app);

		std::string message = getTime() + " from " + computerName;

		for (int i = 0; i < message.size(); i++)
			fout << "#";
		fout << "\n";

		fout << message << "\n";

		int messageCounter = 0;
		for (int i = 0; i < 5; i++)
			messageCounter += temp.getMessageCounter(i);
		fout << messageCounter << " messages, including:\n";

		for (int i = 0; i < 5; i++)
			fout << temp.getMessageCounter(i) << " - " << logLevelToStr(i) << "\n";

		for (int i = 0; i < message.size(); i++)
			fout << "#";

		fout << "\n";

		fout << *temp.getPackage() << "\n";
		fout.close();
	}
}

void logger::add(logLevel level, std::string message)
{
	std::string completeMessage = "[" + logLevelToStr(level) + ", " + getTime() + "] " + message + "\n";
	if ((*waitingPackage.getPackage()).size() + completeMessage.size() > MAX_PACKAGE_SIZE && !waitingPackage.isNull())
	{
		setRP();
		waitingPackage.setNull(false, MAX_PACKAGE_SIZE);
	}
	waitingPackage.addMessage(level, completeMessage);
}

std::string getTime()
{
	time_t now = time(0);
	tm* t = localtime(&now);
	std::ostringstream temp;
	to2(temp, t->tm_mday);
	temp << ".";
	to2(temp, t->tm_mon + 1);
	temp << "." << 1900 + t->tm_year << " ";
	to2(temp, t->tm_hour);
	temp << ":";
	to2(temp, t->tm_min);
	temp << ":";
	to2(temp, t->tm_sec);
	return temp.str();
}

void to2(std::ostringstream& temp, int num)
{
	if (num >= 0 && num <= 9)
		temp << '0';
	temp << num;
}
