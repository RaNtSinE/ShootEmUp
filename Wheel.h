#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Wheel : public GameObject
{
public:
  Wheel()
  : GameObject() {}
  Wheel(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Delta(0) { }

  GLfloat GetDelta() { return this->Delta; }
  void SetDelta(GLfloat delta) { this->Delta = delta; }
  void UpdateDelta(GLfloat delta) { this->Delta += delta; }
private:
  GLfloat Delta;
};
