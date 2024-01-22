#define NOMINMAX
#include <string.h>
#include <array>
#include <thread>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <cpr/cpr.h>

struct STRS
{
    std::string name;
    std::string timezone;
};

void makeHttpRequest(bool& indicator, cpr::Url& url, std::string& rStr);

int main()
{
    sf::RenderWindow window(sf::VideoMode(300, 200), "RealTime", sf::Style::Close);

    bool indicator = 1;
    std::string str;
    std::string lastStr;
    std::array<STRS, 5> arr;
    bool Pressed = 0;
    cpr::Url url {"http://worldtimeapi.org/api/ip"};

    arr[0].name = "LOC";
    arr[1].name = "UA";
    arr[2].name = "DE";
    arr[3].name = "US";
    arr[4].name = "CN";

    arr[0].timezone = "ip";
    arr[1].timezone = "timezone/Europe/Kyiv";
    arr[2].timezone = "timezone/Europe/Berlin";
    arr[3].timezone = "timezone/America/New_York";
    arr[4].timezone = "timezone/Asia/Pyongyang";

    sf::Image imgLine;
    imgLine.loadFromFile("resource/images/InterfaceRealTime.png");
    sf::Texture txtrLine;
    txtrLine.loadFromImage(imgLine);
    sf::Sprite spLine;
    spLine.setTexture(txtrLine);
    spLine.setTextureRect(sf::IntRect(0, 0, 31, 5));
    spLine.setScale(8, 8);
    spLine.setPosition(10, 100);

    sf::Image imgFon;
    imgFon.loadFromFile("resource/images/InterfaceRealTime.png");
    sf::Texture txtrFon;
    txtrFon.loadFromImage(imgLine);
    sf::Sprite spFon;
    spFon.setTexture(txtrFon);
    spFon.setTextureRect(sf::IntRect(0, 6, 7, 5));
    spFon.setScale(8, 8);
    spFon.setPosition(10, 100);


    sf::Font font;
    sf::Text text2;
    sf::Text text;
    sf::Text text3;

    if (!font.loadFromFile("resource/fonts/NewWar.ttf"))
    {
        return 0;
    }
    text.setFont(font);
    text.setFillColor(sf::Color(249, 148, 23));
    text.setCharacterSize(38);
    text.setPosition(10, 0);

    text3.setFont(font);
    text3.setFillColor(sf::Color(249, 148, 23));
    text3.setCharacterSize(24);
    text3.setPosition(10, 50);

    text2 = text;
    text2.setCharacterSize(16);

    std::thread th(makeHttpRequest, std::ref(indicator), std::ref(url), std::ref(str));
    th.detach();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) { Pressed = 0; }

        sf::Sprite spTemp = spFon;
        spTemp.setScale(8, 8);
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
        for (short i = 0; i < 5; i++)
        {

            spTemp.setPosition(10 + (48 * i), 100);
            if (spTemp.getGlobalBounds().contains(worldPosition)
                && sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && Pressed == 0)
            {
                url = { "http://worldtimeapi.org/api/" + arr[i].timezone };
                spFon.setPosition(spTemp.getPosition());
                Pressed = 1;
            }
        }



        std::string time;
        std::string timezone;
        std::string mainStr;

        if (!indicator)
        {
            std::string s = str;
            time = nlohmann::json::parse(s).at("datetime");
            timezone = nlohmann::json::parse(s).at("timezone");
            time = time.substr(11, 8);
            mainStr = time + "\n\n\n   Amperline";
        }
        else
        {
            mainStr = "Network\nError \n\n   Amperline";
        }

        text3.setString(timezone);
        text.setString(mainStr);
        window.clear(sf::Color(77, 76, 125));

        window.draw(text);
        if (!indicator) { window.draw(text3); }
        window.draw(spFon);

        window.draw(spLine);

        text2.setString(arr[0].name);
        text2.setPosition(19, 110);
        window.draw(text2);
        for (short i = 1; i < 5; i++)
        {
            text2.setString(arr[i].name);
            text2.setPosition(23 + (49 * i), 110);
            switch (i)
            {
            case 3:
                text2.setPosition(23 + (49 * i), 110);
            case 4:
                text2.setPosition(21 + (49 * i), 110);
            }
            window.draw(text2);
        }

        window.display();
    }
    return 0;
}

void makeHttpRequest(bool& indicator, cpr::Url& url, std::string& rStr)
{
    while (true)
    {
        cpr::Response resp = cpr::Get(url, cpr::Timeout{1000});
        indicator = 1;

        if (resp.status_code == 200)
        {
            rStr = resp.text;
            indicator = 0;
        }
    }
}