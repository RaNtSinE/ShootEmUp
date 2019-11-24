#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Player : public GameObject
{
public:


  Player()
  : GameObject() {}
  Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity) { }
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
