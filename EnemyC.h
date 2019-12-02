#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Enemy.h"

class EnemyC : public Enemy
{
public:

  EnemyC()
  : Enemy() {}
  EnemyC(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
  : Enemy(pos, size, sprite, velocity, hitPoints, type) { }
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
      this->SetTexture(ResourceManager::GetTexture("redcar5damage"));
    }
    else
      this->SetTexture(ResourceManager::GetTexture("redcar5"));
    this->Position.y += this->Velocity.y * 2 * dt;
    if (this->Position.x > (WidthLeft + this->Size.x) && this->Position.x < (WidthRight - this->Size.x) && this->Position.y > 0)
    {
      this->Position.x -= this->Velocity.y * dt;
    }
  }

  virtual void SpawnBullets(GLfloat Time, GLfloat dt, GLfloat PlayerPosx, GLfloat PlayerPosy, std::vector<Bullet> * Bullets) override
  {
    if(this->Position.y >= 0 && !this->OnScreen)
    {
      this->PeriodBullet = Time + 50 * dt;
      this->OnScreen = GL_TRUE;
    }
    if (Time / this->PeriodBullet >= 1 && this->OnScreen)
    {
      glm::vec2 bulPos = glm::vec2(this->Position.x + this->Size.x / 2 - 15, this->Position.y + this->Size.y / 1.5);
      glm::vec2 velocity(0.0f, 0.0f);
      float x, y;
      x = this->Position.x - PlayerPosx;
      y = this->Position.y - PlayerPosy;
      velocity.x = sqrt((this->BulletVelocity * this->BulletVelocity * x * x) / (x * x + y * y));
      velocity.y = abs(y / x * velocity.x);
      if (this->Position.x > PlayerPosx)
        velocity.x = -velocity.x;
      if (this->Position.y > PlayerPosy)
        velocity.y = -velocity.y;

      if (!this->Destroyed)
      Bullets->push_back
      (
        Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity, GL_FALSE, GL_TRUE)
      );

      this->PeriodBullet += 100 * dt;
    }
  }
};
