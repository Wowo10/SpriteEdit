#ifndef SETTINGS_HPP_INNCLUDED
#define SETTINGS_HPP_INNCLUDED

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <fstream>

#include <sys/stat.h> //for filecheck
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Settings
{
public:
    ////////////////////bools for not twice event checking in one loop iteration

    static bool keypressed;

    static void SetEventVariables();

    ///////////////////variables for Imgui hax
    static char s_framecount[255];
    static int spritechosen;

    static sf::Clock deltaClock;

    ///////////////////other

    static std::string settingpath;
    static std::string imagespath;
    static sf::Color bgcolor;

    static sf::Sprite preview;

    static int preview_width;
    static int preview_height;

    static int framescount;
    static int framewidth;

    static void Init();

    static void SetFramesCount(int fcount);

    ///////////////////utilities

    static bool FileExists(const std::string& name);

    static void ListDirectory(const std::string& path, std::vector<std::string>& namelist);

    ///////////////////read settings
    static std::map<std::string, std::string> readsettings;
    static std::string ReadSetting(const std::string& setting);

    ///////////////////load sprites
    static std::map<std::string, sf::Texture*> loadedtextures;
    static sf::Texture* LoadTexture(const std::string& name);

};

#endif // SETTINGS_HPP_INNCLUDED
