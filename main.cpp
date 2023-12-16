#include "display.hpp"

//NOTES:
//pawn cut not checked
int main() {
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 800;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess");
    
    BoardLayout bl(100, 100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            bl.event_loader(event);
        }

        window.clear(sf::Color(255,255,255));
        bl.loop(window);
        window.display();
    }
    return 0; 
}