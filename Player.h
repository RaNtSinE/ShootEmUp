#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Wheel.h"

class Player : public GameObject
{
public:
  Player()
  : GameObject() {}
  Player(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Knockback(GL_FALSE), Invincible(GL_FALSE), TimeInvincible(0),
	  Open(GL_FALSE), PeriodOpen(1), Tick(0), GunEnable(GL_FALSE), KnockbackVelocity(glm::vec2(0.0f, 0.0f)), PeriodBullet(1),
	  StartGun(GL_FALSE), StartOpen(GL_FALSE), BulletSize(30), DamageBuff(GL_FALSE), MaxTimeInInvincible(1), Vampiric(0),
      DamageCost(500), DurabilityCost(400), TimeInInvincibleCost(600), VampiricCost(600) { }

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
  GLboolean GetDamageBuff() { return this->DamageBuff;  }
  GLfloat GetBulletDamage() { return this->BulletDamage; }
  GLfloat GetHitPoints() { return this->HitPoints; }
  GLboolean GetRotateLeft() { return this->RotateLeft; }
  GLboolean GetRotateRight() { return this->RotateRight; }
  GLfloat GetBulletVelocityX() { return this->BulletVelocity.x; }
  GLfloat GetBulletVelocityY() { return this->BulletVelocity.y; }
  glm::vec2 GetBulletVelocity() { return this->BulletVelocity; }
  GLfloat GetMaxHitPoints() { return this->MaxHitPoints; }
  GLfloat GetArmor() { return this->Armor; }
  GLfloat GetMaxBulletDamage() { return this->MaxBulletDamage; }
  GLboolean GetTripleShoot() { return this->TripleShoot; }
  GLfloat GetMaxTimeInInvincible() { return this->MaxTimeInInvincible; }
  GLfloat GetVampiric() { return this->Vampiric; }
  GLint GetDamageCost() { return this->DamageCost; }
  GLint GetDurabilityCost() { return this->DurabilityCost; }
  GLint GetTimeInInvincibleCost() { return this->TimeInInvincibleCost; }
  GLint GetVampiricCost() { return this->VampiricCost; }
  void SetTripleShoot(GLboolean tripleShoot) { this->TripleShoot = tripleShoot; }
  void SetHitPoints(GLfloat hitPoints) { this->HitPoints = hitPoints; }
  void SetBulletDamage(GLfloat bulletDamage) { this->BulletDamage = bulletDamage; }
  void SetBulletVelocity(glm::vec2 bulletVelocity) { this->BulletVelocity = bulletVelocity; }
  void SetArmor(GLfloat armor) { this->Armor = armor; }
  void UpdateShootSpeed(GLfloat shootSpeed) { this->ShootSpeed /= shootSpeed; }
  void SetHitBarColor(glm::vec3 hitBarColor) { this->HitBarColor = hitBarColor; }
  void UpdatePeriodBullet(GLfloat period) { this->PeriodBullet += period; }
  void SetInvincible(GLboolean invincible) { this->Invincible = invincible; }
  void SetTimeInvincible(GLfloat time) { this->TimeInvincible = time; }
  void SetKnockback(GLboolean knockback) { this->Knockback = knockback; }
  void SetKnockbackVelocity(glm::vec2 knockbackVelocity) { this->KnockbackVelocity = knockbackVelocity; }
  void SetPeriodOpen(GLfloat period) { this->PeriodOpen = period; }
  void SetOpen(GLboolean open) { this->Open = open; }
  void SetStartOpen(GLboolean startOpen) { this->StartOpen = startOpen; }
  void SetDamageBuff(GLboolean buff) { this->DamageBuff = buff; }
  void SetRotateLeft(GLboolean rot) { this->RotateLeft = rot; }
  void SetRotateRight(GLboolean rot) { this->RotateRight = rot; }
  void UpgradeTimeInInvincible(GLfloat time) { this->MaxTimeInInvincible += time; }
  void UpgradeVampiric(GLfloat vampiric) { this->Vampiric += vampiric; }
  void UpdateHitPoints(GLfloat damage) { this->HitPoints -= damage / this->Armor; }
  void UpdateVampiric(GLfloat damage) { this->HitPoints += damage; }
  void UpgradeBulletDamage(GLfloat damage) { this->MaxBulletDamage += damage; }
  void UpgradeHitPoints(GLfloat hitPoints) { this->MaxHitPoints += hitPoints; }
  void UpdateDamageCost() { this->DamageCost *= 1.2; }
  void UpdateDurabilityCost() { this->DurabilityCost *= 1.2; }
  void UpdateTimeInInvincibleCost() { this->TimeInInvincibleCost *= 1.25; }
  void UpdateVampiricCost() { this->VampiricCost *= 1.25; }
  void ResetPlayer()
  {
	  this->Knockback = GL_FALSE;
	  this->Invincible = GL_FALSE;
	  this->TimeInvincible = 0;
	  this->Open = GL_FALSE;
	  this->PeriodOpen = 1;
	  this->Tick = 0;
	  this->GunEnable = GL_FALSE;
	  this->KnockbackVelocity = glm::vec2(0.0f, 0.0f);
	  this->PeriodBullet = 1;
	  this->StartGun = GL_FALSE;
	  this->BulletSize = 30;
	  this->DamageBuff = GL_FALSE;

  }

  virtual void ResetCar() { }
  virtual void SetWheelTexture() { }
  virtual void DrawWheel_1(SpriteRenderer& Renderer) { }
  virtual void DrawWheel_2(SpriteRenderer& Renderer) { }
  virtual void DrawHitBar(SpriteRenderer& renderer, GLfloat Width, GLfloat Height) { }
  virtual void UpdatePlayer(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Height, GLfloat Time) { }
  virtual void SpawnBullets(GLfloat Time, GLfloat dt, std::vector<Bullet>* Bullets) { }

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
  GLboolean DamageBuff;
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
  GLfloat BulletSize;

  glm::vec3 HitBarColor;
  GLboolean RotateLeft;
  GLboolean RotateRight;
  GLboolean TripleShoot;
  glm::vec2 BulletVelocity;
  GLfloat HitPoints;
  GLfloat MaxHitPoints;
  GLfloat BulletDamage;
  GLfloat MaxBulletDamage;
  GLfloat ShootSpeed;
  GLfloat Armor;
  GLfloat MaxTimeInInvincible;
  GLfloat Vampiric;
  GLint DamageCost;
  GLint DurabilityCost;
  GLint TimeInInvincibleCost;
  GLint VampiricCost;

  Wheel Wheel_1;
  Wheel Wheel_2;
};
