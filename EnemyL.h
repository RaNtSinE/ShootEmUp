#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Enemy.h"

class EnemyL : public Enemy
{
public:
  EnemyL()
  : Enemy() {}
  EnemyL(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
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
    this->Period += dt;
    if (this->Position.y > Height / 5 && this->Period < 10)
      this->Velocity.y -= 40 * dt;
    if (this->Velocity.y <= 0 && this->Period < 10)
      this->Velocity.y = 0;
    if (this->Period >= 10)
      this->Velocity.y -= 60 * dt;
    if (this->Velocity.y <= 60 && this->Position.x <= WidthRight - this->Size.x && this->Position.x >= Width / 2)
    {
      this->Velocity.x -= 50 * dt;
    }
    if (this->Velocity.y <= 60 && this->Position.x >= WidthLeft && this->Position.x <= Width / 2)
    {
      this->Velocity.x += 50 * dt;
    }
    this->Position.y += this->Velocity.y * 1.7 * dt;
    if (this->Position.x >= (WidthLeft + this->Size.x) && this->Position.x <= (WidthRight - this->Size.x) && this->Position.y > 0)
      this->Position.x += this->Velocity.x * dt;
    if (this->Position.x <= (WidthLeft + this->Size.x))
      this->Position.x = WidthLeft + this->Size.x;
    if (this->Position.x >= WidthRight - this->Size.x)
      this->Position.x = WidthRight - this->Size.x;

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
      glm::vec2 velocity1(0.0f, 0.0f);
      glm::vec2 velocity2(0.0f, 0.0f);
      glm::vec2 velocity3(0.0f, 0.0f);
      glm::vec2 velocity4(0.0f, 0.0f);
      // if (this->BulletVelocity * this->BulletVelocity - velocity1.x * velocity1.x < 0)
      // {
      //   velocity1.y = - velocity1.y;
      // }
      // if (this->BulletVelocity * this->BulletVelocity - velocity2.x * velocity2.x < 0)
      // {
      //   velocity2.y = - velocity2.y;
      // }
      if (this->Position.y > PlayerPosy)
      {
        velocity.y = -velocity.y;
        // velocity1.y = -velocity1.y;
        // velocity2.y = -velocity2.y;
      }
      if (abs(velocity.x) <= abs(velocity.y))
      {
        velocity1.x = velocity.x + velocity.x * 0.5;
        velocity1.y = sqrt(this->BulletVelocity * this->BulletVelocity - velocity1.x * velocity1.x);
        velocity2.x = velocity.x - velocity.x * 0.5;
        velocity2.y = sqrt(this->BulletVelocity * this->BulletVelocity - velocity2.x * velocity2.x);
        velocity3.x = velocity.x + velocity.x * 1;
        velocity3.y = sqrt(this->BulletVelocity * this->BulletVelocity - velocity3.x * velocity3.x);
        velocity4.x = velocity.x - velocity.x * 1;
        velocity4.y = sqrt(this->BulletVelocity * this->BulletVelocity - velocity4.x * velocity4.x);
        if (this->Position.y > PlayerPosy)
        {
          velocity1.y = -velocity1.y;
          velocity2.y = -velocity2.y;
          velocity3.y = -velocity3.y;
          velocity4.y = -velocity4.y;
        }
      }
      if (abs(velocity.x) > abs(velocity.y))
      {
        velocity1.y = velocity.y + velocity.y * 0.5;
        velocity1.x = sqrt(this->BulletVelocity * this->BulletVelocity - velocity1.y * velocity1.y);
        velocity2.y = velocity.y - velocity.y * 0.5;
        velocity2.x = sqrt(this->BulletVelocity * this->BulletVelocity - velocity2.y * velocity2.y);
        velocity3.y = velocity.y + velocity.y * 1;
        velocity3.x = sqrt(this->BulletVelocity * this->BulletVelocity - velocity3.y * velocity3.y);
        velocity4.y = velocity.y - velocity.y * 1;
        velocity4.x = sqrt(this->BulletVelocity * this->BulletVelocity - velocity4.y * velocity4.y);
        if (this->Position.x > PlayerPosx)
        {
          velocity1.x = -velocity1.x;
          velocity2.x = -velocity2.x;
          velocity3.x = -velocity3.x;
          velocity4.x = -velocity4.x;
        }
      }


      if (!this->Destroyed)
      {
        Bullets->push_back
        (
          Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity, GL_FALSE, GL_TRUE)
        );
        Bullets->push_back
        (
          Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity1, GL_FALSE, GL_TRUE)
        );
        Bullets->push_back
        (
          Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity2, GL_FALSE, GL_TRUE)
        );
        Bullets->push_back
        (
          Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity3, GL_FALSE, GL_TRUE)
        );
        Bullets->push_back
        (
          Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity4, GL_FALSE, GL_TRUE)
        );
      }
      this->PeriodBullet += 100 * dt;
    }
  }
};
