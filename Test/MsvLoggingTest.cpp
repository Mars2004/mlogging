#include "pch.h"

#include "../MsvSpdLogLoggerProvider.h"
#include "../MsvLoggingObject.h"

#include <fstream>
#include <regex>


const char* logFileName = "msvtestlogfile.txt";
const char* nullLogFileName = "msvnulltestlogfile.txt";
const char* loggerName = "MsvTest";


class TestLoggingObject:
	public MsvLoggingObject
{
public:
	TestLoggingObject(std::shared_ptr<MsvLogger> spLogger):
		MsvLoggingObject(spLogger)
	{
		
	}

	TestLoggingObject(std::shared_ptr<IMsvLoggerProvider> spLoggerProvider, const char* loggerName):
		MsvLoggingObject(spLoggerProvider, loggerName)
	{
		
	}

	std::shared_ptr<MsvLogger> GetLogger()
	{
		return m_spLogger;
	}
};

class LoggerProviderTests:
	public::testing::Test
{
public:
	LoggerProviderTests(std::shared_ptr<IMsvLoggerProvider> spLoggerProvider, const char* logFileName):
		m_spLoggerProvider(spLoggerProvider),
		m_spLogger(nullptr),
		m_logFileName(logFileName)
	{
		
	}

	virtual void SetUp()
	{
		deleteLogFile();

		m_spLogger = m_spLoggerProvider->GetLogger(loggerName);
		m_spLogger->set_level(spdlog::level::trace);

		EXPECT_NE(m_spLoggerProvider, nullptr);
		EXPECT_NE(m_spLogger, nullptr);
	}

	virtual void TearDown()
	{
		//clean all loggers before delete test log files
		m_spLogger.reset();
		spdlog::drop_all();
		spdlog::shutdown();

		deleteLogFile();
	}

	bool logFileExists()
	{
		std::ifstream logFile(m_logFileName);
		return logFile.good();
	}

	void deleteLogFile()
	{
		if (logFileExists())
		{
			EXPECT_EQ(remove(m_logFileName.c_str()), 0);
		}
	}

	//tested class
	std::shared_ptr<MsvLogger> m_spLogger;
	std::shared_ptr<IMsvLoggerProvider> m_spLoggerProvider;

protected:
	std::string m_logFileName;
};

class NullLoggerProviderTests:
	public LoggerProviderTests
{
public:
	NullLoggerProviderTests():
		LoggerProviderTests(std::shared_ptr<IMsvLoggerProvider>(new MsvNullLoggerProvider("", nullLogFileName)), nullLogFileName)
	{

	}
};

class SpdLogLoggerProviderTests:
	public LoggerProviderTests
{
public:
	SpdLogLoggerProviderTests():
		LoggerProviderTests(std::shared_ptr<IMsvLoggerProvider>(new MsvLoggerProvider("", logFileName)), logFileName)
	{

	}
};

typedef NullLoggerProviderTests MsvLoggingObjectTests;

TEST_F(NullLoggerProviderTests, ItShouldNotWriteLogToFile)
{
	MSV_LOG_DEBUG(m_spLogger, "message");
	MSV_LOG_INFO(m_spLogger, "message");
	MSV_LOG_WARN(m_spLogger, "message");
	MSV_LOG_ERROR(m_spLogger, "message");
	MSV_LOG_CRITICAL(m_spLogger, "message");

	MSV_LOG_DEBUG(m_spLogger, "message {}", 2);
	MSV_LOG_INFO(m_spLogger, "message {}", 2);
	MSV_LOG_WARN(m_spLogger, "message {}", 2);
	MSV_LOG_ERROR(m_spLogger, "message {}", 2);
	MSV_LOG_CRITICAL(m_spLogger, "message {}", 2);

	EXPECT_FALSE(logFileExists());
}

