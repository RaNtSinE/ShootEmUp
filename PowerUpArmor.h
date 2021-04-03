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

class PowerUpArmor : public PowerUp
{
public:
  PowerUpArmor(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
    : PowerUp(type, color, duration, position, texture){ }
  void SetDefault(Player& YellowDevilCar) 
  { 
      YellowDevilCar.SetArmor(1);
      YellowDevilCar.SetColor(glm::vec3(1.0f));
      YellowDevilCar.SetHitBarColor(glm::vec3(0.0f, 0.5f, 0.0f));
  };
  void SetBuff(Player& YellowDevilCar)
  {
      YellowDevilCar.SetArmor(4);
      YellowDevilCar.SetColor(glm::vec3(0.7f, 0.7f, 0.8f));
      YellowDevilCar.SetHitBarColor(glm::vec3(0.2f, 0.4f, 0.2f));
  };

private:
};
