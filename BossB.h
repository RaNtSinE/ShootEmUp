#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Enemy.h"

class BossB : public Enemy
{
public:
	BossB()
		: Enemy() {}
	BossB(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity, GLfloat hitPoints, GLint type)
		: Enemy(pos, size, sprite, velocity, hitPoints, type), Move(0), SwitchMove(GL_FALSE), PeriodBuf(2), LastMove(3), MoveRight(GL_TRUE), MoveLeft(GL_FALSE), MoveUp(GL_TRUE), MoveDown(GL_FALSE), ActiveMove(GL_FALSE), BulletActive(GL_FALSE), Shoot(GL_FALSE), NumberShoot(0), PlayerPos(0.0f) {
		this->Boss = GL_TRUE; this->Money = 600; this->MaxHitPoints = hitPoints;
	}
	virtual void UpdateEnemy(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time) override
	{
		Texture2D myTexture;
		if (this->Damage == GL_TRUE)
		{
			this->PeriodDamage = 0.05;
			this->Damage = GL_FALSE;
		}
		if (this->PeriodDamage > 0)
		{
			this->PeriodDamage -= dt;
			this->SetTexture(ResourceManager::GetTexture("bossadamage"));
		}
		else
			this->SetTexture(ResourceManager::GetTexture("bossa"));

		this->Period += dt;
		if (this->Move == 0)
		{
			if (!this->ActiveMove)
			{
				this->PeriodBuf = 2;
				this->ActiveMove = GL_TRUE;
			}
			if (this->Position.y > Height / 5 && this->Velocity.y > 0)
			{
				this->Velocity.y -= 80 * dt;
				this->PeriodBuf = 2;
			}
			if (this->Velocity.y <= 0)
			{
				this->Velocity.y = 0;
				this->PeriodBuf -= dt;
			}
			if (this->PeriodBuf <= 0)
			{
				//this->GenetrateMove(5);
				if (this->HitPoints/this->MaxHitPoints >= 0.4)
					do
					{
						this->Move = this->GenerateMove(2);

					} while (this->Move == this->LastMove);
				else
				{
					this->Move = 3;
				}
				//this->Move = 1;
				//this->Move = 2;
				this->ActiveMove = GL_FALSE;
				this->BulletActive = GL_FALSE;
			}
		}
		if (this->Move == 1)
		{
			if (!this->ActiveMove)
			{
				this->PeriodBuf = 15;
				this->ActiveMove = GL_TRUE;
			}
			this->PeriodBuf -= dt;
			if (this->PeriodBuf > 0)
			{
					if (this->Position.y <= Height / 6 && this->Position.y >= 0)
					{
						this->MoveDown = GL_TRUE;
						this->MoveUp = GL_FALSE;
					}
					if (this->Position.y >= Height / 4 - this->Size.y && this->Position.y <= Height - this->Size.y)
					{
						this->MoveUp = GL_TRUE;
						this->MoveDown = GL_FALSE;
					}
					if (this->Position.x <= WidthRight - this->Size.x && this->Position.x >= Width - Width / 2.3)
					{
						this->MoveLeft = GL_TRUE;
						this->MoveRight = GL_FALSE;
					}
					if (this->Position.x >= WidthLeft + this->Size.x && this->Position.x <= Width / 2.3)
					{
						this->MoveRight = GL_TRUE;
						this->MoveLeft = GL_FALSE;
					}
					if (this->MoveLeft)
					{
						this->Velocity.x -= 100 * dt;
					}
					if (this->MoveRight)
					{
						this->Velocity.x += 100 * dt;
					}
					if (this->MoveUp)
					{
						this->Velocity.y -= 100 * dt;
					}
					if (this->MoveDown)
					{
						this->Velocity.y += 100 * dt;
					}
					if (this->Position.x >= WidthRight - this->Size.x * 2 - 1)
						this->Velocity.x = -1;
					if (this->Position.x <= (WidthLeft + this->Size.x + 1))
						this->Velocity.x = 1;
					if (this->Position.y <= 10)
					{
						this->Velocity.y = 1;
						this->MoveDown = GL_TRUE;
					}
					if (this->Position.y >= Height - this->Size.y - 1)
					{
						this->Velocity.y = -1;
						this->MoveUp = GL_TRUE;
					}
			}
			if (this->PeriodBuf <= 0)
			{
				if (this->Position.x >= Width / 2)
				{
					this->Velocity.x -= 100 * dt;
				}
				if (this->Position.x <= Width / 2)
				{
					this->Velocity.x += 100 * dt;
				}
				if (this->Position.x > Width / 2.2 && this->Position.x < Width - Width / 2.2)
				{
					this->Velocity.x = 0;
					if (this->Position.y < Height / 2)
						this->Velocity.y = 140;
					if (this->Position.y >= Height / 2)
						this->Velocity.y = -140;
					this->LastMove = this->Move;
					this->Move = 0;
					this->ActiveMove = GL_FALSE;
					this->BulletActive = GL_FALSE;
				}
			}
		}
		if (this->Move == 2)
		{
			if (!this->ActiveMove)
			{
				this->PeriodBuf = 15;
				this->ActiveMove = GL_TRUE;
			}
			this->PeriodBuf -= dt;
			if (this->PeriodBuf > 0)
			{
				if (this->Position.y >= Height - Height / 6 && this->Position.y <= Height - this->Size.y)
				{
					this->MoveDown = GL_FALSE;
					this->MoveUp = GL_TRUE;
				}
				if (this->Position.y <= Height - Height / 4 - this->Size.y && this->Position.y >= 0)
				{
					this->MoveUp = GL_FALSE;
					this->MoveDown = GL_TRUE;
				}
				if (this->Position.x <= WidthRight - this->Size.x && this->Position.x >= Width - Width / 2.3)
				{
					this->MoveLeft = GL_TRUE;
					this->MoveRight = GL_FALSE;
				}
				if (this->Position.x >= WidthLeft + this->Size.x && this->Position.x <= Width / 2.3)
				{
					this->MoveRight = GL_TRUE;
					this->MoveLeft = GL_FALSE;
				}
				if (this->MoveLeft)
				{
					this->Velocity.x -= 100 * dt;
				}
				if (this->MoveRight)
				{
					this->Velocity.x += 100 * dt;
				}
				if (this->MoveUp)
				{
					this->Velocity.y -= 100 * dt;
				}
				if (this->MoveDown)
				{
					this->Velocity.y += 100 * dt;
				}
				if (this->Position.x >= WidthRight - this->Size.x * 2 - 1)
					this->Velocity.x = -1;
				if (this->Position.x <= (WidthLeft + this->Size.x + 1))
					this->Velocity.x = 1;
				if (this->Position.y <= 10)
				{
					this->Velocity.y = 1;
					this->MoveDown = GL_TRUE;
				}
				if (this->Position.y >= Height - this->Size.y - 1)
				{
					this->Velocity.y = -1;
					this->MoveUp = GL_TRUE;
				}
			}
			if (this->PeriodBuf <= 0)
			{
				if (this->Position.x >= Width / 2)
				{
					this->Velocity.x -= 100 * dt;
				}
				if (this->Position.x <= Width / 2)
				{
					this->Velocity.x += 100 * dt;
				}
				if (this->Position.x > Width / 2.2 && this->Position.x < Width - Width / 2.2)
				{
					this->Velocity.x = 0;
					if (this->Position.y < Height / 2)
						this->Velocity.y = 140;
					if (this->Position.y >= Height / 2)
						this->Velocity.y = -140;
					this->LastMove = this->Move;
					this->Move = 0;
					this->ActiveMove = GL_FALSE;
					this->BulletActive = GL_FALSE;
				}
			}
		}
		if (this->Move == 3)
		{
			if (!this->ActiveMove)
			{
				this->PeriodBuf = 20;
				this->ActiveMove = GL_TRUE;
			}
			this->PeriodBuf -= dt;
			if (this->HitPoints / this->MaxHitPoints < 0.4 || this->PeriodBuf > 0)
			{
				float x, y;
				x = this->Position.x - PlayerPos.x;
				y = this->Position.y - PlayerPos.y;
				this->Velocity.x = sqrt((150 * 150 * x * x) / (x * x + y * y));
				this->Velocity.y = abs(y / x * this->Velocity.x);

				if (this->Position.x > PlayerPos.x)
					this->Velocity.x = -this->Velocity.x;
				if (this->Position.y > PlayerPos.y)
				{
					this->Velocity.y = -this->Velocity.y;
				}

				//if (this->MoveLeft)
				//{
				//	this->Velocity.x -= 100 * dt;
				//}
				//if (this->MoveRight)
				//{
				//	this->Velocity.x += 100 * dt;
				//}
				//if (this->MoveUp)
				//{
				//	this->Velocity.y -= 150 * dt;
				//}
				//if (this->MoveDown)
				//{
				//	this->Velocity.y += 150 * dt;
				//}
				if (this->Position.x >= WidthRight - this->Size.x * 2 - 1)
					this->Velocity.x = -1;
				if (this->Position.x <= (WidthLeft + this->Size.x + 1))
					this->Velocity.x = 1;
				if (this->Position.y <= 1)
					this->Velocity.y = 1;
				if (this->Position.y >= Height - this->Size.y - 1)
					this->Velocity.y = -1;
			}
			if (this->HitPoints / this->MaxHitPoints >= 0.4 && this->PeriodBuf < 0)
			{
				if (this->Position.x >= Width / 2)
				{
					this->Velocity.x -= 100 * dt;
				}
				if (this->Position.x <= Width / 2)
				{
					this->Velocity.x += 100 * dt;
				}
				if (this->Position.x > Width / 2.2 && this->Position.x < Width - Width / 2.2)
				{
					this->Velocity.x = 0;
					if (this->Position.y < Height / 2)
						this->Velocity.y = 140;
					if (this->Position.y >= Height / 2)
						this->Velocity.y = -140;
					this->LastMove = this->Move;
					this->Move = 0;
					this->ActiveMove = GL_FALSE;
					this->BulletActive = GL_FALSE;
				}
			}
		}
		this->Position.y += this->Velocity.y * 1.7 * dt;
		if (this->Position.x >= (WidthLeft) && this->Position.x <= (WidthRight - this->Size.x) && this->Position.y > 0)
			this->Position.x += this->Velocity.x * dt;
		if (this->Position.x <= (WidthLeft))
			this->Position.x = WidthLeft;
		if (this->Position.x >= WidthRight - this->Size.x)
			this->Position.x = WidthRight - this->Size.x;
		if (this->Position.y <= 0 && this->PeriodBullet > 5)
			this->Position.y = 10;
		if (this->Position.y >= Height - this->Size.y)
			this->Position.y = Height - this->Size.y;
	}

