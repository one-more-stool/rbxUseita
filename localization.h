#pragma once
#include <string>
#include <unordered_map>

struct LocalizedStrings {
    std::string robloxWindow;
    std::string blocking;
    std::string ok;
    std::string autoUnblock;
};

class Localization {
public:
    static Localization& getInstance();
    
    void setLanguage(const std::string& language);
    const LocalizedStrings& getStrings() const;
    std::string getLanguage() const;
    
private:
    Localization();
    void loadLanguages();
    
    std::string currentLanguage;
    std::unordered_map<std::string, LocalizedStrings> languages;
    LocalizedStrings currentStrings;
};
