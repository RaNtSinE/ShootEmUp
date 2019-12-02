#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Player : public GameObject
{
public:
  Player()
  : GameObject() {}
  Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Knockback(GL_FALSE), Invincible(GL_FALSE), TimeInvincible(0), Open(GL_FALSE), PeriodOpen(1), Tick(0), GunEnable(GL_FALSE), KnockbackVelocity(glm::vec2(0.0f, 0.0f)), PeriodBullet(1), StartGun(GL_FALSE), StartOpen(GL_FALSE) { }

  GLboolean GetKnockback() { return this->Knockback; }
  GLfloat GetTimeInvincible() { return this->TimeInvincible; }
  GLboolean GetInvincible() { return this->Invincible; }
  GLboolean GetOpen() { return this->Open; }
  GLfloat GetPeriodOpen() { return this->PeriodOpen; }
  GLint GetTick() { return this->Tick; }
  GLboolean GetGunEnable() { return this->GunEnable; }
  GLfloat GetKnockbackVelocityX() { return this->KnockbackVelocity.x; }
  GLfloat GetKnockbackVelocityY() { return this->KnockbackVelocity.y; }
  glm::vec2 GetKnockbackVelocity() { return this->KnockbackVelocity; }
  GLfloat GetPeriodBullet() { return this->PeriodBullet; }
  GLboolean GetStartGun() { return this->StartGun; }
  GLboolean GetStartOpen() { return this->StartOpen; }
  void UpdatePeriodBullet(GLfloat period) { this->PeriodBullet += period; }
  void SetInvincible(GLboolean invincible) { this->Invincible = invincible; }
  void SetTimeInvincible(GLfloat time) { this->TimeInvincible = time; }
  void SetKnockback(GLboolean knockback) { this->Knockback = knockback; }
  void SetKnockbackVelocity(glm::vec2 knockbackVelocity) { this->KnockbackVelocity = knockbackVelocity; }
  void SetPeriodOpen(GLfloat period) { this->PeriodOpen = period; }
  void SetOpen(GLboolean open) { this->Open = open; }
  void SetStartOpen(GLboolean startOpen) { this->StartOpen = startOpen; }


  virtual void UpdatePlayer(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Height, GLfloat Time) { }
  void MoveLeft(GLfloat velocityX)
  {
    this->Position.x -= velocityX;
  }
  void MoveRight(GLfloat velocityX)
  {
    this->Position.x += velocityX;
  }
  void MoveForward(GLfloat velocityY)
  {
    this->Position.y -= velocityY;
  }
  void MoveBack(GLfloat velocityY)
  {
    this->Position.y += velocityY;
  }
protected:
  GLboolean Knockback;
  GLfloat TimeInvincible;
  GLboolean Invincible;
  GLboolean Open;
  GLfloat PeriodOpen;
  GLint Tick;
  GLboolean GunEnable;
  glm::vec2 KnockbackVelocity;
  GLfloat PeriodBullet;
  GLboolean StartGun;
  GLboolean StartOpen;
};
