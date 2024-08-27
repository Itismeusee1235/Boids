#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "Entity.hpp"
using namespace sf;
using namespace std;

float distance(Vector2f a, Vector2f b)
{
    return sqrtf(pow((a.x-b.x),2) + pow((a.y-b.y), 2));
}

int main()
{
    float vel = 25;

    RenderWindow win(VideoMode(400, 400), "Boids");
    Clock clock;
    Entity test("./S.png", Vector2f(200,200), Vector2f(30,40));    

    while (win.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        Event ev;
        while (win.pollEvent(ev))
        {
            if(ev.type == Event::Closed)
            {
                win.close();
            }
                       
        }

        win.clear();
        test.Update(deltaTime);
        test.Draw(win);
        win.display();
        
    }
    

    return 0;

}