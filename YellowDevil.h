#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Player.h"
#include "Wheel.h"

class YellowDevil : public Player
{
public:
  YellowDevil()
  : Player() {}
  YellowDevil(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : Player(pos, size, sprite, velocity), Wheel_1(glm::vec2(pos.x + 1.0f,pos.y + 14.0f), glm::vec2(5.8f, 13.8f), sprite, velocity), Wheel_2(glm::vec2(pos.x + size.x - 6.8f, pos.y + 14.0f), glm::vec2(5.8f, 13.8f), sprite, velocity), BulletDamage(50), BulletVelocity(0, -400), HitPoints(400), RotateLeft(GL_FALSE), RotateRight(GL_FALSE) { }

  GLboolean GetRotateLeft() { return this->RotateLeft; }
  GLboolean GetRotateRight() { return this->RotateRight; }
  GLfloat GetBulletVelocityX() { return this->BulletVelocity.x; }
  GLfloat GetBulletVelocityY() { return this->BulletVelocity.y; }
  glm::vec2 GetBulletVelocity() { return this->BulletVelocity; }
  GLfloat GetHitPoints() { return this->HitPoints; }
  GLfloat GetBulletDamage() { return this->BulletDamage; }
  void SetRotateLeft(GLboolean rot) { this->RotateLeft = rot; }
  void SetRotateRight(GLboolean rot) { this->RotateRight = rot; }
  void SetHitPoints(GLfloat hitPoints) { this->HitPoints = hitPoints; }
  void SetBulletDamage(GLfloat bulletDamage) { this->BulletDamage = bulletDamage; }
  void SetBulletVelocity(glm::vec2 bulletVelocity) { this->BulletVelocity = bulletVelocity; }
  void UpdateHitPoints(GLfloat damage) { this->HitPoints -= damage; }
  void SetWheelTexture()
  {
    this->Wheel_1.SetTexture(ResourceManager::GetTexture("wheel"));
    this->Wheel_2.SetTexture(ResourceManager::GetTexture("wheel"));
  }
  void DrawWheel_1(SpriteRenderer &Renderer)
  {
    this->Wheel_1.Draw(Renderer);
  }
  void DrawWheel_2(SpriteRenderer &Renderer)
  {
    this->Wheel_2.Draw(Renderer);
  }
  virtual void UpdatePlayer(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Height, GLfloat Time) override
  {
    if (this->Position.x <= WidthLeft)
      this->Position.x = WidthLeft;
    if (this->Position.x >= WidthRight - this->Size.x)
      this->Position.x = WidthRight - this->Size.x;

    if (this->RotateLeft && this->Rotation > glm::radians(-4.0f) && this->Position.x > WidthLeft)
    {
      this->Rotation += glm::radians(-10.0f) * dt;
      this->Wheel_1.UpdateRotation(glm::radians(-80.0f) * dt);
      this->Wheel_2.UpdateRotation(glm::radians(-80.0f) * dt);
      this->Wheel_1.UpdateDelta(-8.0f * dt);
      this->Wheel_2.UpdateDelta(-1.0f * dt);
    }
    else if (this->RotateRight && this->Rotation < glm::radians(4.0f) && this->Position.x < WidthRight - this->Size.x)
    {
      this->Rotation += glm::radians(10.0f) * dt;
      this->Wheel_1.UpdateRotation(glm::radians(80.0f) * dt);
      this->Wheel_2.UpdateRotation(glm::radians(80.0f) * dt);
      this->Wheel_1.UpdateDelta(1.0f * dt);
      this->Wheel_2.UpdateDelta(8.0f * dt);
    }
    else if (this->Rotation > 0 || this->Position.x >= WidthRight - this->Size.x)
    {
      this->Rotation -= glm::radians(10.0f) * dt;
      this->Wheel_1.UpdateRotation(-glm::radians(80.0f) * dt);
      this->Wheel_2.UpdateRotation(-glm::radians(80.0f) * dt);
      this->Wheel_1.UpdateDelta(-1.0f * dt);
      this->Wheel_2.UpdateDelta(-8.0f * dt);
      if (this->Rotation <= 0)
      {
        this->Rotation = 0;
      }
    }
    else if (this->Rotation < 0 || this->Position.x <= WidthLeft)
    {
      this->Rotation += glm::radians(10.0f) * dt;
      this->Wheel_1.UpdateRotation(glm::radians(80.0f) * dt);
      this->Wheel_2.UpdateRotation(glm::radians(80.0f) * dt);
      this->Wheel_1.UpdateDelta(8.0f * dt);
      this->Wheel_2.UpdateDelta(1.0f * dt);
    }
    if (this->Rotation <= 0 + glm::radians(10.0f) * dt / 2 && this->Rotation >= 0 - glm::radians(10.0f) * dt / 2)
    {
      this->Rotation = 0;
      this->Wheel_1.SetRotation(0);
      this->Wheel_2.SetRotation(0);
      this->Wheel_1.SetDelta(0);
      this->Wheel_2.SetDelta(0);
    }
    this->Wheel_1.SetPositionX(this->Position.x + 1.0f + this->Wheel_1.GetDelta());
    this->Wheel_1.SetPositionY(this->Position.y + 14.0f);
    this->Wheel_2.SetPositionX(this->Position.x + this->Size.x - 6.8f + this->Wheel_2.GetDelta());
    this->Wheel_2.SetPositionY(this->Position.y + 14.0f);
    if(this->Open == GL_TRUE)
    {
      if (!this->StartGun)
      {
        this->PeriodBullet = Time;
        this->StartGun = GL_TRUE;
      }
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
      this->SetTexture(ResourceManager::GetTexture("car"));
      this->GunEnable = GL_FALSE;
    }
    if(this->Tick == 1)
      this->SetTexture(ResourceManager::GetTexture("car1"));
    if(this->Tick == 2)
    {
      this->SetTexture(ResourceManager::GetTexture("car2"));
    }
    if(this->Tick == 3)
    {
      this->SetTexture(ResourceManager::GetTexture("car3"));
      this->GunEnable = GL_FALSE;
    }
    if(this->Tick == 4)
    {
      this->SetTexture(ResourceManager::GetTexture("car4"));
    }
    if(this->Tick == 5)
    {
      this->SetTexture(ResourceManager::GetTexture("car5"));
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
private:
  GLboolean RotateLeft;
  GLboolean RotateRight;
  glm::vec2 BulletVelocity;
  GLfloat HitPoints;
  GLfloat BulletDamage;
  Wheel Wheel_1;
  Wheel Wheel_2;
};
