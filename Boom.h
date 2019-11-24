#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Boom : public GameObject
{
public:
  GLfloat Period;
  GLint Tick;
  Boom()
  : GameObject() {}
  Boom(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Period(1), Tick(0) { }

  // virtual void UpdateEnemy(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time) { };
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
