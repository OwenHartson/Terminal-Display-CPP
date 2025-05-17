#include "config.h"

Config::Config(){}

Config::~Config(){}

bool Config::readConfigFile(const std::string& filePath){
    std::ifstream configFile(filePath);

    if(!configFile.is_open()){
        std::cerr << "Could not open file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while(std::getline(configFile, line)){
        line.erase(0, line.find_first_not_of(" \t\r\n"));

        if(line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string key, value;
        if(std::getline(ss, key, '=') && std::getline(ss, value)){
            configMap[key] = value;
        }
    }

    configFile.close();

    return true;
}

std::unordered_map<std::string, std::string> Config::getConfigMap(){
    return configMap;
}

Config::StringCode Config::hashString(const std::string& str){
    if(str == "show_os") return StringCode::SHOW_OS;
    if(str == "show_host_name") return StringCode::SHOW_HOST_NAME;
    if(str == "show_ascii_art") return StringCode::SHOW_ASCII_ART;
    if(str == "show_cpu") return StringCode::SHOW_CPU;
    if(str == "show_ram") return StringCode::SHOW_RAM;
    if(str == "show_gpu") return StringCode::SHOW_GPU;
    return StringCode::ERROR;
}