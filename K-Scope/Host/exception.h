#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
#include <stdexcept>

class Exception :public std::runtime_error
{
public:
    Exception(const std::string &message);
    ~Exception() throw();

protected:

};

class TimeOutException :public Exception
{
public:
    TimeOutException(const std::string &message);
};

class DeviceNotFoundException :public Exception
{
public:
    DeviceNotFoundException(const std::string &message);
};

class IOException :public Exception
{
public:
    IOException(const std::string &message);
};

void ThrowErrCode(int errCode, const std::string &where, int line);

#define LOCATION __FILE__, __LINE__

#endif // EXCEPTION_H