TEST_F(NullLoggerProviderTests, ItShouldBeSameLogger)
{
	std::shared_ptr<MsvLogger> spLogger = m_spLoggerProvider->GetLogger(loggerName);

	EXPECT_EQ(m_spLogger, spLogger);
}

//this test might have race conditions with below one
TEST_F(SpdLogLoggerProviderTests, ItShouldWriteLogToFile)
{
	MSV_LOG_DEBUG(m_spLogger, "message");
	MSV_LOG_INFO(m_spLogger, "message");
	MSV_LOG_WARN(m_spLogger, "message");
	MSV_LOG_ERROR(m_spLogger, "message");
	MSV_LOG_CRITICAL(m_spLogger, "message");

	MSV_LOG_DEBUG(m_spLogger, "message {}", 2);
	MSV_LOG_INFO(m_spLogger, "message {}", 2);
	MSV_LOG_WARN(m_spLogger, "message {}", 2);
	MSV_LOG_ERROR(m_spLogger, "message {}", 2);
	MSV_LOG_CRITICAL(m_spLogger, "message {}", 2);

	EXPECT_TRUE(logFileExists());

	std::ifstream logFile(logFileName);
	std::vector<std::string> lines;

	std::string line;
	while (std::getline(logFile, line))
	{
		lines.push_back(line);
	}
	
	EXPECT_EQ(lines.size(), 10);
	EXPECT_TRUE(std::regex_match(lines[0], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[debug\\] \\[[^\\]]*:[0-9]{1,4}\\]: message")));
	EXPECT_TRUE(std::regex_match(lines[1], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[info\\] \\[[^\\]]*:[0-9]{1,4}\\]: message")));
	EXPECT_TRUE(std::regex_match(lines[2], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[warning\\] \\[[^\\]]*:[0-9]{1,4}\\]: message")));
	EXPECT_TRUE(std::regex_match(lines[3], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[error\\] \\[[^\\]]*:[0-9]{1,4}\\]: message")));
	EXPECT_TRUE(std::regex_match(lines[4], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[critical\\] \\[[^\\]]*:[0-9]{1,4}\\]: message")));

	EXPECT_TRUE(std::regex_match(lines[5], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[debug\\] \\[[^\\]]*:[0-9]{1,4}\\]: message 2")));
	EXPECT_TRUE(std::regex_match(lines[6], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[info\\] \\[[^\\]]*:[0-9]{1,4}\\]: message 2")));
	EXPECT_TRUE(std::regex_match(lines[7], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[warning\\] \\[[^\\]]*:[0-9]{1,4}\\]: message 2")));
	EXPECT_TRUE(std::regex_match(lines[8], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[error\\] \\[[^\\]]*:[0-9]{1,4}\\]: message 2")));
	EXPECT_TRUE(std::regex_match(lines[9], std::regex("\\[[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{9}\\] \\[[0-9]*\\] \\[[0-9]*\\] \\[MsvTest\\] \\[critical\\] \\[[^\\]]*:[0-9]{1,4}\\]: message 2")));
}

TEST_F(SpdLogLoggerProviderTests, ItShouldBeSameLogger)
{
	std::shared_ptr<MsvLogger> spLogger = m_spLoggerProvider->GetLogger(loggerName);

	EXPECT_EQ(m_spLogger, spLogger);
}

TEST_F(MsvLoggingObjectTests, ItShouldBeSameLogger_InLoggingObjectByLoggerProvider)
{
	TestLoggingObject loggingObject(m_spLoggerProvider, loggerName);
	std::shared_ptr<MsvLogger> spLogger = loggingObject.GetLogger();

	EXPECT_EQ(m_spLogger, spLogger);
}

TEST_F(MsvLoggingObjectTests, ItShouldBeSameLogger_InLoggingObjectByLogger)
{
	TestLoggingObject loggingObject(m_spLogger);
	std::shared_ptr<MsvLogger> spLogger = loggingObject.GetLogger();

	EXPECT_EQ(m_spLogger, spLogger);
}
