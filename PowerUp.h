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

const glm::vec2 SIZE(50, 50);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
  PowerUp(std::string type,glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
    : GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { }

  const std::string &GetType() const { return this->Type; }
  GLfloat GetDuration() { return this->Duration; }
  GLboolean GetActivated() const { return this->Activated; }
 // void AddPowerUp(PowerUp* obj) { this->PowerUps.push_back(obj); }
  const std::vector<PowerUp*>& GetPowerUps() const { return this->PowerUps; }
  void UpdateDuration(GLfloat dt) { this->Duration -= dt; }
  void SetActivated(GLboolean activated) { this->Activated = activated; }
  virtual void SetDefault(Player& YellowDevilCar) { };
  virtual void SetBuff(Player& YellowDevilCar) { };

private:
  std::string Type;
  GLfloat Duration;
  GLboolean Activated;
  std::vector<PowerUp*> PowerUps;
};
