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

    const char* files[imagefiles.size()];
    for(int i = 0; i < imagefiles.size(); i++)
    {
        files[i] = imagefiles[i].c_str();
    }

    std::vector<std::string> itemfiles;
    Settings::ListDirectory(Settings::itemspath, itemfiles);

    std::sort(itemfiles.begin(), itemfiles.end());

    std::vector<sf::Sprite*> itemsprites;

    const char* items[itemfiles.size()];
    for(int i = 0; i < itemfiles.size(); i++)
    {
        items[i] = itemfiles[i].c_str();
    }

    /* Borders work!
    sf::RectangleShape rs = sf::RectangleShape(sf::Vector2f(100, 50));
    rs.setPosition(300,100);

    rs.setFillColor(sf::Color(0,0,0,0));

    rs.setOutlineColor(sf::Color(0,0,0));
    rs.setOutlineThickness(5);*/


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

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !Settings::keypressed)
            {
                //anything to debug
                Settings::keypressed = !Settings::keypressed;
            }

            //F1 to clear items
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && !Settings::keypressed)
            {
                itemsprites.clear();
                Settings::keypressed = !Settings::keypressed;
            }
        }

        Settings::Animate();

        ImGui::SFML::Update(window, Settings::deltaclock.restart());

        ImGui::Begin("Window1");

        if(ImGui::Combo("Filename", &Settings::spritechosen, files, IM_ARRAYSIZE(files)))
        {
            Settings::SetPreview(files[Settings::spritechosen]);
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

        if(ImGui::SliderFloat("Scale", &Settings::preview_scale, Settings::minscale, Settings::maxscale))
        {
            Settings::SetScale();
        }

        if(ImGui::Checkbox("Loop animation",&Settings::animationloop))
        {
            Settings::TurnAnimation();
        }

        if(ImGui::InputInt("Frametimer",&Settings::animationtimer))
        {  }

        if(ImGui::Combo("Add Item", &Settings::itemchosen, items, IM_ARRAYSIZE(items)))
        {

        }

        if(ImGui::Button("Confirm"))
        {
            sf::Sprite* temp = new sf::Sprite(*Settings::LoadTexture(items[Settings::itemchosen]));
            temp->setPosition(100,100);

            itemsprites.push_back(temp);
        }

        ImGui::End();

        window.clear(Settings::bgcolor);

        window.draw(Settings::preview);

        for(auto& it : itemsprites)
        {
            window.draw(*it);
        }

        //window.draw(rs);

        ImGui::SFML::Render(window);

        window.display();

    }

    ImGui::SFML::Shutdown();

    return 0;
}
