#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>

class Config{
    public:
        Config();
        ~Config();

        enum class StringCode{
            SHOW_OS,
            SHOW_HOST_NAME,
            SHOW_ASCII_ART,
            SHOW_CPU,
            SHOW_RAM,
            SHOW_GPU,
            ERROR
        };

        bool readConfigFile( const std::string& filePath );
        std::unordered_map<std::string, std::string> getConfigMap();
        StringCode hashString(const std::string& str);

    private:
        std::unordered_map<std::string, std::string> configMap;
};