#include <iostream>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <sys/utsname.h>
#include <vector>

#include "config.h"
#include "color_text.h"
#include "ascii_art.h"

using namespace std;

const string CONFIG_FILE_PATH{"config.txt"};

string getOS();
string getHostname();
void displayConfigOptions(const unordered_map<string, string>& config, Config& configFile);


int main(){
    Config configFile;
    if(configFile.readConfigFile(CONFIG_FILE_PATH)){
        const auto& config = configFile.getConfigMap();
        displayConfigOptions(config, configFile);
    }
}

string getOS(){
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        return string(buffer.sysname);
    } else {
        return "Unknown Linux";
    }
}

string getHostname(){
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    return string(hostname);
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
                    infoLines.push_back(colorize("Operating System: ", titleColor) + colorize(getOS(), infoColor));
                    break;
                case Config::StringCode::SHOW_HOST_NAME:
                    infoLines.push_back(colorize("Host Name: ", titleColor) + colorize(getHostname(), infoColor));
                    break;
                case Config::StringCode::SHOW_ASCII_ART:
                    showAsciiArt = true;
                    break;
                default:
                    break;
            }
        }
    }

    if(showAsciiArt == true){
        auto art = AsciiArt::getArt();

        size_t maxLines = max(art.size(), infoLines.size());
        for(size_t i = 0; i < maxLines; i++){
            string artPart = (i < art.size()) ? art[i] : string(art.empty() ? 0 : art[0].size(), ' ');
            string infoPart = (i < infoLines.size()) ? infoLines[i] : "";
            cout << artPart << "  " << infoPart << endl;
        }
    }else{
        for(size_t i = 0; i < infoLines.size(); i++){
            string infoPart = (i < infoLines.size()) ? infoLines[i] : "";
            cout << infoPart << endl;
        }
    }
}