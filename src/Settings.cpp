#include "Settings.hpp"

////////////////events checkin
bool Settings::keypressed = false;

void Settings::SetEventVariables()
{
    keypressed = false;
}
///////////////imgui hax

char Settings::s_framecount[255];
int Settings::spritechosen;

sf::Clock Settings::deltaClock;

///////////////other

std::string Settings::settingpath;
std::string Settings::imagespath;
sf::Color Settings::bgcolor;

sf::Sprite Settings::preview;

int Settings::preview_width;
int Settings::preview_height;

int Settings::framescount;
int Settings::framewidth;

void Settings::Init()
{
    settingpath = "data/usr/settings.csv";
    imagespath = "data/images/";
    spritechosen = 5;

    bgcolor = sf::Color(60,50,50);

    preview.setTexture(*Settings::LoadTexture("spritesheet"));
    preview.setPosition(100,100);

    preview_width = preview.getGlobalBounds().width;
    preview_height = preview.getGlobalBounds().height;

    SetFramesCount(1);
}

void Settings::SetFramesCount(int fcount)
{
    framescount = fcount;
    framewidth = preview_width/framescount%preview_width;

    preview.setTextureRect(sf::IntRect(framewidth,0,preview_width/framescount,preview_height));
}
///////////////utilities

bool Settings::FileExists(const std::string& path) //Hail StackOverflow
{
    // return ( access( (imagespath+path).c_str(), F_OK ) != 1 );
    struct stat buffer;
    return (stat (path.c_str(), &buffer) == 0);
}

void Settings::ListDirectory(const std::string& path, std::vector<std::string>& namelist)
{
    for(auto& p : fs::directory_iterator("data/images"))
    {
        std::ostringstream oss;
        oss << p;

        std::string temp = oss.str();
        std::string name = "";
        for(int i = temp.size()-2; i > 0; i--)
        {
            if(temp[i] == '/' or temp[i] == '\\')
                break;

            name += temp[i];
        }

        std::reverse(name.begin(), name.end());

        namelist.push_back(name);
    }
}

///////////////read settings
std::map<std::string, std::string> Settings::readsettings;
std::string Settings::ReadSetting(const std::string& setting)
{
    //check if setting was needed before
    auto search = readsettings.find(setting);
    if(search != readsettings.end())
    {
        return search->second;
    }

    //if not then lets check the file!
    std::fstream file;
    file.open(settingpath, std::ios::in);

    if(!file.good())
    {
        std::cout << "Setting file load error!\n";
        return "";
    }

    std::string title ="";
    std::string value ="";
    getline(file, title); //reading headers

    while(!file.eof())
    {
        getline(file, title, ';');
        getline(file, value);
        if (title == "")
            continue; //emptyline

        if (title == setting) //found!
        {
            readsettings[title] = value;
            return value;
        }

    }

    file.close();

    return "";
}

///////////////////load sprites
std::map<std::string, sf::Texture*> Settings::loadedtextures;

sf::Texture* Settings::LoadTexture(const std::string& name)
{
    //check if setting was needed before
    auto search = loadedtextures.find(name);
    if(search != loadedtextures.end())
    {
        return search->second;
    }
    //if not then lets check the files!

    sf::Texture* temp = new sf::Texture();

    if(FileExists(imagespath+name+".png"))
    {
        temp->loadFromFile(imagespath+name+".png");
    }

    return temp;
}
