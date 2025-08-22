#include "process_manager.h"
#include "scoped_handle.h"
#include <tlhelp32.h>
#include <iostream>

// Windows NT API includes
#define WIN32_NO_STATUS
#include <winternl.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

#pragma comment(lib, "ntdll.lib")

// Undocumented structures for handle enumeration
typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG ProcessId;
    UCHAR ObjectTypeNumber;
    UCHAR Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX {
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_INFORMATION Information[1];
} SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

typedef struct _OBJECT_NAME_INFORMATION {
    UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

typedef struct _OBJECT_TYPE_INFORMATION {
    UNICODE_STRING TypeName;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

// Define constants
#define SystemHandleInformation 16
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

// Function prototypes (already declared in winternl.h, so we don't need to redeclare them)

ProcessManager::ProcessManager() 
    : m_lastCacheTime(0) {
}

ProcessManager::~ProcessManager() {
}

std::vector<ProcessInfo> ProcessManager::findRbxProcesses() {
    // Check cache first
    if (isCacheValid()) {
        return m_cachedProcesses;
    }
    
    std::vector<ProcessInfo> processes;
    processes.reserve(10); // Pre-allocate memory
    
    ScopedHandle hSnapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (!hSnapshot.isValid()) {
        return processes;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            std::string processName(pe32.szExeFile);
            if (processName == "RobloxPlayerBeta.exe") {
                ProcessInfo processInfo;
                processInfo.processId = pe32.th32ProcessID;
                processInfo.processName = processName;
                processInfo.handles = getProcessHandles(pe32.th32ProcessID);
                processes.push_back(processInfo);
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    
    // Update cache
    m_cachedProcesses = processes;
    m_lastCacheTime = GetTickCount();
    
    return processes;
}

bool ProcessManager::isCacheValid() const {
    return (GetTickCount() - m_lastCacheTime) < CACHE_DURATION;
}

std::vector<HandleInfo> ProcessManager::getProcessHandles(DWORD processId) {
    std::vector<HandleInfo> handles;
    
    ULONG bufferSize = 0x10000;
    PSYSTEM_HANDLE_INFORMATION_EX pHandleInfo = nullptr;
    NTSTATUS status;

    do {
        if (pHandleInfo) {
            free(pHandleInfo);
        }
        
        pHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)malloc(bufferSize);
        if (!pHandleInfo) {
            return handles;
        }

        status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, 
                                        pHandleInfo, bufferSize, &bufferSize);
    } while (status == STATUS_INFO_LENGTH_MISMATCH);

    if (!NT_SUCCESS(status)) {
        free(pHandleInfo);
        return handles;
    }

    HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (!hProcess) {
        free(pHandleInfo);
        return handles;
    }

    for (ULONG i = 0; i < pHandleInfo->NumberOfHandles; i++) {
        SYSTEM_HANDLE_INFORMATION& handleInfo = pHandleInfo->Information[i];
        
        if (handleInfo.ProcessId != processId) {
            continue;
        }

        HANDLE hDuplicate = nullptr;
        if (!DuplicateHandle(hProcess, (HANDLE)(uintptr_t)handleInfo.Handle, 
                           GetCurrentProcess(), &hDuplicate, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
            continue;
        }

        std::string typeName = GetObjectTypeName(hDuplicate, hProcess);
        if (typeName == "Event") {
            std::string objectName = GetObjectName(hDuplicate, hProcess);
            if (!objectName.empty() && objectName.find("ROBLOX_singletonEvent") != std::string::npos) {
                HandleInfo handle;
                handle.handle = (HANDLE)(uintptr_t)handleInfo.Handle;
                handle.type = typeName;
                handle.name = objectName;
                handles.push_back(handle);
            }
        }

        CloseHandle(hDuplicate);
    }

    CloseHandle(hProcess);
    free(pHandleInfo);
    return handles;
}

std::string ProcessManager::UnicodeToAnsi(void* pUnicodeString) {
    PUNICODE_STRING pUnicode = (PUNICODE_STRING)pUnicodeString;
    if (!pUnicode || pUnicode->Length == 0) {
        return "";
    }
    
    int ansiLength = WideCharToMultiByte(CP_ACP, 0, pUnicode->Buffer, 
                                       pUnicode->Length / sizeof(WCHAR), 
                                       nullptr, 0, nullptr, nullptr);
    
    if (ansiLength == 0) {
        return "";
    }
    
    std::string result(ansiLength, '\0');
    WideCharToMultiByte(CP_ACP, 0, pUnicode->Buffer, 
                       pUnicode->Length / sizeof(WCHAR), 
                       &result[0], ansiLength, nullptr, nullptr);
    
    return result;
}

std::string ProcessManager::GetObjectName(HANDLE hObject, HANDLE hProcess) {
    ULONG bufferSize = 256;
    POBJECT_NAME_INFORMATION pObjectName = nullptr;
    NTSTATUS status;

    do {
        if (pObjectName) {
            free(pObjectName);
        }
        
        pObjectName = (POBJECT_NAME_INFORMATION)malloc(bufferSize);
        if (!pObjectName) {
            return "";
        }

        status = NtQueryObject(hObject, (OBJECT_INFORMATION_CLASS)ObjectNameInformation, 
                             pObjectName, bufferSize, &bufferSize);
    } while (status == STATUS_INFO_LENGTH_MISMATCH);

    std::string result;
    if (NT_SUCCESS(status) && pObjectName->Name.Length > 0) {
        result = UnicodeToAnsi((void*)&pObjectName->Name);
    }

    free(pObjectName);
    return result;
}

std::string ProcessManager::GetObjectTypeName(HANDLE hObject, HANDLE hProcess) {
    ULONG bufferSize = 256;
    POBJECT_TYPE_INFORMATION pObjectType = nullptr;
    NTSTATUS status;

    do {
        if (pObjectType) {
            free(pObjectType);
        }
        
        pObjectType = (POBJECT_TYPE_INFORMATION)malloc(bufferSize);
        if (!pObjectType) {
            return "";
        }

        status = NtQueryObject(hObject, (OBJECT_INFORMATION_CLASS)ObjectTypeInformation, 
                             pObjectType, bufferSize, &bufferSize);
    } while (status == STATUS_INFO_LENGTH_MISMATCH);

    std::string result;
    if (NT_SUCCESS(status) && pObjectType->TypeName.Length > 0) {
        result = UnicodeToAnsi((void*)&pObjectType->TypeName);
    }

    free(pObjectType);
    return result;
}

void ProcessManager::closeProcessHandles(ProcessInfo& process) {
    HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, process.processId);
    if (!hProcess) {
        return;
    }

    for (auto& handleInfo : process.handles) {
        BOOL result = DuplicateHandle(hProcess, handleInfo.handle, NULL, NULL, 0, FALSE, DUPLICATE_CLOSE_SOURCE);
        if (!result) {
            DWORD error = GetLastError();
            // Handle error if needed
        }
    }

    CloseHandle(hProcess);
    
    // Clear handles after closing them
    process.handles.clear();
}
