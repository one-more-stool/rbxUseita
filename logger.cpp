#include "logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <windows.h>

void Logger::log(Level level, const std::string& message) {
    if (level < m_level) return;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::string timestamp = getCurrentTime();
    std::string levelStr = getLevelString(level);
    std::string logMessage = timestamp + " [" + levelStr + "] " + message + "\n";
    
    // Output to console
    std::cout << logMessage;
    
    // Output to debugger
    OutputDebugStringA(logMessage.c_str());
    
    // Write to file if specified
    if (!m_logFile.empty()) {
        std::ofstream file(m_logFile, std::ios::app);
        if (file.is_open()) {
            file << logMessage;
            file.close();
        }
    }
}

std::string Logger::getLevelString(Level level) const {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARN";
        case LOG_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    std::tm tm_buf;
    localtime_s(&tm_buf, &time_t);
    ss << std::put_time(&tm_buf, "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}
