# MarsTech Logging
 - [Instalation](#instalation)
	 - [Configuration](#configuration)
 - [MsvLogger](#msvlogger)
	 - [Log macros](#log-macros)
 - [Logger providers](#logger-providers)
 - [Logging object base](#logging-object-base)
 - [Source Code Documentation](#source-code-documentation)
 - [License](#license)

MarsTech Logging (MLOGGING) is a C++ project to make logging easier. It defines [log macros](#log-macros) and [logger providers](#logger-providers) for easier logging and creating loggers.

## Instalation
MLOGGING is header only project/library - there is no static or dynamic library. You can download repository and include header file mlogging.h to your project.

### Dependencies

 - [spdlog](https://github.com/gabime/spdlog)

### Configuration
MLOGGING uses [SPDLOG](https://github.com/gabime/spdlog) as default logger. So, download or clone [SPDLOG](https://github.com/gabime/spdlog) and set path to the directory where [SPDLOG](https://github.com/gabime/spdlog) is stored. The last step is to include mlogging.h header file.

If you want to use another logging library, create your own logger providers, change definition of [MsvLogger](#msvlogger) and create your own mlogging.h header file.

## MsvLogger
It is typedef which defines logger type. It is used in classes, functions etc. as logger type. Default logger is SPDLOG and it is defined as:
~~~cpp
typedef spdlog::logger MsvLogger;
~~~

### Log macros
There are log macros to easy create log message including filename and line number:

 - MSV_LOG_DEBUG(msvLogger, msg, ...)
 - MSV_LOG_INFO(msvLogger, msg, ...)
 - MSV_LOG_WARN(msvLogger, msg, ...)
 - MSV_LOG_ERROR(msvLogger, msg, ...)
 - MSV_LOG_CRITICAL(msvLogger, msg, ...)

**Example:**
~~~cpp
#include "mlogging/mlogging.h"

//called in some function or method:

MSV_LOG_INFO(m_spLogger, "message");
//prints: [DATE TIME] [processId] [threadId] [loggerName] [info] message

//for message formatting visit project SPDLOG
MSV_LOG_ERROR(m_spLogger, "message {}", 2);
//prints: [DATE TIME] [processId] [threadId] [loggerName] [error] message 2
~~~

## Logger providers
Logger provider is object which creates loggers. It is usefull for unit testing because you can inject NULL logger provider which creates only NULL loggers (they throw all log messages away).

**Example:**
~~~cpp
#include "mlogging/MsvSpdLogLoggerProvider.h"

class LoggingObject
{
	LoggingObject(std::shared_ptr<IMsvLoggerProvider> spLoggerProvider)
	{
		if (spLoggerProvider)
		{
			m_spLogger = spLoggerProvider->GetLogger("MyLoggerName");
		}
	}
	
protected:
	std::shared_ptr<MsvLogger> m_spLogger;
};
~~~

## Logging object base
There is also implementation of logging object base which implements base operations with loggers. It creates (or assigns) logger in its constructor and it also implements copy constructor and assign operator.
Just inherit from it and use m_spLogger member for logging in your child class.

**Example:**
~~~cpp
#include "mlogging/MsvLoggingObject.h"

class TestLoggingObject:
	public MsvLoggingObject
{
public:
	TestLoggingObject(std::shared_ptr<MsvLogger> spLogger):
		MsvLoggingObject(spLogger)
	{
	}

	TestLoggingObject(std::shared_ptr<IMsvLoggerProvider> spLoggerProvider):
		MsvLoggingObject(spLoggerProvider, "TestLogger")
	{
	}
	
	//Do not forget to call parents assign operator in your child defined assign operators.
	//Childs default assign operator will use it automatically.
	TestLoggingObject& operator= (const TestLoggingObject& origin)
	{
		this->MsvLoggingObject::operator=(origin);
		//...
	}
	
	//logger is accessible as protected member "m_spLogger" and can be used in log macros
};
~~~

## Source Code Documentation
You can find generated source code documentation at [https://www.marstech.cz/projects/mlogging/1.0.1/doc](https://www.marstech.cz/projects/mlogging/1.0.1/doc).

## License
This project is released under GNU General Public License version 3. If you can not or do not want to accept GNU GPLv3 license and you would like to use this project under another license, please contact me on [info@marstech.cz](mailto:info@marstech.cz) or visit [www.marstech.cz](https://www.marstech.cz/en).
