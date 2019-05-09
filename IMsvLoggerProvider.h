/**************************************************************************************************//**
* @defgroup		MLOGGING MarsTech Logging
* @brief			MarsTech logging macros and classes.
* @details		Contains implementation and all definitions of MarsTech Logging.
* @copyright	GNU General Public License (GPLv3).
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Logging Interface
* @details		MarsTech logger provider interface.
* @author		Martin Svoboda
* @date			09.11.2018
* @copyright	GNU General Public License (GPLv3).
* @warning		This file should not be included directly. It should be included in file which
*					defines @ref MsvLogger macro and logging macros.
*					Use file mlogging.h to include MLOGGING to your project or create your own one.
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


#ifndef MARSTECH_ILOGGER_PROVIDER_H
#define MARSTECH_ILOGGER_PROVIDER_H


/**************************************************************************************************//**
* @brief		Interface for logger provider.
* @details	Logger provider interface for getting loggers interfaces/imlementations.
* @author	Martin Svoboda
* @date		09.11.2018
******************************************************************************************************/
class IMsvLoggerProvider
{
public:
	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~IMsvLoggerProvider() {  }

	/**************************************************************************************************//**
	* @brief			Get logger by names.
	* @details		Returns interface/implementation of @ref MsvLogger with name in loggerName.
	* @param[in]	loggerName	Logger name which will be included to log file.
	* @returns		MsvLogger
	* @see			MsvLogger
	******************************************************************************************************/
	virtual std::shared_ptr<MsvLogger> GetLogger(const char* loggerName = "MsvLogger") const = 0;

	/**************************************************************************************************//**
	* @brief			Get logger by names.
	* @details		Returns interface/implementation of @ref MsvLogger with name in loggerName.
	* @param[in]	loggerName		Logger name which will be included to log file.
	* @param[in]	logFile			Log file name.
	* @param[in]	maxLogFileSize	Maximum size of one log file (in bytes).
	* @param[in]	maxLogFiles		Maximum number of log files (rotating logger, oldest file will be deleted).
	* @returns		MsvLogger
	* @see			MsvLogger
	******************************************************************************************************/
	virtual std::shared_ptr<MsvLogger> GetLogger(const char* loggerName, const char* logFile, int maxLogFileSize, int maxLogFiles) const = 0;
};


#endif // !MARSTECH_ILOGGER_PROVIDER_H

/** @} */	//End of group MLOGGING.
