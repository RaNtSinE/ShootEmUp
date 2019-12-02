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
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), HitPoints(hitPoints),Type(type), BulletVelocity(200), BulletDamage(40), Knockback(GL_FALSE), Period(1), Damage(GL_FALSE), PeriodDamage(0), PeriodBullet(1), OnScreen(GL_FALSE) { }

  GLfloat GetTime() { return this->Time; }
  GLfloat GetPeriodBullet() { return this->PeriodBullet; }
  GLfloat GetHitPoints() { return this->HitPoints; }
  GLfloat GetBulletVelocity() { return this->BulletVelocity; }
  GLfloat GetBulletDamage() { return this->BulletDamage; }
  GLboolean GetKnockback() { return this->Knockback; }
  GLint GetType() { return this->Type; }
  GLfloat GetPeriod() { return this->Period; }
  GLboolean GetDamage() { return this->Damage; }
  GLfloat GetPeriodDamage() { return this->PeriodDamage; }
  GLboolean GetOnScreen() { return this->OnScreen; }
  void UpdateHitPoints(GLfloat damage) { this->HitPoints -= damage; }
  void SetKnockback(GLboolean knockback) { this->Knockback = knockback; }
  void SetDamage(GLboolean damage) { this->Damage = damage; }

  virtual void UpdateEnemy(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time) { };
  virtual void SpawnBullets(GLfloat Time, GLfloat dt, GLfloat PlayerPosx, GLfloat PlayerPosy, std::vector<Bullet> * Bullets) { };

protected:
  GLfloat Time;
  GLfloat PeriodBullet;
  GLfloat HitPoints;
  GLfloat BulletVelocity;
  GLfloat BulletDamage;
  GLboolean Knockback;
  GLint Type;
  GLfloat Period;
  GLboolean Damage;
  GLfloat PeriodDamage;
  GLboolean OnScreen;
};
