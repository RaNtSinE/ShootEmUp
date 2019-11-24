#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Enemy.h"

class EnemyF : public Enemy
{
public:

  GLfloat Period;
  EnemyF()
  : Enemy(), Period(0) {}
  EnemyF(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
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
    if (this->Position.y > 0 && this->Position.y < Height / 4)
    {
      this->Velocity.y -= 20 * dt;
    }
    if ((this->Position.y >= Height / 4) && (this->Position.x < WidthRight - this->Size.x * 2))
    {
      this->Velocity.x += 80 * dt;
    }
    if (this->Position.x < WidthRight - this->Size.x && this->Position.x > WidthRight - this->Size.x * 2)
    {
      this->Velocity.x -= 160 * dt;
      this->Velocity.y += 50 * dt;
    }
    this->Position.y += this->Velocity.y * 1.5 * dt;
    if (this->Position.x >= (WidthLeft + this->Size.x) && this->Position.x <= (WidthRight - this->Size.x * 2) && this->Position.y > 0)
      this->Position.x += this->Velocity.x * dt;
  }
};
