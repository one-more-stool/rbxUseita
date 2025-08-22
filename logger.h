#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    enum Level {
        LOG_DEBUG = 0,
        LOG_INFO = 1,
        LOG_WARNING = 2,
        LOG_ERROR = 3
    };

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void setLevel(Level level) { m_level = level; }
    void setLogFile(const std::string& filename) { m_logFile = filename; }
    
    void debug(const std::string& message) { log(LOG_DEBUG, message); }
    void info(const std::string& message) { log(LOG_INFO, message); }
    void warning(const std::string& message) { log(LOG_WARNING, message); }
    void error(const std::string& message) { log(LOG_ERROR, message); }
    
    void log(Level level, const std::string& message);

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Level m_level = LOG_INFO;
    std::string m_logFile = "rbxUseita.log";
    std::mutex m_mutex;
    
    std::string getLevelString(Level level) const;
    std::string getCurrentTime() const;
};
