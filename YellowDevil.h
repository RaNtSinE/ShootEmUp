#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <math.h>

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
		: Player(pos, size, sprite, velocity)
	{
		this->BulletDamage = 50;
		this->MaxBulletDamage = 50;
		this->BulletVelocity = glm::vec2(0, -800);
		this->HitPoints = 200;
		this->MaxHitPoints = 200;
		this->RotateLeft = GL_FALSE;
		this->RotateRight = GL_FALSE;
		this->ShootSpeed = 20;
		this->Armor = 1;
		this->TripleShoot = GL_FALSE;
		this->HitBarColor = glm::vec3(0.0f, 0.5f, 0.0f);
		this->Wheel_1 = Wheel(glm::vec2(pos.x + 2.5f, pos.y + 35.0f), glm::vec2(11.6f, 27.6f), sprite, velocity);
		this->Wheel_2 = Wheel(glm::vec2(pos.x + size.x - 13.8f, pos.y + 35.0f), glm::vec2(11.6f, 27.6f), sprite, velocity);
	}


	void ResetCar() 
	{ 
		this->BulletDamage = this->MaxBulletDamage;
		this->BulletVelocity = glm::vec2(0, -800);
		this->HitPoints = this->MaxHitPoints;
		this->RotateLeft = GL_FALSE;
		this->RotateRight = GL_FALSE;
		this->ShootSpeed = 20;
		this->Armor = 1;
		this->TripleShoot = GL_FALSE;
		this->HitBarColor = glm::vec3(0.0, 0.5, 0.0);
	}
	void SetWheelTexture()
	{
		this->Wheel_1.SetTexture(ResourceManager::GetTexture("wheel"));
		this->Wheel_2.SetTexture(ResourceManager::GetTexture("wheel"));
	}
	void DrawWheel_1(SpriteRenderer& Renderer) override
	{
		this->Wheel_1.Draw(Renderer);
	}
	void DrawWheel_2(SpriteRenderer& Renderer) override
	{
		this->Wheel_2.Draw(Renderer);
	}
	void DrawHitBar(SpriteRenderer& renderer, GLfloat Width, GLfloat Height) override
	{
		Texture2D myTexture;
		GLfloat percent = this->GetHitPoints() / this->GetMaxHitPoints();
		myTexture = ResourceManager::GetTexture("infobar");
		renderer.DrawSprite(myTexture, glm::vec2(Width / 8.5 - 195 - percent * 15, 0 + 64), glm::vec2(480 * percent, 120), 0.0f, HitBarColor);

		myTexture = ResourceManager::GetTexture("bar");
		renderer.DrawSprite(myTexture, glm::vec2(Width / 8.5 - 210, 0 + 64), glm::vec2(480, 120), 0.0f, glm::vec3(0.5, 0.5, 0.5));
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
		this->Wheel_1.SetPositionX(this->Position.x + 2.5f + this->Wheel_1.GetDelta());
		this->Wheel_1.SetPositionY(this->Position.y + 35.0f);
		this->Wheel_2.SetPositionX(this->Position.x + this->Size.x - 13.8f + this->Wheel_2.GetDelta());
		this->Wheel_2.SetPositionY(this->Position.y + 35.0f);
		if (this->Open == GL_TRUE)
		{
			if (!this->StartGun)
			{
				this->PeriodBullet = Time;
				this->StartGun = GL_TRUE;
			}
			if (Time / this->PeriodOpen >= 1)
			{
				this->PeriodOpen += 5 * dt;
				if (this->Tick < 5)
					this->Tick++;
			}
		}
		else
		{
			if (Time / this->PeriodOpen >= 1)
			{
				this->PeriodOpen += 5 * dt;
				if (this->Tick > 0)
					this->Tick--;
			}
		}
		if (this->Tick == 0)
		{
			this->SetTexture(ResourceManager::GetTexture("car"));
			this->GunEnable = GL_FALSE;
		}
		if (this->Tick == 1)
			this->SetTexture(ResourceManager::GetTexture("car1"));
		if (this->Tick == 2)
		{
			this->SetTexture(ResourceManager::GetTexture("car2"));
		}
		if (this->Tick == 3)
		{
			this->SetTexture(ResourceManager::GetTexture("car3"));
			this->GunEnable = GL_FALSE;
		}
		if (this->Tick == 4)
		{
			this->SetTexture(ResourceManager::GetTexture("car4"));
		}
		if (this->Tick == 5)
		{
			this->SetTexture(ResourceManager::GetTexture("car5"));
			this->GunEnable = GL_TRUE;
		}
		if (this->Knockback == GL_TRUE)
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
	virtual void SpawnBullets(GLfloat Time, GLfloat dt, std::vector<Bullet>* Bullets) override
	{
		glm::vec2 bulPos(this->GetPositionX() + this->Size.x / 2 - this->BulletSize / 2, this->GetPositionY());
		if (Time / this->GetPeriodBullet() >= 1)
		{
			this->UpdatePeriodBullet(this->ShootSpeed * dt);
			if (this->GetGunEnable())
				if (!this->TripleShoot)
					Bullets->push_back
					(
						Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("bullet"), this->GetBulletVelocity(), GL_TRUE, GL_FALSE)
					);
				else
				{
					glm::vec2 bulletVelocity;
					bulletVelocity.x = 200;
					bulletVelocity.y = sqrt(this->GetBulletVelocityY() * this->GetBulletVelocityY() - bulletVelocity.x * bulletVelocity.x);

					Bullets->push_back
					(
						Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("bullet"), glm::vec2(-bulletVelocity.x, -bulletVelocity.y), GL_TRUE, GL_FALSE)
					);
					Bullets->push_back
					(
						Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("bullet"), this->GetBulletVelocity(), GL_TRUE, GL_FALSE)
					);
					Bullets->push_back
					(
						Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("bullet"), glm::vec2(bulletVelocity.x, -bulletVelocity.y), GL_TRUE, GL_FALSE)
					);
				}
		}
	}
private:
};
