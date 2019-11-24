#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Enemy.h"

class EnemyD : public Enemy
{
public:

  GLfloat Period;
  EnemyD()
  : Enemy(), Period(0) {}
  EnemyD(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
  : Enemy(pos, size, sprite, velocity, hitPoints, type), Period(0) { }
  virtual void UpdateEnemy(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time) override
  {
    if (this->Damage == GL_TRUE)
    {
      this->PeriodDamage = 0.05;
      this->Damage = GL_FALSE;
    }
    if (this->PeriodDamage > 0)
    {
      this->PeriodDamage -= dt;
      this->Sprite = ResourceManager::GetTexture("redcar5damage");
    }
    else
      this->Sprite = ResourceManager::GetTexture("redcar5");
    this->Position.y += this->Velocity.y * dt;
    if (this->Position.x >= (WidthLeft + this->Size.x) && this->Position.x <= (WidthRight - this->Size.x) && this->Position.y > 0)
    {
      // this->Position.x += sin(Time) * 150 * dt;
      this->Position.x += this->Velocity.x;
      if (this->Position.x <= Width / 2 - this->Size.x)
      {
        this->Velocity.x += 50 * dt;
      }
      if (this->Position.x > Width / 2)
      {
        this->Velocity.x -= 50 * dt;
      }
      if (this->Position.x < (WidthLeft + this->Size.x))
        this->Position.x = WidthLeft + this->Size.x;
      if (this->Position.x > (WidthRight - this->Size.x))
        this->Position.x = WidthRight - this->Size.x;
    }
  }
};
