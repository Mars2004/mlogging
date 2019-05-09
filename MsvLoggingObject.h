/**************************************************************************************************//**
* @addtogroup	MLOGGING
* @{
******************************************************************************************************/

/**************************************************************************************************//**
* @file
* @brief			MarsTech Logging Object
* @details		Base logging object to inherit.
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


#ifndef MARSTECH_LOGGING_OBJECT_H
#define MARSTECH_LOGGING_OBJECT_H


#include "mlogging.h"


#ifdef MSV_ASSERTS
#if !defined(_DEBUG) && !defined(DEBUG) && !defined(NDEBUG)
#define NDEBUG
#endif // !defined(_DEBUG) && !defined(DEBUG) && !defined(NDEBUG)


MSV_DISABLE_ALL_WARNINGS

#include <cassert>

MSV_ENABLE_WARNINGS

#endif


/**************************************************************************************************//**
* @brief		Base logging object.
* @details	Implements base operations with logger. Inherit this class for easy work with loggers
*				in your child classes.
* @author	Martin Svoboda
* @date		09.11.2018
******************************************************************************************************/
class MsvLoggingObject
{
public:
	/**************************************************************************************************//**
	* @brief			Base logging object constructor.
	* @details		
	* @param[in]	spLogger		Smart pointer to @ref MsvLogger. It will be used for logging.
	* @see			MsvLogger
	******************************************************************************************************/
	MsvLoggingObject(std::shared_ptr<MsvLogger> spLogger):
		m_spLogger(spLogger)
	{
#ifdef MSV_ASSERTS
		assert(m_spLogger && "Logger is null!");
#endif
	}

	/**************************************************************************************************//**
	* @brief			Base logging object constructor.
	* @details		Gets logger with loggerName from spLoggerProvider.
	* @param[in]	spLoggerProvider	Smart pointer to @ref IMsvLoggerProvider to get logger.
	* @param[in]	loggerName			Logger name (it will used to get logger from spLoggerProvider).
	* @see			MsvLogger
	* @see			IMsvLoggerProvider
	******************************************************************************************************/
	MsvLoggingObject(std::shared_ptr<IMsvLoggerProvider> spLoggerProvider, const char* loggerName):
		m_spLogger(nullptr)
	{
		if (spLoggerProvider)
		{
			m_spLogger = spLoggerProvider->GetLogger(loggerName);
		}

#ifdef MSV_ASSERTS
		assert(spLoggerProvider && "Logger provider is null!");
		assert(m_spLogger && "Logger is null!");
#endif
	}

	/**************************************************************************************************//**
	* @brief		Virtual destructor.
	******************************************************************************************************/
	virtual ~MsvLoggingObject() {  }

	/**************************************************************************************************//**
	* @brief			Base logging object copy constructor.
	* @details		Gets copy of @ref MsvLogger from the origin object and use it its logger.
	* @param[in]	origin		Ogiginal object to copy.
	******************************************************************************************************/
	MsvLoggingObject(const MsvLoggingObject& origin):
		m_spLogger(origin.m_spLogger)
	{
	
	}
	
	/**************************************************************************************************//**
	* @brief			Base logging object assign operator.
	* @details		Gets copy of @ref MsvLogger from the origin object and use it its logger.
	* @param[in]	origin		Ogiginal object to assign.
	* @warning		Do not forget to call this assgin operator in your child defined assign operators.
	*					Childs default assign operator will use it automatically.
	******************************************************************************************************/
	MsvLoggingObject& operator= (const MsvLoggingObject& origin)
	{
		m_spLogger = origin.m_spLogger;
	}

protected:
	/**************************************************************************************************//**
	* @brief			Smart pointer to logger.
	* @details		It is used for logging in childs objects.
	* @see			MsvLogger
	******************************************************************************************************/
	std::shared_ptr<MsvLogger> m_spLogger;
};


#endif // !MARSTECH_LOGGING_OBJECT_H

/** @} */	//End of group MLOGGING.

