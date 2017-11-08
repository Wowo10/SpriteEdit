#include "Settings.hpp"

int main()
{
    Settings::Init();

    sf::RenderWindow window(sf::VideoMode(
                                std::stoi(Settings::ReadSetting("resx")),
                                std::stoi(Settings::ReadSetting("resy"))), "SpriteEditor");

    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    window.resetGLStates();

    std::vector<std::string> imagefiles;
    Settings::ListDirectory(Settings::imagespath,imagefiles);

    while(window.isOpen())
    {
        Settings::SetEventVariables();
        sf::Event event;

        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && Settings::keypressed)
            {
                //some event action
                Settings::keypressed = !Settings::keypressed;
            }

        }

        ImGui::SFML::Update(window, Settings::deltaClock.restart());


        ImGui::Begin("Window1");

        const char* items[imagefiles.size()];
        for(int i = 0; i < imagefiles.size(); i++)
        {
            items[i] = imagefiles[i].c_str();
        }

        ImGui::Combo("filename", &Settings::spritechosen, items, IM_ARRAYSIZE(items));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.

        if(ImGui::InputText("Frames", Settings::s_framecount, 255))
        {
            Settings::SetFramesCount(atoi(Settings::s_framecount));
        }

        if(ImGui::Button("+"))
        {
            if(Settings::s_framecount != "")
                strcpy(Settings::s_framecount, std::to_string(atoi(Settings::s_framecount)+1).c_str());

            Settings::SetFramesCount(atoi(Settings::s_framecount));
        }

        if(ImGui::Button("-"))
        {
            if(Settings::s_framecount != "")
                strcpy(Settings::s_framecount, std::to_string(atoi(Settings::s_framecount)-1).c_str());

            Settings::SetFramesCount(atoi(Settings::s_framecount));
        }

        ImGui::End();

        window.clear(Settings::bgcolor);

        window.draw(Settings::preview);

        ImGui::SFML::Render(window);

        window.display();

    }

    /* foreachloop
    for(auto& setting : Settings::readsettings)
    {
        std::cout << setting.first << " " << setting.second <<"\n";
    }*/


    ImGui::SFML::Shutdown();

    return 0;
}
