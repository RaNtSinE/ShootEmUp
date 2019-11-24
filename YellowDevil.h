#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Player.h"

class YellowDevil : public Player
{
public:
  glm::vec2 BulletVelocity;
  GLfloat BulletDamage;
  GLfloat HitPoints;
  YellowDevil()
  : Player() {}
  YellowDevil(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : Player(pos, size, sprite, velocity), BulletDamage(50), BulletVelocity(0, -400), HitPoints(400) { }
  virtual void UpdatePlayer(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Height, GLfloat Time) override
  {
    if (this->Position.x <= WidthLeft)
      this->Position.x = WidthLeft;
    if (this->Position.x >= WidthRight - this->Size.x)
      this->Position.x = WidthRight - this->Size.x;
    if(this->Open == GL_TRUE)
    {
      if(Time / this->PeriodOpen >= 1)
      {
        this->PeriodOpen += 5 * dt;
        if(this->Tick < 5)
          this->Tick++;
      }
    }
    else
    {
      if(Time / this->PeriodOpen >= 1)
      {
        this->PeriodOpen += 5 * dt;
        if(this->Tick > 0)
          this->Tick--;
      }
    }
    if(this->Tick == 0)
    {
      this->Sprite = ResourceManager::GetTexture("car");
      this->GunEnable = GL_FALSE;
    }
    if(this->Tick == 1)
      this->Sprite = ResourceManager::GetTexture("car1");
    if(this->Tick == 2)
    {
      this->Sprite = ResourceManager::GetTexture("car2");
    }
    if(this->Tick == 3)
    {
      this->Sprite = ResourceManager::GetTexture("car3");
      this->GunEnable = GL_FALSE;
    }
    if(this->Tick == 4)
    {
      this->Sprite = ResourceManager::GetTexture("car4");
    }
    if(this->Tick == 5)
    {
      this->Sprite = ResourceManager::GetTexture("car5");
      this->GunEnable = GL_TRUE;
    }
    if(this->Knockback == GL_TRUE)
    {

      if (this->Position.x >= WidthLeft && this->Position.x <= (WidthRight - this->Size.x))
        this->Position.x += this->KnockbackVelocity.x * dt;
      if (this->Position.y >= 0 && this->Position.y <= (Height - this->Size.y))
        this->Position.y += this->KnockbackVelocity.y * dt;
      this->KnockbackVelocity.x -= this->KnockbackVelocity.x * 4 * dt;
      this->KnockbackVelocity.y -= this->KnockbackVelocity.y * 4 * dt;
      if (this->KnockbackVelocity.x <= 1 && this->KnockbackVelocity.x >= -1)
      {
        this->KnockbackVelocity.x = 0;
      }
      if (this->KnockbackVelocity.y <= 1 && this->KnockbackVelocity.y >= -1)
      {
        // this->Knockback = GL_FALSE;
        this->KnockbackVelocity.y = 0;
      }
      if (this->KnockbackVelocity.x == 0 && this->KnockbackVelocity.y == 0)
      {
        this->Knockback = GL_FALSE;
      }
    }
    if (Time >= TimeInvincible)
    {
      this->Invincible = GL_FALSE;
    }

  }
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
