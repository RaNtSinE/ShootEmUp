#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Enemy : public GameObject
{
public:

  GLfloat HitPoints;
  GLfloat BulletVelocity;
  GLfloat BulletDamage;
  Enemy()
  : GameObject() {}
  Enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), HitPoints(hitPoints), BulletVelocity(200), BulletDamage(40) { }
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
