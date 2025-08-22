#pragma once
#include <string>

class Config {
public:
    // User settings (saved to config.ini)
    bool autoUnblock = false;
    std::string language = "en";
    
    // Window settings
    int windowWidth = 500;
    int windowHeight = 250;
    bool centerWindow = true;
    
    // Font settings
    std::string fontPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    std::string fallbackFontPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    float fontSize = 16.0f;
    
    // Animation settings
    int fadeInDuration = 500;  // milliseconds
    int fadeOutDuration = 300; // milliseconds
    int hoverAnimationDuration = 200; // milliseconds
    int statusAnimationDuration = 300; // milliseconds
    
    // Process monitoring
    int updateInterval = 1000; // milliseconds
    
    // UI settings
    bool enableAnimations = true;
    bool enableHoverEffects = true;
    
    // Performance settings
    bool enableCaching = true;
    int cacheDuration = 1000; // milliseconds
    bool enableVsync = false;
    
    // Debug settings
    bool enableDebugOutput = false;
    bool enablePerformanceMonitoring = false;
    
    // Singleton pattern
    static Config& getInstance() {
        static Config instance;
        return instance;
    }
    
    // Config file management
    void loadFromFile();
    void saveToFile();
    std::string detectSystemLanguage();
    
private:
    Config();
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    std::string getConfigPath();
    std::string trim(const std::string& str);
};
