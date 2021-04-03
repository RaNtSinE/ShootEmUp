#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "YellowDevil.h"
#include "PowerUp.h"

class PowerUpDamage : public PowerUp
{
public:
    PowerUpDamage(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
        : PowerUp(type, color, duration, position, texture) { }
    void SetDefault(Player& YellowDevilCar)
    {
          YellowDevilCar.SetBulletDamage(YellowDevilCar.GetMaxBulletDamage());
          YellowDevilCar.SetDamageBuff(GL_FALSE);
    };
    void SetBuff(Player& YellowDevilCar)
    {
         YellowDevilCar.SetBulletDamage(YellowDevilCar.GetMaxBulletDamage() * 1.5);
         YellowDevilCar.SetDamageBuff(GL_TRUE);
    };

private:
};
