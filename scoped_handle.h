#pragma once
#include <windows.h>

class ScopedHandle {
private:
    HANDLE m_handle;

public:
    // Constructor
    explicit ScopedHandle(HANDLE handle) : m_handle(handle) {}
    
    // Destructor - automatically closes handle
    ~ScopedHandle() {
        if (m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr) {
            CloseHandle(m_handle);
        }
    }
    
    // Disable copy constructor and assignment
    ScopedHandle(const ScopedHandle&) = delete;
    ScopedHandle& operator=(const ScopedHandle&) = delete;
    
    // Allow move constructor and assignment
    ScopedHandle(ScopedHandle&& other) noexcept : m_handle(other.m_handle) {
        other.m_handle = INVALID_HANDLE_VALUE;
    }
    
    ScopedHandle& operator=(ScopedHandle&& other) noexcept {
        if (this != &other) {
            if (m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr) {
                CloseHandle(m_handle);
            }
            m_handle = other.m_handle;
            other.m_handle = INVALID_HANDLE_VALUE;
        }
        return *this;
    }
    
    // Conversion operator to HANDLE
    operator HANDLE() const { return m_handle; }
    
    // Get handle value
    HANDLE get() const { return m_handle; }
    
    // Check if handle is valid
    bool isValid() const { 
        return m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr; 
    }
    
    // Release ownership (caller is responsible for closing)
    HANDLE release() {
        HANDLE temp = m_handle;
        m_handle = INVALID_HANDLE_VALUE;
        return temp;
    }
};
