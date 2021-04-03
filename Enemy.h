#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Bullet.h"

class Enemy : public GameObject
{
public:
  Enemy()
  : GameObject() {}
  Enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), HitPoints(hitPoints), MaxHitPoints(hitPoints), Type(type), BulletVelocity(400), BulletDamage(40), Knockback(GL_FALSE), Period(1), Damage(GL_FALSE), PeriodDamage(0), PeriodBullet(1), OnScreen(GL_FALSE), KnockbackVelocity(0.0f), Boss(GL_FALSE) { }

  GLfloat GetTime() { return this->Time; }
  GLfloat GetPeriodBullet() { return this->PeriodBullet; }
  GLfloat GetHitPoints() { return this->HitPoints; }
  GLfloat GetMaxHitPoints() { return this->MaxHitPoints; }
  GLfloat GetBulletVelocity() { return this->BulletVelocity; }
  GLfloat GetBulletDamage() { return this->BulletDamage; }
  GLboolean GetKnockback() { return this->Knockback; }
  GLint GetType() { return this->Type; }
  GLfloat GetPeriod() { return this->Period; }
  GLboolean GetDamage() { return this->Damage; }
  GLfloat GetPeriodDamage() { return this->PeriodDamage; }
  GLboolean GetOnScreen() { return this->OnScreen; }
  GLboolean GetBoss() { return this->Boss; }
  GLint GetMoney() { return this->Money; }
  void UpdateHitPoints(GLfloat damage) { this->HitPoints -= damage; }
  void SetKnockback(GLboolean knockback) { this->Knockback = knockback; }
  void SetDamage(GLboolean damage) { this->Damage = damage; }
  void SetKnockbackVelocity(glm::vec2 knockbackVelocity) { this->KnockbackVelocity = knockbackVelocity;  }

  virtual void UpdateEnemy(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time) { };
  virtual void SpawnBullets(GLfloat Time, GLfloat dt, GLfloat PlayerPosx, GLfloat PlayerPosy, std::vector<Bullet> * Bullets) { };

  virtual void UpdateKnockback(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight)
  {
	  if (this->GetKnockback())
	  {
		  if (this->GetPositionX() >= WidthLeft && this->GetPositionX() <= (WidthRight - this->GetSizeX()))
			  this->UpdatePositionX(-KnockbackVelocity.x * dt);
		  this->UpdatePositionY(-this->KnockbackVelocity.y * dt);
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
			  this->SetKnockback(GL_FALSE);
		  }
	  }
  }

protected:
	glm::vec2 KnockbackVelocity;
  GLfloat Time;
  GLfloat PeriodBullet;
  GLfloat HitPoints;
  GLfloat MaxHitPoints;
  GLfloat BulletVelocity;
  GLfloat BulletDamage;
  GLboolean Knockback;
  GLint Type;
  GLint Money;
  GLfloat Period;
  GLboolean Damage;
  GLfloat PeriodDamage;
  GLboolean OnScreen;
  GLboolean Boss;
};
