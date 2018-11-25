/**************************************************************************************************//**
* @addtogroup	MLOGGING
* @{
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


#ifndef MARSTECH_LOGGING_H
#define MARSTECH_LOGGING_H


#include "spdlog/spdlog.h"


/**************************************************************************************************//**
* @brief			Logger type definition.
* @details		Defines logger type. It is used in classes, functions etc. as logger type.
******************************************************************************************************/
typedef spdlog::logger MsvLogger;


#include "IMsvLoggerProvider.h"


/**************************************************************************************************//**
* @def			MSV_STRINGIFY
* @brief			Stringification macro.
* @see			MSV_STRINGIFY_
******************************************************************************************************/
#define MSV_STRINGIFY(stringify) MSV_STRINGIFY_(stringify)

/**************************************************************************************************//**
* @def			MSV_STRINGIFY_
* @brief			Stringification macro.
* @see			MSV_STRINGIFY
******************************************************************************************************/
#define MSV_STRINGIFY_(stringify) #stringify


/**************************************************************************************************//**
* @def			MSV_LOG(msvLogger, msvSeverity, msg, ...)
* @brief			Base log macro.
* @details		Base log macro used by log macros with severity.
* @param[in]	msvLogger	Logger.
* @param[in]	msvSeverity	Log severity.
* @param[in]	msg			Log formated message.
* @warning		Do not use this macro directly for logging. Use one of macros below.
* @see			MSV_LOG_DEBUG
* @see			MSV_LOG_INFO
* @see			MSV_LOG_WARN
* @see			MSV_LOG_ERROR
* @see			MSV_LOG_CRITICAL
******************************************************************************************************/
#define MSV_LOG(msvLogger, msvSeverity, msg, ...) if (msvLogger) { msvLogger->msvSeverity("["__FILE__ ":" MSV_STRINGIFY(__LINE__) "]: " msg, __VA_ARGS__); }

/**************************************************************************************************//**
* @def			MSV_LOG_DEBUG(msvLogger, msg, ...)
* @brief			Log debug.
* @details		Log debug message.
* @param[in]	msvLogger	Logger.
* @param[in]	msg			Log formated message.
* @note			Default logging severity is info - log messages from this macro won't be in log file.
******************************************************************************************************/
#define MSV_LOG_DEBUG(msvLogger, msg, ...) MSV_LOG(msvLogger, debug, msg, __VA_ARGS__)

/**************************************************************************************************//**
* @def			MSV_LOG_INFO(msvLogger, msg, ...)
* @brief			Log info.
* @details		Log info message.
* @param[in]	msvLogger	Logger.
* @param[in]	msg			Log formated message.
******************************************************************************************************/
#define MSV_LOG_INFO(msvLogger, msg, ...) MSV_LOG(msvLogger, info, msg, __VA_ARGS__)

/**************************************************************************************************//**
* @def			MSV_LOG_WARN(msvLogger, msg, ...)
* @brief			Log warning.
* @details		Log warning message.
* @param[in]	msvLogger	Logger.
* @param[in]	msg			Log formated message.
******************************************************************************************************/
#define MSV_LOG_WARN(msvLogger, msg, ...) MSV_LOG(msvLogger, warn, msg, __VA_ARGS__)

/**************************************************************************************************//**
* @def			MSV_LOG_ERROR(msvLogger, msg, ...)
* @brief			Log error.
* @details		Log error message.
* @param[in]	msvLogger	Logger.
* @param[in]	msg			Log formated message.
* @note			Default flush severity is error - message will be written to file immediatelly.
******************************************************************************************************/
#define MSV_LOG_ERROR(msvLogger, msg, ...) MSV_LOG(msvLogger, error, msg, __VA_ARGS__)

/**************************************************************************************************//**
* @def			MSV_LOG_CRITICAL(msvLogger, msg, ...)
* @brief			Log critical.
* @details		Log critical message.
* @param[in]	msvLogger	Logger.
* @param[in]	msg			Log formated message.
* @note			Default flush severity is error - message will be written to file immediatelly.
******************************************************************************************************/
#define MSV_LOG_CRITICAL(msvLogger, msg, ...) MSV_LOG(msvLogger, critical, msg, __VA_ARGS__)


#endif // !MARSTECH_LOGGING_H

/** @} */	//End of group MLOGGING.
