#include "config.h"
#include <fstream>
#include <sstream>
#include <windows.h>

Config::Config() {
    loadFromFile();
}

std::string Config::getConfigPath() {
    return "config.ini";
}

std::string Config::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

void Config::loadFromFile() {
    std::ifstream file(getConfigPath());
    if (!file.is_open()) {
        // First run - detect system language
        language = detectSystemLanguage();
        saveToFile(); // Save default config
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue; // Skip empty lines and comments
        }
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));
            
            if (key == "autoUnblock") {
                autoUnblock = (value == "true" || value == "1");
            } else if (key == "language") {
                language = value;
            }
        }
    }
    file.close();
}

void Config::saveToFile() {
    std::ofstream file(getConfigPath());
    if (!file.is_open()) {
        return;
    }
    
    file << "# rbxUseita Configuration File\n";
    file << "# This file stores user preferences\n\n";
    
    file << "# Auto-unblock setting (true/false)\n";
    file << "autoUnblock=" << (autoUnblock ? "true" : "false") << "\n\n";
    
    file << "# User interface language\n";
    file << "language=" << language << "\n\n";
    
    file.close();
}

std::string Config::detectSystemLanguage() {
    // Get system locale
    LCID lcid = GetSystemDefaultLCID();
    LANGID langId = LANGIDFROMLCID(lcid);
    WORD primaryLang = PRIMARYLANGID(langId);
    
    // Map Windows language IDs to our supported languages
    switch (primaryLang) {
        case LANG_RUSSIAN:
            return "ru";
        case LANG_FINNISH:
            return "fi";
        case LANG_SPANISH:
            return "es";
        case LANG_FRENCH:
            return "fr";
        case LANG_GERMAN:
            return "de";
        case LANG_ITALIAN:
            return "it";
        case LANG_PORTUGUESE:
            return "pt";
        case LANG_POLISH:
            return "pl";
        case LANG_DUTCH:
            return "nl";
        case LANG_SWEDISH:
            return "sv";
        case LANG_NORWEGIAN:
            return "no";
        case LANG_DANISH:
            return "da";
        case LANG_CZECH:
            return "cs";
        case LANG_HUNGARIAN:
            return "hu";
        case LANG_ROMANIAN:
            return "ro";
        case LANG_BULGARIAN:
            return "bg";
        case LANG_UKRAINIAN:
            return "uk";
        case LANG_TURKISH:
            return "tr";
        case LANG_VIETNAMESE:
            return "vi";
        case LANG_INDONESIAN:
            return "id";
        case LANG_MALAY:
            return "ms";
        case LANG_GREEK:
            return "el";
        case LANG_CROATIAN:
            return "hr";
        case LANG_SLOVAK:
            return "sk";
        case LANG_SLOVENIAN:
            return "sl";
        case LANG_LITHUANIAN:
            return "lt";
        case LANG_LATVIAN:
            return "lv";
        case LANG_ESTONIAN:
            return "et";
        case LANG_ENGLISH:
        default:
            return "en";
    }
}
