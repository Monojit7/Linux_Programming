/*

Chain of Responsibility pattern comes under behavioural pattern.
As the name suggest, it decouples the sender and receiver based
on the type of request. 

There will be chain of receiver object for a particular sender object.
If one receiver can not handle the request it passes on the request to
another receiver.


In the below example we are developing a logging mechanism in which
if the message log level is INFO then the message will be printed in
Standard Console , File Logger, and Error memory . If the message is
DEBUG then it will print in File Logger and Error Memory .If the message
is Error memory then it will be printed only in Error Memory.



                

*/

#include <iostream>
#include <list>
using namespace std;

class AbstractLogger
{

protected:
    int logLevel;

public:
    enum LOGLEVEL
    {
        INFO = 1,
        DEBUG = 2,
        ERROR = 3
    };

    AbstractLogger *mAbstractNextLogger = nullptr;

    void setNextLogger(AbstractLogger *mAbstractNextLogger)
    {
        this->mAbstractNextLogger = mAbstractNextLogger;
    }

    void logMessage(int level, string message)
    {
        if (this->logLevel <= level)
        {
            write(message);
        }

        if (this->mAbstractNextLogger != nullptr)
        {
           // this will call next logger logmessage .
           // suppose this object is error logger then
           // next logger filelogger . The filelogger 
           // logmessage will be called .
           
            this->mAbstractNextLogger->logMessage (level, message);
        }
    }

    virtual void write(string message) = 0;
};

class ConsoleLogger : public AbstractLogger
{
public:
    ConsoleLogger(int level)
    {
        this->logLevel = level;
    }

    virtual void write(string message)
    {
        cout << "Standard Console INFO output " << message << endl;
    }
};

class FileLogger : public AbstractLogger
{
public:
    FileLogger(int level)
    {
        this->logLevel = level;
    }

    void write(string message)
    {
        cout << "FileLogger Debug output " << message << endl;
    }
};

class ErrorLogger : public AbstractLogger
{
public:
    ErrorLogger(int level)
    {
        this->logLevel = level;
    }

    void write(string message)
    {
        cout << "ErrorLogger Error output " << message << endl;
    }
};

class ChainPatternDemo
{
public:
    AbstractLogger *getLogger()
    {
        AbstractLogger *mConsoleLogger = new ConsoleLogger(AbstractLogger::INFO);
        AbstractLogger *mFileLogger = new FileLogger(AbstractLogger::DEBUG);
        AbstractLogger *mErrorLogger = new ErrorLogger(AbstractLogger::ERROR);

        mErrorLogger->setNextLogger(mFileLogger);
        mFileLogger->setNextLogger(mConsoleLogger);
        mConsoleLogger->setNextLogger(nullptr);

        return mErrorLogger;
    }
};

int main()
{
    ChainPatternDemo *mChainPatternDemo = new ChainPatternDemo;
    AbstractLogger *mAbstractLogger = mChainPatternDemo->getLogger();
    mAbstractLogger->logMessage(AbstractLogger::INFO, " This is just an Information ");
    mAbstractLogger->logMessage(AbstractLogger::DEBUG, " This is a debug info ");
    mAbstractLogger->logMessage(AbstractLogger::ERROR, " This is an Error ");

    return 0;
}