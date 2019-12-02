#pragma once
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
  PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
    : GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { }

  const std::string &GetType() const { return this->Type; }
  GLfloat GetDuration() { return this->Duration; }
  GLboolean GetActivated() const { return this->Activated; }
  void UpdateDuration(GLfloat dt) { this->Duration -= dt; }
  void SetActivated(GLboolean activated) { this->Activated = activated; }

private:
  std::string Type;
  GLfloat Duration;
  GLboolean Activated;
};
