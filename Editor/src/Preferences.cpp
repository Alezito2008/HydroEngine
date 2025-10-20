#include "Preferences.h"
#include "Console.h"

#include <fstream>
#include <functional>

static constexpr const char* DEFAULT_CONFIG_FILE = "config.ini";

std::string PreferenceManager::s_FileName;
Preferences PreferenceManager::s_Preferences;

template<>
inline void PreferenceManager::DeserializeField<bool>(const std::string& field, bool& valueToChange) {
    if (field != "true" && field != "false" && field != "1" && field != "0") {
        Console::Warning("Expected: <bool> on " + field);
        return;
    }
    valueToChange = field == "true" || field == "1";
}

template<>
inline void PreferenceManager::DeserializeField<int>(const std::string& field, int& valueToChange) {
    try {
        valueToChange = stoi(field); 
    } catch (...) {
        Console::Warning("Expected: <int> on " + field);
        return;
    }
}

template<>
inline void PreferenceManager::DeserializeField<float>(const std::string& field, float& valueToChange) {
    try {
        valueToChange = stof(field); 
    } catch (...) {
        Console::Warning("Expected: <float> on " + field);
    }
}

static std::string trim(const std::string& s) {
    const size_t start = s.find_first_not_of(" \t\r\n");
    const size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

Preferences& PreferenceManager::GetPreferences(const std::string& fileName)
{
    if (s_PreferencesLoaded) return s_Preferences;

    s_PreferencesLoaded = true;
    s_FileName = fileName.empty() ? DEFAULT_CONFIG_FILE : fileName;
    std::string buffer;
    std::ifstream in(s_FileName);

    if (!in.is_open()) {
        Console::Error("Couldn't open preferences file: " + fileName);
        return s_Preferences; // Devolver por defecto
    }

    int line_number = 0;

    while (getline(in, buffer)) {
        line_number++;

        if (buffer.empty()) continue;

        size_t equal_pos = buffer.find("=");
        if (equal_pos == std::string::npos) {
            Console::Warning("Invalid line on " + s_FileName + ':' + std::to_string(line_number) + " '" + buffer + '\'');
            continue;
        };
        std::string key = trim(buffer.substr(0, equal_pos));
        std::string value = trim(buffer.substr(equal_pos + 1));
        
        if (key == "theme") {
            s_Preferences.theme = value;
        } else if (key == "fontSize") {
            DeserializeField<float>(value, s_Preferences.fontSize);
        } else if (key == "autoScroll") {
            DeserializeField<bool>(value, s_Preferences.autoScroll);
        } else {
            Console::Error("Unknown key '" + key + "' in file " + s_FileName);
        }
    }

    return s_Preferences;
}

bool PreferenceManager::SavePreferences(const std::string& fileName)
{
    std::string fileToOpen;

    if (!fileName.empty()) fileToOpen = fileName;
    else if (!s_FileName.empty()) fileToOpen = s_FileName;
    else fileToOpen = DEFAULT_CONFIG_FILE;

    std::ofstream out(fileToOpen);
    if (!out.is_open()) return false;

    out << "theme = " << s_Preferences.theme << '\n';
    out << "fontSize = " << s_Preferences.fontSize << '\n';
    out << "autoScroll = " << s_Preferences.autoScroll << '\n';

    return true;
}
