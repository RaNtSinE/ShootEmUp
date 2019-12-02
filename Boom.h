#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Boom : public GameObject
{
public:
  Boom()
  : GameObject() {}
  Boom(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Period(1), Tick(0) { }
  
  GLfloat GetPeriod() { return this->Period; }
  GLint GetTick() { return this->Tick; }
  void UpdateBoom() { this->Tick++; }
private:
  GLfloat Period;
  GLint Tick;
};
