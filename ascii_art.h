#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class AsciiArt{
    public:
        static std::vector<std::string> getArt(){
            std::vector<std::string> art;
            std::string fileName = "ascii_art.txt";
            std::ifstream file(fileName);
            if(!file.is_open()){
                std::cerr << "Could not open ASCII art file: " << fileName << std::endl;
                return art;
            }

            std::string line;
            while(std::getline(file, line)){
                art.push_back(line);
            }

            return art;
        }
    private:
};