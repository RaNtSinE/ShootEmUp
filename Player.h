#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Player : public GameObject
{
public:

  GLboolean Knockback;
  GLfloat TimeInvincible;
  GLboolean Invincible;

  Player()
  : GameObject() {}
  Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Knockback(GL_FALSE), Invincible(GL_FALSE), TimeInvincible(0) { }
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
