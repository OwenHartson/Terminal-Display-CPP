#pragma once
#include <string>
#include <unistd.h>
#include <sys/utsname.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>

class SysInfo{
    public:
        SysInfo();
        ~SysInfo();

        static std::string getOS(){
            struct utsname buffer;
            if(uname(&buffer) == 0){
                return std::string(buffer.sysname);
            }else{
                return "Unknown";
            }
        }

        static std::string getHostName(){
            char hostname[256];
            gethostname(hostname, sizeof(hostname));
            return std::string(hostname);
        }

        static std::string getCPU(){
            std::ifstream cpuinfo("/proc/cpuinfo");
            std::string line;
            std::string modelName;

            std::string returnValue;

            if(cpuinfo.is_open()){
                while(getline(cpuinfo, line)){
                    std::stringstream ss(line);
                    std::string key;

                    if(getline(ss, key, ':')){
                        std::string value;

                        if(getline(ss, value)){
                            if(key.find("model name") != std::string::npos){
                                modelName = value.substr(value.find_first_not_of(" \t"));
                                returnValue = modelName;
                            } 
                        }
                    }
                }
            }else{
                returnValue = "Unknown";
            }

            cpuinfo.close();

            return returnValue;
        }

        static std::string getRam(){
            std::ifstream memInfoFile("/proc/meminfo");
            int memAvailKB{0};
            int memTotalKB{0};
            int kbInGb{1'048'576};
            std::string returnValue;

            if(memInfoFile.is_open()){
                std::string line;

                while(getline(memInfoFile, line)){
                    std::istringstream iss(line);
                    std::string key;

                    if(getline(iss, key, ':')){
                        std::string value;
                        if(iss >> value){
                            if(key == "MemAvailable"){
                                memAvailKB = std::stoi(value);
                            }else if(key == "MemTotal"){
                                memTotalKB = std::stoi(value);
                            }
                        }
                    }
                }

                double memAvailGB = static_cast<double>(memAvailKB) / kbInGb;
                double memTotalGB = static_cast<double>(memTotalKB) / kbInGb;
                
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << memAvailGB << "/" << memTotalGB << " GB Available";

                returnValue = oss.str();
            }else{
                returnValue = "Unknown";
            }

            memInfoFile.close();

            return returnValue;
        }

        static std::string getGPU(){
            std::ifstream gpuinfo("/proc/driver/nvidia/gpus/0/information");
            std::string line;
            std::string gpuName;

            if(gpuinfo.is_open()){
                while(getline(gpuinfo, line)){
                    std::istringstream iss(line);
                    std::string key;

                    if(getline(iss, key, ':')){
                        std::string value;

                        if(getline(iss, value)){
                            if(key.find("Model") != std::string::npos){
                                gpuName = value.substr(value.find_first_not_of(" \t"));
                                break;
                            }
                        }
                    }
                }
            }
            gpuinfo.close();
            if(!gpuName.empty()) return gpuName;

            struct stat bufferCheck;
            if(stat("/usr/bin/lspci", &bufferCheck) != 0 && stat("/bin/lspci", &bufferCheck) != 0){
                return "Unknown";
            }

            FILE* pipe = popen("lspci | grep -i 'vga\\|3d\\|2d'", "r");
            if(!pipe) return "Unknown";
            char buffer[256];
            std::string result;
            while(fgets(buffer, sizeof(buffer), pipe) != nullptr){
                result += buffer;
            }
            pclose(pipe);

            if(!result.empty()){
                size_t pos = result.find(":");
                if(pos != std::string::npos){
                    return result.substr(pos + 2);
                }else{
                    return result;
                }
            }

            return "Unknown";
        }
};