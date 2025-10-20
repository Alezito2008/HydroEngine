#pragma once

#include <unordered_map>
#include <string>

struct Preferences {
    std::string theme = "Dark";
    float fontSize = 16;
    bool autoScroll = true;
};

class PreferenceManager {
public:
    static Preferences& GetPreferences(const std::string& fileName = "");
    static bool SavePreferences(const std::string& fileName = "");
private:
    static std::string s_FileName;
    static Preferences s_Preferences;
    inline static bool s_PreferencesLoaded = false;
    
    template <typename T>
    static void DeserializeField(const std::string& field, T& valueToChange);
};
