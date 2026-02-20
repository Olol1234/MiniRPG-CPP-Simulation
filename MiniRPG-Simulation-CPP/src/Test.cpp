#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
    sf::Texture testTexture;
    if (!testTexture.loadFromFile("data/Enemy1.png"))
    {
        std::cout << "Failed to load goblin.png\n";
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        //sf::Texture testTexture;
        //if (!testTexture.loadFromFile("Enemy1.png"))
        //{
        //    std::cout << "Failed to load goblin.png\n";
        //}

        sf::Sprite testSprite;
        testSprite.setTexture(testTexture);

        // Optional: position it in center
        testSprite.setPosition(400.f, 300.f);

        // Optional: scale if too small
        testSprite.setScale(0.07f, 0.07f);

        window.draw(testSprite);

        window.display();
    }

    return 0;
}
