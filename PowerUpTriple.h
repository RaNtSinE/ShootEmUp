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

class PowerUpTriple : public PowerUp
{
public:
    PowerUpTriple(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
        : PowerUp(type, color, duration, position, texture) { }
    void SetDefault(Player& YellowDevilCar)
    {
          YellowDevilCar.SetTripleShoot(GL_FALSE);
    };
    void SetBuff(Player& YellowDevilCar)
    {
         YellowDevilCar.SetTripleShoot(GL_TRUE);
    };

private:
};
