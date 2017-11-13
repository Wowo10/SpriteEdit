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
    Settings::ListDirectory(Settings::imagespath, imagefiles);

    std::sort(imagefiles.begin(), imagefiles.end());

    const char* items[imagefiles.size()];
    for(int i = 0; i < imagefiles.size(); i++)
    {
        items[i] = imagefiles[i].c_str();
    }

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

        Settings::Animate();

        ImGui::SFML::Update(window, Settings::deltaclock.restart());

        ImGui::Begin("Window1");

        if(ImGui::Combo("Filename:", &Settings::spritechosen, items, IM_ARRAYSIZE(items)))
        {
            Settings::SetPreview(items[Settings::spritechosen]);
        }

        if(!Settings::folder && ImGui::InputInt("Frames", &Settings::framescount))
        {
            if(Settings::framescount == 0)
                Settings::framescount++;

            Settings::SetFramesCount();
        }

        if(ImGui::SliderInt("Current frame", &Settings::frame, 0, Settings::framescount-1))
        {
            Settings::SetFrame();
        }

        if(ImGui::SliderFloat("Scale", &Settings::preview_scale, 0.5f, 3.0f))
        {
            Settings::SetScale();
        }

        if(ImGui::Checkbox("Loop animation",&Settings::animationloop))
        {
            Settings::TurnAnimation();
        }

        if(ImGui::InputInt("Frametimer",&Settings::animationtimer))
        {  }

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
