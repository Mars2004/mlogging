

#ifndef MARSTECH_LOGGER_PROVIDER_MOCK_H
#define MARSTECH_LOGGER_PROVIDER_MOCK_H


#include <gmock\gmock.h>

#include "..\IMsvLoggerProvider.h"


class MsvLoggerProvider_Mock:
	public IMsvLoggerProvider
{
public:
	virtual ~MsvLoggerProvider_Mock() {  }

	MOCK_CONST_METHOD1(GetLogger, std::shared_ptr<MsvLogger>(const char* loggerName));
	MOCK_CONST_METHOD4(GetLogger, std::shared_ptr<MsvLogger>(const char* loggerName, const char* logFile, int maxLogFileSize, int maxLogFiles));
};


#endif // !MARSTECH_LOGGER_PROVIDER_MOCK_H
