#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace sf;
using namespace std;

float distance(Vector2f a, Vector2f b)
{
    return sqrtf(pow((a.x-b.x),2) + pow((a.y-b.y), 2));
}

int main()
{
    const float PI = 3.14159265;    
    float vel = 25;

    RenderWindow win(VideoMode(400, 400), "Boids");
    Clock clock;

    Texture texture;
    if(!texture.loadFromFile("./S.png"))
    {
        cout << "Failed";
        return 0;
    }
    Sprite player(texture);
    player.setScale(Vector2f(0.5,0.5));
    player.setOrigin(Vector2f(player.getGlobalBounds().width/2, player.getGlobalBounds().height/2));
    player.setPosition(Vector2f(200,200));

    CircleShape target(5);

    vector<Vector2f> targets;


    while (win.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        Vector2f currPos = player.getPosition();
        Vector2f finalPos = currPos;

        Event ev;
        while (win.pollEvent(ev))
        {
            if(ev.type == Event::Closed)
            {
                win.close();
            }
            if(ev.type == Event::MouseButtonPressed)
            {
                Vector2i mousePos = Mouse::getPosition(win);
                #pragma region FixingMousPos
                if(mousePos.x > 400)
                {
                    mousePos.x = 390;
                }
                else if (mousePos.x < 0)
                {
                    mousePos.x = 0;
                }
                
                if(mousePos.y > 400)
                {
                    mousePos.y = 390;
                }
                else if (mousePos.y < 0)
                {
                    mousePos.y = 0;
                }
                #pragma endregion
                mousePos.x -= 2.5;
                mousePos.y -= 2.5;
                targets.push_back(Vector2f(mousePos.x, mousePos.y));
            }
           
        }


        if(targets.size() != 0)
        {
            if(distance(targets[0], currPos) <= 5)
            {
                targets.erase(targets.begin());
            }
            else
            {
                Vector2f dirVector = Vector2f(targets[0].x - currPos.x, targets[0].y - currPos.y);
                int mag = (int)sqrtf(pow(dirVector.x, 2) + pow(dirVector.y, 2));
                if(mag != 0)
                {
                    dirVector.x /= mag/vel;
                    dirVector.y /= mag/vel;
                }
                finalPos.x += dirVector.x * deltaTime * vel;
                finalPos.y += dirVector.y * deltaTime * vel;
            }
        }
        else if (distance(Vector2f(200, 200), currPos) > 5)
        {
            Vector2f t(200, 200);
            Vector2f dirVector = Vector2f(t.x - currPos.x, t.y - currPos.y);
            int mag = (int)sqrtf(pow(dirVector.x, 2) + pow(dirVector.y, 2));
            if(mag != 0)
            {
                dirVector.x /= mag/vel;
                dirVector.y /= mag/vel;
            }
            finalPos.x += dirVector.x * deltaTime * vel;
            finalPos.y += dirVector.y * deltaTime * vel;
        }
        


        player.setPosition(finalPos);

        if(finalPos != currPos)
        {
        
            float dx = finalPos.x - currPos.x;
            float dy = finalPos.y - currPos.y;

            float angle = (atan2(dy, dx)) * 180/PI;

            player.setRotation(90 + angle);

        }

        win.clear(Color(135, 206, 235));
        win.draw(player);
           
        for(auto i : targets)
            {
                target.setPosition(Vector2f(i.x, i.y));
                win.draw(target);
            }
        win.display();
        
    }
    

    return 0;

}