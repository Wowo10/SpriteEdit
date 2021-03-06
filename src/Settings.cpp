#include "Settings.hpp"

////////////////events checkin
bool Settings::keypressed = false;

void Settings::SetEventVariables()
{
    keypressed = false;
}
///////////////imgui hax

int Settings::spritechosen;
float Settings::minscale, Settings::maxscale;

sf::Clock Settings::deltaclock;
int Settings::animationtimer;
sf::Clock Settings::animationclock;

///////////////other

std::string Settings::settingpath;
std::string Settings::imagespath;
sf::Color Settings::bgcolor;

sf::Sprite Settings::preview;

std::string Settings::currentfolder;
std::vector<std::string> Settings::imagesinfolder;

int Settings::preview_width;
int Settings::preview_height;
float Settings::preview_scale;

bool Settings::animationloop;

void Settings::Animate()
{
    if(animationloop && animationclock.getElapsedTime().asMilliseconds() >= animationtimer)
    {
        animationclock.restart();
        frame = (frame + 1)%(framescount);
        SetFrame();
    }
}

void Settings::TurnAnimation()
{
    //animationloop = !animationloop;
}

bool Settings::folder;

int Settings::framescount;
int Settings::framewidth;
int Settings::frame;

void Settings::Init()
{
    settingpath = "data/usr/settings.csv";
    imagespath = "data/images/";
    spritechosen = 5;
    minscale = std::stof(ReadSetting("minscale"));
    maxscale = std::stof(ReadSetting("maxscale"));

    framescount = 1;
    frame = 0;

    bgcolor = sf::Color(
                        std::stoi(ReadSetting("bgred")),
                        std::stoi(ReadSetting("bgblue")),
                        std::stoi(ReadSetting("bggreen")));


    animationtimer = std::stof(Settings::ReadSetting("animationtimer"));

    preview.setTexture(*Settings::LoadTexture("spritesheet"));
    preview.setPosition(100,100);

    animationloop = false;

    folder = false;
    currentfolder="";

    preview_width = preview.getGlobalBounds().width;
    preview_height = preview.getGlobalBounds().height;
    preview_scale = 1.0f;

    SetFramesCount();
}

void Settings::SetPreview(const std::string& filename)
{
    folder = filename.substr( filename.length() - 4 ) != ".png";

    if(folder)
    {
        currentfolder = filename;

        imagesinfolder.clear();

        ListDirectory(imagespath+"/"+filename, imagesinfolder);

        std::sort(imagesinfolder.begin(), imagesinfolder.end());

        framescount = imagesinfolder.size();
    }
    else
    {
        sf::Vector2f tempposition = preview.getPosition();
        sf::Vector2f tempscale = preview.getScale();
        preview = sf::Sprite(*LoadTexture(filename));

        preview.setPosition(tempposition);
        preview.setScale(tempscale);

        preview_width = preview.getLocalBounds().width;
        preview_height = preview.getLocalBounds().height;
    }
    SetFramesCount();
}

void Settings::SetFramesCount()
{
    frame = 0;

    if(folder)
    {

    }
    else
    {
        framewidth = (preview_width/framescount)%preview_width;
    }
    SetFrame();
}

void Settings::SetFrame()
{
    if(folder)
    {
        sf::Vector2f tempposition = preview.getPosition();
        sf::Vector2f tempscale = preview.getScale();

        preview = sf::Sprite(*LoadTexture(currentfolder+"/"+imagesinfolder[frame]));

        preview.setPosition(tempposition);
        preview.setScale(tempscale);
    }
    else
        preview.setTextureRect(sf::IntRect(framewidth*frame,0,preview_width/framescount,preview_height));
}

void Settings::SetScale()
{
    preview.setScale(preview_scale,preview_scale);
}
///////////////utilities

bool Settings::FileExists(const std::string& path) //Hail StackOverflow
{
    struct stat buffer;
    return (stat (path.c_str(), &buffer) == 0);
}

void Settings::ListDirectory(const std::string& path, std::vector<std::string>& namelist)
{
    for(auto& p : fs::directory_iterator(path))
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

    std::string mypath = imagespath+name;

    if(mypath.substr( mypath.length() - 4 ) != ".png")
        mypath += ".png";

    if(FileExists(mypath))
    {
        temp->loadFromFile(mypath);

        loadedtextures[name] = temp;
    }

    return temp;
}
