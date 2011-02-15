#include "exception.h"
#include <sstream>

Exception::Exception(const std::string &message)
        :std::runtime_error(message)
{

}

Exception::~Exception()throw()
{

}


TimeOutException::TimeOutException(const std::string &message)
        :Exception(message)
{

}

DeviceNotFoundException::DeviceNotFoundException(const std::string &message)
        :Exception(message)
{

}

IOException::IOException(const std::string &message)
        :Exception(message)
{

}


void ThrowErrCode(int errCode, const std::string &where, int line)
{
    std::stringstream buffer;
    switch(errCode)
    {
    case -116:
        buffer << "TimeOut has elapsed in " << where << " line " << line;
        throw TimeOutException(buffer.str());
        break;
    case -3:
        buffer << "IO error in " << where << " line " << line;
        throw IOException(buffer.str());
        break;
    case -5:
         buffer << "Device not found or not responsing error in " << where << " line " << line;
        throw DeviceNotFoundException(buffer.str());
        break;
    default:
        if(errCode < 0)
        {
            buffer << "Unknown error in " << where << " line " << line << ". Error code = errCode ";
            throw Exception(buffer.str());
        }
    }
}