	virtual void SpawnBullets(GLfloat Time, GLfloat dt, GLfloat PlayerPosx, GLfloat PlayerPosy, std::vector<Bullet>* Bullets) override
	{
		this->PlayerPos = glm::vec2(PlayerPosx, PlayerPosy);
		if (this->Position.y >= 0 && !this->OnScreen)
		{
			this->PeriodBullet = Time + 50 * dt;
			this->OnScreen = GL_TRUE;
		}
		if (this->Move == 1)
		{
			if (!this->BulletActive)
			{
				this->PeriodBullet = Time + 50 * dt;
				this->BulletActive = GL_TRUE;
			}
			if (Time / this->PeriodBullet >= 1 && this->OnScreen)
			{
				glm::vec2 bulPos = glm::vec2(this->Position.x + this->Size.x / 2 - 15, this->Position.y + this->Size.y / 1.1);
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
				if (this->Position.y > PlayerPosy)
				{
					velocity.y = -velocity.y;
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
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity1, GL_FALSE, GL_TRUE)
					//);
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity2, GL_FALSE, GL_TRUE)
					//);
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity3, GL_FALSE, GL_TRUE)
					//);
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity4, GL_FALSE, GL_TRUE)
					//);
				}
				this->PeriodBullet += 30 * dt;
			}
		}
		if (this->Move == 2)
		{
			if (!this->BulletActive)
			{
				this->PeriodBullet = Time + 50 * dt;
				this->BulletActive = GL_TRUE;
			}
			if (Time / this->PeriodBullet >= 1 && this->OnScreen)
			{
				glm::vec2 bulPos = glm::vec2(this->Position.x + this->Size.x / 2 - 15, this->Position.y + this->Size.y / 1.1);
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
				if (this->Position.y > PlayerPosy)
				{
					velocity.y = -velocity.y;
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
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity1, GL_FALSE, GL_TRUE)
					//);
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity2, GL_FALSE, GL_TRUE)
					//);
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity3, GL_FALSE, GL_TRUE)
					//);
					//Bullets->push_back
					//(
					//	Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity4, GL_FALSE, GL_TRUE)
					//);
				}
				this->PeriodBullet += 30 * dt;
			}
		}
		if (this->Move == 3)
		{
			if (!this->BulletActive)
			{
				this->PeriodBullet = Time + 50 * dt;
				this->BulletActive = GL_TRUE;
			}
			if (Time / this->PeriodBullet >= 1 && this->OnScreen)
			{
				glm::vec2 bulPos = glm::vec2(this->Position.x + this->Size.x / 2 - 15, this->Position.y + this->Size.y / 1.1);
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
				if (this->Position.y > PlayerPosy)
				{
					velocity.y = -velocity.y;
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
	}

	virtual void UpdateKnockback(GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight)
	{

	}
private:
	glm::vec2 PlayerPos;
	GLint Move;
	GLint LastMove;
	GLint NumberShoot;
	GLboolean SwitchMove;
	GLboolean MoveRight;
	GLboolean MoveLeft;
	GLboolean MoveUp;
	GLboolean MoveDown;
	GLboolean ActiveMove;
	GLboolean BulletActive;
	GLboolean Shoot;
	GLfloat PeriodBuf;
	GLfloat MaxHitPoints;
	GameObject HitBar;
	GameObject HitBarInfo;
	GLint GenerateMove(GLint numberMoves)
	{
		return (rand() % numberMoves + 1);
	}
};

