#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "config.h"
#include "color_text.h"
#include "ascii_art.h"
#include "get_sys_info.h"

using namespace std;

const string CONFIG_FILE_PATH{"config.txt"};

void displayConfigOptions(const unordered_map<string, string>& config, Config& configFile);

int main(){
    Config configFile;
    if(configFile.readConfigFile(CONFIG_FILE_PATH)){
        const auto& config = configFile.getConfigMap();
        displayConfigOptions(config, configFile);
    }
}

void displayConfigOptions(const unordered_map<string, string>& config, Config& configFile){
    bool showAsciiArt{false};

    std::string titleColor{"blue"};
    auto titleColorIt = config.find("title_color");
    if(titleColorIt != config.end()){
        titleColor = titleColorIt->second;
    }

    std::string infoColor{"white"};
    auto infoColorIt = config.find("info_color");
    if(infoColorIt != config.end()){
        infoColor = infoColorIt->second;
    }

    auto colorize = [&](const string& text, const string& color)-> string{
        if(color == "red") return ColorText::red(text);
        if(color == "green") return ColorText::green(text);
        if(color == "yellow") return ColorText::yellow(text);
        if(color == "cyan") return ColorText::cyan(text);
        if(color == "magenta") return ColorText::magenta(text);
        if(color == "white") return ColorText::white(text);
        if(color == "black") return ColorText::black(text);
        if(color == "bold") return ColorText::bold(text);
        if(color == "none") return ColorText::none(text);
        return ColorText::blue(text);
    };

    vector<string> infoLines;
    for(const auto & [key, value] : config){
        if(value == "true"){
            switch(configFile.hashString(key)){
                case Config::StringCode::SHOW_OS:
                    infoLines.push_back(colorize("Operating System: ", titleColor) + colorize(SysInfo::getOS(), infoColor));
                    break;
                case Config::StringCode::SHOW_HOST_NAME:
                    infoLines.push_back(colorize("Host Name: ", titleColor) + colorize(SysInfo::getHostName(), infoColor));
                    break;
                case Config::StringCode::SHOW_ASCII_ART:
                    showAsciiArt = true;
                    break;
                case Config::StringCode::SHOW_CPU:
                    infoLines.push_back(colorize("CPU: ", titleColor) + colorize(SysInfo::getCPU(), infoColor));
                    break;
                case Config::StringCode::SHOW_RAM:
                    infoLines.push_back(colorize("RAM: ", titleColor) + colorize(SysInfo::getRam(), infoColor));
                    break;
                case Config::StringCode::SHOW_GPU:
                    infoLines.push_back(colorize("GPU: ", titleColor) + colorize(SysInfo::getGPU(), infoColor));
                    break;
                default:
                    break;
            }
        }
    }

    infoLines.at(0) = "";

    auto strip_ansi = [](const std::string& str) -> std::string{
        string result;
        bool inEscape = false;
        for(size_t i = 0; i < str.size(); ++i){
            if(str[i] == '\033') inEscape = true;
            else if(inEscape && str[i] == 'm') inEscape = false;
            else if(!inEscape) result += str[i];
        }
        return result;
    };

    size_t maxInfoLen{0};
    for(const auto& line : infoLines){
        size_t len = strip_ansi(line).length();
        if(len > maxInfoLen){
            maxInfoLen = len;
        }
    }

    string infoBorder = string(maxInfoLen, '_');

    if(showAsciiArt == true){
        auto art = AsciiArt::getArt();

        size_t maxArtLength{0};
        for(const auto& line : art){
            if(line.length() > maxArtLength){
                maxArtLength = line.length();
            }
        }
        size_t infoIndent = maxArtLength + 2;

        size_t maxLines = max(art.size(), infoLines.size() + 2);
        for(size_t i = 0; i < maxLines; i++){
            string artPart = (i < art.size()) ? art[i] : string(art.empty() ? 0 : art[0].size(), ' ');
            string infoPart;

            if(i == 0 || i == infoLines.size() + 1){
                infoPart = infoBorder;
            }else if(i - 1 < infoLines.size()){
                string stripped = strip_ansi(infoLines[i - 1]);
                size_t pad = maxInfoLen > stripped.length() ? maxInfoLen - stripped.length() : 0;
                infoPart = infoLines[i - 1] + string(pad, ' ');
            }else{
                infoPart = "";
            }

            cout << artPart;
            if(!infoPart.empty()){
                cout << string(infoIndent - artPart.length(), ' ') << infoPart;
            }
            cout << endl;
        }

    }else{
        cout << infoBorder << endl;
        cout << endl;
        for(size_t i = 0; i < infoLines.size(); i++){
            string stripped = strip_ansi(infoLines[i]);
            size_t pad = maxInfoLen > stripped.length() ? maxInfoLen - stripped.length() : 0;
            cout << infoLines[i] << string(pad, ' ') << endl;
        }
        cout << infoBorder << endl;
    }
}