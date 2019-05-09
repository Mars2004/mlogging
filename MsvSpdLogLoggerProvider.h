/**************************************************************************************************//**
* @addtogroup	MLOGGING
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Logging SPDLOG Implementation
* @details		Implementation of SPDLOG logger providers.
* @author		Martin Svoboda
* @date			09.11.2018
* @copyright	GNU General Public License (GPLv3).
* @see			MsvLogger
******************************************************************************************************/


/*
This file is part of MarsTech Logging.

MarsTech Dependency Injection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

MarsTech Promise Like Syntax is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef MARSTECH_SPDLOG_LOGGER_PROVIDER_H
#define MARSTECH_SPDLOG_LOGGER_PROVIDER_H


#include "mlogging.h"

MSV_DISABLE_ALL_WARNINGS

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/null_sink.h"

#include <mutex>

MSV_ENABLE_WARNINGS


/**************************************************************************************************//**
* @brief		Logger provider implementation.
* @details	Implements logger provider for SPDLOG logging library.
* @author	Martin Svoboda
* @date		09.11.2018
******************************************************************************************************/
class MsvLoggerProvider:
	public IMsvLoggerProvider
{
public:
	/**************************************************************************************************//**
	* @brief			SPDLOG logger provider constructor.
	* @param[in]	logFolder		Logger folder where to store all log files of loggers created by this provider.
	* @param[in]	logFile			Log file name.
	* @param[in]	maxLogFileSize	Maximum size of one log file (in bytes).
	* @param[in]	maxLogFiles		Maximum size of log files (rotating logger, oldest file will be deleted).
	* @warning		Logging folder must be created before the first logger. SPDLOG and MLOGGING do not
	*					create log folder by itself.
	*					Please, create this folder during instalation or before getting the first logger.
	* @see			MsvLogger
	******************************************************************************************************/
	MsvLoggerProvider(const char* logFolder = "", const char* logFile = "msvlog.txt", int maxLogFileSize = 10485760, int maxLogFiles = 3):
		m_logFolder(logFolder),
		m_logFile(logFile),
		m_maxLogFileSize(maxLogFileSize),
		m_maxLogFiles(maxLogFiles)
	{
	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvLoggerProvider() {  }

	/**************************************************************************************************//**
	* @copydoc IMsvLoggerProvider::GetLogger(const char*) const
	******************************************************************************************************/
	virtual std::shared_ptr<MsvLogger> GetLogger(const char* loggerName = "MsvLogger") const override
	{
		return GetLogger(loggerName, m_logFile.c_str(), m_maxLogFileSize, m_maxLogFiles);
	}

	/**************************************************************************************************//**
	* @copydoc IMsvLoggerProvider::GetLogger(const char*, const char*, int, int) const
	******************************************************************************************************/
	virtual std::shared_ptr<MsvLogger> GetLogger(const char* loggerName, const char* logFile, int maxLogFileSize, int maxLogFiles) const override
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		std::shared_ptr<MsvLogger> spLogger(nullptr);

		try
		{
			//try to get logger if already exists
			spLogger = spdlog::get(loggerName);

			if (!spLogger)
			{
				//logger does not exists -> create new one
				spLogger = spdlog::rotating_logger_mt(loggerName, m_logFolder + logFile, maxLogFileSize, maxLogFiles);

				spLogger->set_level(spdlog::level::info);
				//[2014-31-10 23:46:59.256789123] [processId] [threadId] [loggerName] [severity] "message"
				//[2014-31-10 23:46:59.256789123] [1111] [2222] [MsvLogger] [info] Some message
				spLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%F] [%P] [%t] [%n] [%l] %v");
				spLogger->flush_on(spdlog::level::err);
			}
		}
		catch (...)
		{
			//exception caught -> set nullptr and continue
			spLogger = nullptr;
		}

		return spLogger;
	}

