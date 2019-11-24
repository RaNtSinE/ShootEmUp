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
  GLboolean Knockback;
  GLint Type;
  GLfloat Period;
  GLboolean Damage;
  GLfloat PeriodDamage;
  Enemy()
  : GameObject() {}
  Enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), HitPoints(hitPoints),Type(type), BulletVelocity(200), BulletDamage(40), Knockback(GL_FALSE), Period(1), Damage(GL_FALSE), PeriodDamage(0) { }
  virtual void UpdateEnemy(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time) { };
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
