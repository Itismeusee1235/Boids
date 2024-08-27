#ifndef ENTITY
#define ENTITY

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#define PI 3.14159265

using namespace sf;
using namespace std;

class Entity
{
    Vector2f pos;
    Vector2f velocity;
    Texture texture;
    Sprite entity;

    public:
    Entity(string file, Vector2f pos, Vector2f velocity=Vector2f(0,0))
    {
        cout << "called" << endl;
        this->pos = pos;
        cout << this->pos.x << " " << this->pos.y << endl;
        this->velocity = velocity;
        if(!texture.loadFromFile(file))
        {
            return;
        }
        this->entity = Sprite(texture);
        
        FloatRect bounds = this->entity.getGlobalBounds();
        this->entity.setOrigin(Vector2f(bounds.width/2, bounds.height/2));
    }

    void Draw(RenderWindow& win)
    {
        this->entity.setPosition(this->pos);
        win.draw(entity);
    }

    void Update(float deltaTime)
    {
        Vector2f finalPos = this->pos;
        finalPos.x += this->velocity.x * deltaTime;
        finalPos.y += this->velocity.y * deltaTime;

        float dx = finalPos.x - this->pos.x;
        float dy = finalPos.y - this->pos.y;
        float angle = atan2(dy, dx)*180/PI + 90;
        this->entity.setRotation(angle);
        this->pos = finalPos;
    }
};

#endif