#ifndef __LOGGING__LOG_STREAM_HPP__
#define __LOGGING__LOG_STREAM_HPP__

class LogStream
{
   public:
    LogStream(
        LogLevel           level,
        const LogCategory& category,
        const char*        file,
        int                line,
        const char*        function
    )
        : _enabled{LogManager::instance().isEnabled(category, level)},
          _level{level},
          _category{category},
          _file{file},
          _line{line},
          _function{function}
    {
    }

    ~LogStream()
    {
        if (_enabled)
        {
            LogManager::instance().write(
                _level,
                _category,
                _file,
                _line,
                _function,
                std::move(_buffer)
            );
        }
    }

    template <typename T>
    LogStream& operator<<(const T& v)
    {
        if (_enabled)
        {
            _buffer +=
                QString::fromUtf8(QVariant::fromValue(v).toString().toUtf8());
        }
        return *this;
    }

   private:
    bool               _enabled;
    LogLevel           _level;
    const LogCategory& _category;
    const char*        _file;
    int                _line;
    const char*        _function;
    QString            _buffer;
};

#endif   // __LOGGING__LOG_STREAM_HPP__