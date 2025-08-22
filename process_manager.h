#pragma once

#include <windows.h>
#include <vector>
#include <string>

struct HandleInfo {
    HANDLE handle;
    std::string type;
    std::string name;
};

struct ProcessInfo {
    DWORD processId;
    std::string processName;
    std::vector<HandleInfo> handles;
};

class ProcessManager {
public:
    ProcessManager();
    ~ProcessManager();

    std::vector<ProcessInfo> findRbxProcesses();
    void closeProcessHandles(ProcessInfo& process);

private:
    std::vector<HandleInfo> getProcessHandles(DWORD processId);
    std::string UnicodeToAnsi(void* pUnicodeString);
    std::string GetObjectName(HANDLE hObject, HANDLE hProcess);
    std::string GetObjectTypeName(HANDLE hObject, HANDLE hProcess);
    
    // Caching
    std::vector<ProcessInfo> m_cachedProcesses;
    DWORD m_lastCacheTime;
    static const DWORD CACHE_DURATION = 1000; // 1 second cache
    bool isCacheValid() const;
};
