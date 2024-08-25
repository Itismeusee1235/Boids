#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace sf;
 
int main()
{
    const float PI = 3.14159265;    
    RenderWindow win(VideoMode(400, 400), "Rotation");
    RectangleShape shape(Vector2f(10,10));
    Texture texture;
    Sprite sprite;
    if(!texture.loadFromFile("S.png"))
    {
        cout << "Failed" << endl;
    }
    
    sprite.setTexture(texture);
    Clock clock;
    sprite.setOrigin(Vector2f(16,16));
    sprite.setPosition(200, 200);

    
    while (win.isOpen())
    {
        Event ev;
        while (win.pollEvent(ev))
        {
            if(ev.type == Event::Closed)
            {
                win.close();
            }
        }

        Vector2i mousePos = Mouse::getPosition(win);
        if(mousePos.x > 400)
        {
            mousePos.x = 400;
        }
        else if (mousePos.x < 0)
        {
            mousePos.x = 0;
        }

        if(mousePos.y > 400)
        {
            mousePos.y = 400;
        }
        else if (mousePos.y < 0)
        {
        mousePos.y = 0;
        }
        
        shape.setPosition(sprite.getPosition());
        float x = mousePos.x-sprite.getPosition().x;
        float y = mousePos.y-sprite.getPosition().y;
        cout << x << " " << y << " ";
        float angle = atan2(y, x)*180/PI;
        cout << angle << endl;
        sprite.setRotation(90+angle);

        win.clear();
        win.draw(sprite);
        win.draw(shape);
        win.display();
        
    }
    

    return 0;
}