protected:
	/**************************************************************************************************//**
	* @brief		Locking object.
	* @details	Synchronization object for locking. Mutable to be usable even in const methods.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;

	/**************************************************************************************************//**
	* @brief		Log folder.
	* @details	Log folder for all loggers created by this logger provider.
	******************************************************************************************************/
	std::string m_logFolder;

	/**************************************************************************************************//**
	* @brief		Default log filename.
	* @details	When no log filename is specified, logger will log to this file.
	******************************************************************************************************/
	std::string m_logFile;

	/**************************************************************************************************//**
	* @brief		Default log file size.
	******************************************************************************************************/
	int m_maxLogFileSize;

	/**************************************************************************************************//**
	* @brief		Default maximum count of log files.
	******************************************************************************************************/
	int m_maxLogFiles;
};


/**************************************************************************************************//**
* @brief		NULL logger provider implementation.
* @details	Implements NULL logger provider for SPDLOG logging library. NULL logger does not create
*				log file and any logs. NULL logger throws away all log messages. It is primary used in
*				tests.
* @author	Martin Svoboda
* @date		09.11.2018
* @see		MsvLoggerProvider
******************************************************************************************************/
class MsvNullLoggerProvider:
	public IMsvLoggerProvider
{
public:
	/**************************************************************************************************//**
	* @brief			NULL logger provider constructor.
	* @details		NULL logger throws away all log messages. So, all parameters are no important and their
	*					ignored. Parameters are just for easy usage NULL logger as SPDLOG @ref MsvLoggerProvider.
	* @param[in]	logFolder		Logger folder where to store all log files of loggers created by this provider.
	* @param[in]	logFile			Log file name.
	* @param[in]	maxLogFileSize	Maximum size of one log file (in bytes).
	* @param[in]	maxLogFiles		Maximum size of log files (rotating logger, oldest file will be deleted).
	* @see			MsvLogger
	* @see			MsvLoggerProvider
	******************************************************************************************************/
	MsvNullLoggerProvider(const char* logFolder = "", const char* logFile = "msvlog.txt", int maxLogFileSize = 10485760, int maxLogFiles = 3)
	{
		//unused parameters
		(void)logFolder, logFile, maxLogFileSize, maxLogFiles;
	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvNullLoggerProvider() {  }

	/**************************************************************************************************//**
	* @copydoc IMsvLoggerProvider::GetLogger(const char*) const
	******************************************************************************************************/
	virtual std::shared_ptr<MsvLogger> GetLogger(const char* loggerName = "MsvLogger") const override
	{
		return GetLogger(loggerName, "", 0, 0);
	}

	/**************************************************************************************************//**
	* @copydoc IMsvLoggerProvider::GetLogger(const char*, const char*, int, int) const
	******************************************************************************************************/
	virtual std::shared_ptr<MsvLogger> GetLogger(const char* loggerName, const char* logFile, int maxLogFileSize, int maxLogFiles) const override
	{
		//unused parameters
		(void)logFile, maxLogFileSize, maxLogFiles;

		std::lock_guard<std::recursive_mutex> lock(m_lock);

		std::shared_ptr<MsvLogger> spLogger(nullptr);

		try
		{
			//try to get logger if already exists
			spLogger = spdlog::get(loggerName);

			if (!spLogger)
			{
				//logger does not exists -> create new one
				spLogger = spdlog::create<spdlog::sinks::null_sink_mt>(loggerName);
			}
		}
		catch (...)
		{
			//exception caught -> set nullptr and continue
			spLogger = nullptr;
		}

		return spLogger;
	}

protected:
	/**************************************************************************************************//**
	* @brief		Locking object.
	* @details	Synchronization object for locking. Mutable to be usable even in const methods.
	******************************************************************************************************/
	mutable std::recursive_mutex m_lock;
};


#endif // !MARSTECH_SPDLOG_LOGGER_PROVIDER_H

/** @} */	//End of group MLOGGING.
