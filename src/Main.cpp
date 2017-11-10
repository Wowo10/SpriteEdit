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

        //std::cout << Settings::animationclock.getElapsedTime().asMilliseconds()<<"\n";
        //if( Settings::animationclock.getElapsedTime().asMilliseconds() >= 1000 )
        //    Settings::animationclock.restart();

        Settings::Animate();

        ImGui::SFML::Update(window, Settings::deltaclock.restart());

        ImGui::Begin("Window1");

        const char* items[imagefiles.size()];
        for(int i = 0; i < imagefiles.size(); i++)
        {
            items[i] = imagefiles[i].c_str();
        }

        if(ImGui::Combo("filename", &Settings::spritechosen, items, IM_ARRAYSIZE(items)))
        {
            //will promote to function soon
            sf::Vector2f temp = Settings::preview.getPosition();
            Settings::preview = sf::Sprite(*Settings::LoadTexture(items[Settings::spritechosen]));

            Settings::preview.setPosition(temp);

            Settings::preview_width = Settings::preview.getLocalBounds().width;
            Settings::preview_height = Settings::preview.getLocalBounds().height;
            Settings::framewidth = Settings::preview_width/Settings::framescount%Settings::preview_width;
        }

        if(ImGui::InputInt("Frames", &Settings::framescount))
        {
            Settings::SetFramesCount();
        }

        if(ImGui::SliderInt("slider int", &Settings::frame, 0, Settings::framescount-1))
        {
            Settings::SetFrame();
        }

        if(ImGui::SliderFloat("slider float", &Settings::preview_scale, 1.0f, 3.0f))
        {
            Settings::SetScale();
        }

        if(ImGui::Checkbox("loop animation",&Settings::animationloop))
        {
            Settings::TurnAnimation();
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
