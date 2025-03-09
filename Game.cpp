#include <algorithm>
#include <irrKlang.h>

#include <iostream>
#include <math.h>

using namespace irrklang;

#include "Game.h"
#include "Resource_manager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "TextRenderer.h"
#include "Player.h"
#include "YellowDevil.h"
#include "RedSnake.h"
#include "Enemy.h"
#include "PowerUpBase.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll



ISoundEngine *SoundEngine = createIrrKlangDevice();
SpriteRenderer *Renderer;
//GameObject *Player;
ParticleGenerator *Particles;
PostProcessor *Effects;
TextRenderer *Text;
PowerUpBase* PowerUps;
//YellowDevil *YellowDevilCar;
irrklang::ISound* snd;
PowerUp* PowerUpUpdate;
GLfloat ShakeTime = 0.0f;
// GLfloat Period = 1;
GLboolean GunEnable;
GLboolean Open;
GLboolean knockback = GL_FALSE;
GLboolean PlayerVisible = GL_FALSE;
GLint Tick;
GLboolean Explore = GL_FALSE;
glm::vec2 KnockbackVelocity(0.0f, 0.0f);

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), MenuState(MENU_START), PauseState(PAUSE_CONTINUE), LoseState(LOSE_TRY_AGAIN),
	OptionsState(MUSIC_VOLUME), WinState(WIN_NEXT_LEVEL), StartState(START_LEVEL), YellowState(YELLOW_NOT),
	RedState(RED_NOT), Keys(), KeysProcessed(), Width(width), Height(height), Level(0),Car(0), Lives(3), Velocity(0), Time(0),
	PeriodBullet(1), PeriodBulletEnemy(1), PeriodOpen(1), PeriodInvincible(1), PeriodBoom(1), WidthLeft(width),
	HeightUp(height), WidthRight(width), HeightDown(height), TimeSpeed(1), SlowTime(GL_FALSE), Pause(GL_FALSE),
	MusicVolume(0.3), SoundVolume(1), Money(10000)
{

}

Game::~Game()
{
	delete Renderer;
	delete Particles;
	delete Effects;
	//delete YellowDevilCar;
	SoundEngine->drop();
}

void Game::Init()
{
	ResourceManager::LoadShader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl", nullptr, "sprite");
	ResourceManager::LoadShader("Shaders/ParticleVertexShader.glsl", "Shaders/ParticleFragmentShader.glsl", nullptr, "particles");
	ResourceManager::LoadShader("Shaders/PostVertexShader.glsl", "Shaders/PostFragmentShader.glsl", nullptr, "postprocessing");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particles").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particles").SetMatrix4("projection", projection);

	ResourceManager::LoadTexture("Images/Road.png", GL_FALSE, "background");
	ResourceManager::LoadTexture("Images/Car.png", GL_TRUE, "car");
	ResourceManager::LoadTexture("Images/Bullet.png", GL_TRUE, "bullet");
	ResourceManager::LoadTexture("Images/EnemyBullet.png", GL_TRUE, "enemyBullet");
	ResourceManager::LoadTexture("Images/Car1.png", GL_TRUE, "car1");
	ResourceManager::LoadTexture("Images/Car2.png", GL_TRUE, "car2");
	ResourceManager::LoadTexture("Images/Car3.png", GL_TRUE, "car3");
	ResourceManager::LoadTexture("Images/Car4.png", GL_TRUE, "car4");
	ResourceManager::LoadTexture("Images/Car5.png", GL_TRUE, "car5");
	ResourceManager::LoadTexture("Images/RedCar.png", GL_TRUE, "redcar");
	ResourceManager::LoadTexture("Images/RedCar1.png", GL_TRUE, "redcar1");
	ResourceManager::LoadTexture("Images/RedCar2.png", GL_TRUE, "redcar2");
	ResourceManager::LoadTexture("Images/RedCar3.png", GL_TRUE, "redcar3");
	ResourceManager::LoadTexture("Images/RedCar4.png", GL_TRUE, "redcar4");
	ResourceManager::LoadTexture("Images/RedCar5.png", GL_TRUE, "redcar5");
	ResourceManager::LoadTexture("Images/RedCar5Damage.png", GL_TRUE, "redcar5damage");
	ResourceManager::LoadTexture("Images/Boom1.png", GL_TRUE, "boom1");
	ResourceManager::LoadTexture("Images/Boom2.png", GL_TRUE, "boom2");
	ResourceManager::LoadTexture("Images/Boom3.png", GL_TRUE, "boom3");
	ResourceManager::LoadTexture("Images/Boom4.png", GL_TRUE, "boom4");
	ResourceManager::LoadTexture("Images/Wheel.png", GL_TRUE, "wheel");
	ResourceManager::LoadTexture("Images/Bar.png", GL_TRUE, "bar");
	ResourceManager::LoadTexture("Images/InfoBar.png", GL_TRUE, "infobar");
	ResourceManager::LoadTexture("Images/BossA.png", GL_TRUE, "bossa");
	ResourceManager::LoadTexture("Images/BossADamage.png", GL_TRUE, "bossadamage");
	//ResourceManager::LoadTexture("Images/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	//ResourceManager::LoadTexture("Images/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("Images/powerup_triple.png", GL_TRUE, "powerup_triple");
	ResourceManager::LoadTexture("Images/powerup_damage.png", GL_TRUE, "powerup_damage");
	ResourceManager::LoadTexture("Images/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("Images/powerup_armor.png", GL_TRUE, "powerup_armor");
	ResourceManager::LoadTexture("Images/Quad.png", GL_TRUE, "quad");

	Shader myShader;

	myShader = ResourceManager::GetShader("sprite");

	Renderer = new SpriteRenderer(myShader);
	PowerUps = new PowerUpBase();

	Particles = new ParticleGenerator(ResourceManager::GetShader("particles"), ResourceManager::GetTexture("particlet"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

	GameLevel one; one.Load("Levels/Level1.lvl");
	GameLevel two; two.Load("Levels/Level2.lvl");
	// GameLevel three; three.Load("level3.lvl");
	// GameLevel four; four.Load("level4.lvl");

	this->Levels.push_back(one);
	this->Levels.push_back(two);
	// this->Levels.push_back(three);
	// this->Levels.push_back(four);
	this->Level = 0;


	glm::vec2 playerPos = glm::vec2(this->Width / 2 + PLAYER_SIZE.x, this->Height - PLAYER_SIZE.y);

	Texture2D myTexture;

	myTexture = ResourceManager::GetTexture("car");

	glm::vec2 velocityDevil = glm::vec2(1000, 600);
	//YellowDevil* obj = new YellowDevil(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 160), 200, this->TypeEnemy[wave]);
	this->Cars.push_back(new YellowDevil(playerPos, PLAYER_SIZE, myTexture, velocityDevil));
	myTexture = ResourceManager::GetTexture("redcar");
	this->Cars.push_back(new RedSnake(playerPos, PLAYER_SIZE, myTexture, velocityDevil));
	this->Car = 0;

	//YellowDevilCar = new YellowDevil(playerPos, PLAYER_SIZE, myTexture, velocityDevil);

	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("fonts/ocraext.TTF", 24);

	this->WidthRight = this->WidthRight * 0.7657;
	this->WidthLeft = this->WidthLeft / 3.763;

	this->Time = 0;
	Tick = 0;
	this->PeriodBullet = this->Time + 1;
	this->PeriodBulletEnemy = this->Time + 1;
	this->PeriodOpen = this->Time + 1;
	this->PeriodInvincible = this->Time + 1;
	this->PeriodBoom = this->Time + 1;
	//YellowDevilCar->SetWheelTexture();
	for (int i = 0; i < this->Cars.size(); i++)
		this->Cars[i]->SetWheelTexture();
}

void Game::Update(GLfloat dt)
{
	if (this->State == GAME_ACTIVE || this->State == GAME_PAUSE)
	{
		if (this->Time == 0)
		{
			snd = SoundEngine->play2D("audio/carpenter_brut_turbo_killer.mp3", GL_TRUE, GL_FALSE, GL_TRUE);
		}
		if (snd)
			snd->setVolume(0.2 * MusicVolume);
		dt *= TimeSpeed;
		this->Time += dt;
		GLfloat dtBuf = dt;
		GLint lines = 1;
		this->DoCollisions(dt);
		// Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
		for (GLuint i = 0; i < this->Levels[this->Level].GetNumberWaves(); ++i)
		{
			if (this->Time >= this->Levels[this->Level].GetTimeSpawn(i) && this->Time < this->Levels[this->Level].GetTimeSpawn(i) + dtBuf)
				this->Levels[this->Level].SpawnEnemys(i, lines);
		}
		//YellowDevilCar->UpdatePlayer(dt, this->WidthLeft, this->WidthRight, this->Height, this->Time);
		this->Cars[this->Car]->UpdatePlayer(dt, this->WidthLeft, this->WidthRight, this->Height, this->Time);
		PowerUps->UpdatePowerUps(dt, *this->Cars[this->Car]);
		//PowerUp::UpdatePowerUps(dt, *PowerUps, *this->Cars[this->Car]);
		this->SpawnBullets(dt);
		this->UpdateBullets(dt);
		this->UpdateEnemys(dt);
		this->UpdateBooms(dt);
		if (ShakeTime > 0.0f)
		{
			ShakeTime -= dt;
			if (ShakeTime <= 0.0f)
				Effects->SetShake(GL_FALSE);
		}
		for (Bullet& bullet : this->Bullets)
		{
			if (!bullet.GetDestroyed() && bullet.GetAlly() && this->Cars[this->Car]->GetDamageBuff())
			{
				bullet.SetColor(glm::vec3(1.0f, 0.3f, 0.3f));
			}
			if (!bullet.GetDestroyed() && bullet.GetAlly() && this->Cars[this->Car]->GetDamageBuff())
			{
				bullet.SetColor(glm::vec3(1.0f, 0.3f, 0.3f));
			}
		}
		 if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
		 {
		   this->State = GAME_WIN;
		 }
		if (this->Cars[this->Car]->GetDestroyed())
		{
			this->State = GAME_LOSE;
		}
		if (this->Cars[this->Car]->GetDestroyed())
		{
			this->State = GAME_LOSE;
		}
	}
	if (this->State == GAME_PAUSE || this->State == GAME_LOSE)
	{
		Effects->SetShake(GL_FALSE);
	}
	if (this->State == GAME_OPTIONS)
	{
		if (this->MusicVolume <= 0) this->MusicVolume = 0;
		if (this->MusicVolume >= 1) this->MusicVolume = 1;
		if (this->SoundVolume <= 0) this->SoundVolume = 0;
		if (this->SoundVolume >= 1) this->SoundVolume = 1;
	}
}

void Game::ProcessInput(GLfloat dt)
{
	dt *= TimeSpeed;
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocityX = PLAYER_VELOCITY_X * dt;
		GLfloat velocityY = PLAYER_VELOCITY_Y * dt;

		if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
		{
			if (this->Cars[this->Car]->GetPositionX() >= this->WidthLeft)
			{
				this->Cars[this->Car]->SetRotateLeft(GL_TRUE);
				if (!this->Cars[this->Car]->GetKnockback())
					this->Cars[this->Car]->MoveLeft(velocityX);
				else
					this->Cars[this->Car]->UpdatePositionX(-1 * (velocityX / 1.5 - this->Cars[this->Car]->GetKnockbackVelocityX() * dt));
			}
		}
		else
		{
			this->Cars[this->Car]->SetRotateLeft(GL_FALSE);
		}
		if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
		{
			if (this->Cars[this->Car]->GetPositionX() <= (this->WidthRight - this->Cars[this->Car]->GetSizeX()))
			{
				this->Cars[this->Car]->SetRotateRight(GL_TRUE);
				if (!this->Cars[this->Car]->GetKnockback())
					this->Cars[this->Car]->MoveRight(velocityX);
				else
					this->Cars[this->Car]->UpdatePositionX(velocityX / 1.5 - this->Cars[this->Car]->GetKnockbackVelocityX() * dt);
			}
		}
		else
		{
			this->Cars[this->Car]->SetRotateRight(GL_FALSE);
		}
		if (this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP])
		{
			if (this->Cars[this->Car]->GetPositionY() >= 0)
			{
				if (!this->Cars[this->Car]->GetKnockback())
					this->Cars[this->Car]->MoveForward(velocityY);
				else
					this->Cars[this->Car]->UpdatePositionY(-1 * (velocityY / 1.5 - this->Cars[this->Car]->GetKnockbackVelocityY() * dt));
			}
		}
		if (this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN])
		{
			if (this->Cars[this->Car]->GetPositionY() <= (this->Height - this->Cars[this->Car]->GetSizeY()))
			{
				if (!this->Cars[this->Car]->GetKnockback())
					this->Cars[this->Car]->MoveBack(velocityY);
				else
					this->Cars[this->Car]->UpdatePositionY(velocityY / 1.5 - this->Cars[this->Car]->GetKnockbackVelocityY() * dt);
			}
		}
		if (this->Keys[GLFW_KEY_R] && !this->KeysProcessed[GLFW_KEY_R])
		{
			if (!this->Cars[this->Car]->GetOpen())
			{
				if (!this->Cars[this->Car]->GetStartOpen())
				{
					this->Cars[this->Car]->SetPeriodOpen(this->Time);
					this->Cars[this->Car]->SetStartOpen(GL_TRUE);
				}
				this->Cars[this->Car]->SetOpen(GL_TRUE);
				this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
			}
			else
			{
				this->Cars[this->Car]->SetOpen(GL_FALSE);
				this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
			}
		}
		if (this->Keys[GLFW_KEY_F] && !this->KeysProcessed[GLFW_KEY_F])
		{
			if (!this->SlowTime)
			{
				this->SlowTime = GL_TRUE;
				this->TimeSpeed = 0.5;
				this->KeysProcessed[GLFW_KEY_F] = GL_TRUE;
			}
			else
			{
				this->SlowTime = GL_FALSE;
				this->TimeSpeed = 1;
				this->KeysProcessed[GLFW_KEY_F] = GL_TRUE;
			}
		}
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_PAUSE;
			this->TimeSpeed = 0;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
		}
	}
	if (this->State == GAME_MENU)
	{
		if (this->MenuState == MENU_START)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_START;
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->MenuState = MENU_EXIT;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->MenuState = MENU_GARAGE;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->MenuState == MENU_GARAGE)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->MenuState = MENU_START;
				this->State = GAME_GARAGE;
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->MenuState = MENU_START;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->MenuState = MENU_OPTIONS;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->MenuState == MENU_OPTIONS)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->MenuState = MENU_START;
				this->State = GAME_OPTIONS;
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->MenuState = MENU_GARAGE;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->MenuState = MENU_EXIT;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->MenuState == MENU_EXIT)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->MenuState = MENU_OPTIONS;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->MenuState = MENU_START;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
	}
	if (this->State == GAME_START)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			ResetPlayer();
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_MENU;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
		}
		if (this->StartState == START_LEVEL)
		{
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->StartState = START_CAR;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->StartState = START_CAR;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT])
			{
				this->Level = (this->Level + 1) % 2;
				this->KeysProcessed[GLFW_KEY_A] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT])
			{
				this->Level = (this->Level - 1) % 2;
				this->KeysProcessed[GLFW_KEY_D] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;
			}
		}
		if (this->StartState == START_CAR)
		{
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->StartState = START_LEVEL;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->StartState = START_LEVEL;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT])
			{
				this->Car = (this->Car + 1) % 2;
				this->KeysProcessed[GLFW_KEY_A] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT])
			{
				this->Car = (this->Car - 1) % 2;
				this->KeysProcessed[GLFW_KEY_D] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;
			}
		}
	}
	if (this->State == GAME_PAUSE)
	{
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_ACTIVE;
			this->TimeSpeed = 1;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
		}
		if (this->PauseState == PAUSE_CONTINUE)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_ACTIVE;
				this->TimeSpeed = 1;
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->PauseState = PAUSE_MENU;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->PauseState = PAUSE_RESET;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->PauseState == PAUSE_RESET)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_ACTIVE;
				this->PauseState = PAUSE_CONTINUE;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->PauseState = PAUSE_CONTINUE;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->PauseState = PAUSE_MENU;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->PauseState == PAUSE_MENU)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_MENU;
				this->PauseState = PAUSE_CONTINUE;
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->PauseState = PAUSE_RESET;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->PauseState = PAUSE_CONTINUE;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
	}
	if (this->State == GAME_LOSE)
	{
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_MENU;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
		}
		if (this->LoseState == LOSE_TRY_AGAIN)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_ACTIVE;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->LoseState = LOSE_MENU;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->LoseState = LOSE_MENU;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->LoseState == LOSE_MENU)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_MENU;
				this->LoseState = LOSE_TRY_AGAIN;
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->LoseState = LOSE_TRY_AGAIN;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->LoseState = LOSE_TRY_AGAIN;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
	}
	if (this->State == GAME_OPTIONS)
	{
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_MENU;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
		}
		if (this->OptionsState == MUSIC_VOLUME)
		{
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->OptionsState = SOUND_VOLUME;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->OptionsState = SOUND_VOLUME;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT])
			{
				this->MusicVolume -= 0.02;
				this->KeysProcessed[GLFW_KEY_A] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT])
			{
				this->MusicVolume += 0.02;
				this->KeysProcessed[GLFW_KEY_D] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;
			}
		}
		if (this->OptionsState == SOUND_VOLUME)
		{
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->OptionsState = MUSIC_VOLUME;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->OptionsState = MUSIC_VOLUME;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT])
			{
				this->SoundVolume -= 0.02;
				this->KeysProcessed[GLFW_KEY_A] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT])
			{
				this->SoundVolume += 0.02;
				this->KeysProcessed[GLFW_KEY_D] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;

			}
		}
	}
	if (this->State == GAME_GARAGE)
	{
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_MENU;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_A] && !this->KeysProcessed[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT] && !this->KeysProcessed[GLFW_KEY_LEFT])
		{
			this->Car = (this->Car + 1) % 2;
			this->KeysProcessed[GLFW_KEY_A] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_D] && !this->KeysProcessed[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT] && !this->KeysProcessed[GLFW_KEY_RIGHT])
		{
			this->Car = (this->Car - 1) % 2;
			this->KeysProcessed[GLFW_KEY_D] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
		{
			this->State = GAME_UPGRADE;
			if (this->Car == 0)
			{
				this->YellowState = YELLOW_TIME_IN_INVINCIBLE;
			}
			else if (this->Car == 1)
			{
				this->RedState = RED_VAMPIRIC;
			}
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
		}		
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
		{
			this->State = GAME_UPGRADE;
			if (this->Car == 0)
			{
				this->YellowState = YELLOW_DAMAGE;
			}
			else if (this->Car == 1)
			{
				this->RedState = RED_DAMAGE;
			}
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
		}
	}
	if (this->State == GAME_UPGRADE)
	{
		if (this->Keys[GLFW_KEY_ESCAPE] && !this->KeysProcessed[GLFW_KEY_ESCAPE])
		{
			this->State = GAME_MENU;
			this->KeysProcessed[GLFW_KEY_ESCAPE] = GL_TRUE;
			this->YellowState = YELLOW_NOT;
			this->RedState = RED_NOT;
		}
		if (this->Car == 0)
		{
			if (this->YellowState == YELLOW_DAMAGE)
			{
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
				{
					this->State = GAME_GARAGE;
					this->YellowState = YELLOW_NOT;
					this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
				{
					this->YellowState = YELLOW_DURABILITY;
					this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
				{
					if (this->Money >= this->Cars[this->Car]->GetDamageCost())
					{
						this->Money -= this->Cars[this->Car]->GetDamageCost();
						this->Cars[this->Car]->UpgradeBulletDamage(5);
						this->Cars[this->Car]->UpdateDamageCost();
					}
					this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				}
			}
			if (this->YellowState == YELLOW_DURABILITY)
			{
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
				{
					this->YellowState = YELLOW_DAMAGE;
					this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
				{
					this->YellowState = YELLOW_TIME_IN_INVINCIBLE;
					this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
				{
					if (this->Money >= this->Cars[this->Car]->GetDurabilityCost())
					{
						this->Money -= this->Cars[this->Car]->GetDurabilityCost();
						this->Cars[this->Car]->UpgradeHitPoints(30);
						this->Cars[this->Car]->UpdateDurabilityCost();
					}
					this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				}
			}
			if (this->YellowState == YELLOW_TIME_IN_INVINCIBLE)
			{
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
				{
					this->YellowState = YELLOW_DURABILITY;
					this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
				{
					this->State = GAME_GARAGE;
					this->YellowState = YELLOW_NOT;
					this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
				{
					if (this->Money >= this->Cars[this->Car]->GetTimeInInvincibleCost())
					{
						this->Money -= this->Cars[this->Car]->GetTimeInInvincibleCost();
						this->Cars[this->Car]->UpgradeTimeInInvincible(0.15);
						this->Cars[this->Car]->UpdateTimeInInvincibleCost();
					}
					this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				}
			}
		}
		if (this->Car == 1)
		{
			if (this->RedState == RED_DAMAGE)
			{
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
				{
					this->State = GAME_GARAGE;
					this->RedState = RED_NOT;
					this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
				{
					this->RedState = RED_DURABILITY;
					this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
				{
					if (this->Money >= this->Cars[this->Car]->GetDamageCost())
					{
						this->Money -= this->Cars[this->Car]->GetDamageCost();
						this->Cars[this->Car]->UpgradeBulletDamage(2);
						this->Cars[this->Car]->UpdateDamageCost();
					}
					this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				}
			}
			if (this->RedState == RED_DURABILITY)
			{
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
				{
					this->RedState = RED_DAMAGE;
					this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
				{
					this->RedState = RED_VAMPIRIC;
					this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
				{
					if (this->Money >= this->Cars[this->Car]->GetDurabilityCost())
					{
						this->Money -= this->Cars[this->Car]->GetDurabilityCost();
						this->Cars[this->Car]->UpgradeHitPoints(30);
						this->Cars[this->Car]->UpdateDurabilityCost();
					}
					this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				}
			}
			if (this->RedState == RED_VAMPIRIC)
			{
				if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
				{
					this->RedState = RED_DURABILITY;
					this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
				{
					this->State = GAME_GARAGE;
					this->RedState = RED_NOT;
					this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
					this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
				}
				if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
				{
					if (this->Money >= this->Cars[this->Car]->GetVampiricCost())
					{
						this->Money -= this->Cars[this->Car]->GetVampiricCost();
						this->Cars[this->Car]->UpgradeVampiric(0.005);
						this->Cars[this->Car]->UpdateVampiricCost();
					}
					this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
				}
			}
		}
	}
	if (this->State == GAME_WIN)
	{
		if (this->WinState == WIN_NEXT_LEVEL)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->Level = (this->Level + 1) % 2;
				this->State = GAME_ACTIVE;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->WinState = WIN_MENU;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->WinState = WIN_RESET;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->WinState == WIN_RESET)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->WinState = WIN_NEXT_LEVEL;
				this->State = GAME_ACTIVE;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->WinState = WIN_NEXT_LEVEL;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->WinState = WIN_MENU;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
		if (this->WinState == WIN_MENU)
		{
			if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
			{
				this->State = GAME_MENU;
				this->WinState = WIN_NEXT_LEVEL;
				this->TimeSpeed = 1;
				this->ResetLevel();
				this->ResetPlayer();
				this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP] && !this->KeysProcessed[GLFW_KEY_UP])
			{
				this->WinState = WIN_RESET;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
			if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN] && !this->KeysProcessed[GLFW_KEY_DOWN])
			{
				this->WinState = WIN_NEXT_LEVEL;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
				this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
	}
	// if (this->State == GAME_WIN)
	// {
	//   if (this->Keys[GLFW_KEY_ENTER])
	//   {
	//     this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
	//     Effects->Chaos = GL_FALSE;
	//     this->State = GAME_MENU;
	//   }
	// }
}

void Game::Render(GLfloat dt)
{
	dt *= TimeSpeed;
	if (this->State == GAME_ACTIVE || this->State == GAME_WIN || this->State == GAME_PAUSE || this->State == GAME_LOSE)
	{
		Effects->BeginRender();
		Texture2D myTexture;
		myTexture = ResourceManager::GetTexture("background");
		if (this->State == GAME_ACTIVE)
			this->Velocity += dt * 2000;
		GLfloat upStep = -1 * this->Height;
		if (this->Velocity > this->Height)
			this->Velocity = 0;
		Renderer->DrawSprite(myTexture,
			glm::vec2(0, this->Velocity - this->Height), glm::vec2(this->Width, this->Height), 0.0f);
		Renderer->DrawSprite(myTexture,
			glm::vec2(0, 0 + this->Velocity), glm::vec2(this->Width, this->Height), 0.0f);

		this->Levels[this->Level].Draw(*Renderer, this->Width, this->Height);
		if (!this->Cars[this->Car]->GetInvincible())
		{
			this->Cars[this->Car]->DrawWheel_1(*Renderer);
			this->Cars[this->Car]->DrawWheel_2(*Renderer);
			this->Cars[this->Car]->Draw(*Renderer);
		}
		if (this->Time / this->PeriodInvincible >= 1)
		{
			this->PeriodInvincible += 2 * dt;
			PlayerVisible = GL_TRUE;
		}
		else
			PlayerVisible = GL_FALSE;
		if (this->Cars[this->Car]->GetInvincible() && PlayerVisible)
		{
			this->Cars[this->Car]->DrawWheel_1(*Renderer);
			this->Cars[this->Car]->DrawWheel_2(*Renderer);
			this->Cars[this->Car]->Draw(*Renderer);
		}
		for (PowerUp* powerUp : PowerUps->GetPowerUps())
			if (!powerUp->GetDestroyed())
				powerUp->Draw(*Renderer);
		for (Bullet& bullet : this->Bullets)
			if (!bullet.GetDestroyed())
				bullet.Draw(*Renderer);
		for (Boom& boom : this->Booms)
			if (!boom.GetDestroyed())
				boom.Draw(*Renderer);
		// Particles->Draw();
		Effects->EndRender();
		Effects->Render(glfwGetTime());
		this->Levels[this->Level].DrawHitBar(*Renderer, this->Width, this->Height);
		this->Cars[this->Car]->DrawHitBar(*Renderer, this->Width, this->Height);
		std::stringstream ss; ss << this->Money;
		Text->RenderText("Money:" + ss.str(), 35.0f, 40.0f, 1.5f);
	}
   if (this->State == GAME_MENU)
   {
	   if (this->MenuState == MENU_START)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(80, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->MenuState == MENU_GARAGE)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(120, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->MenuState == MENU_OPTIONS)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 60.0f), glm::vec2(120, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->MenuState == MENU_EXIT)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 90.0f), glm::vec2(80, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   Text->RenderText("Start", Width / 2, Height / 2, 1.0f);
	   Text->RenderText("Garage", Width / 2, Height / 2 + 30.0f, 1.0f);
	   Text->RenderText("Options", Width / 2, Height / 2 + 60.0f, 1.0f);
	   Text->RenderText("Exit", Width / 2, Height / 2 + 90.0f, 1.0f);

   }
   if (this->State == GAME_GARAGE || this->State == GAME_UPGRADE)
   {

	   std::stringstream ss; ss << this->Money;
	   std::stringstream sd; sd << this->Cars[this->Car]->GetMaxBulletDamage();
	   std::stringstream sh; sh << this->Cars[this->Car]->GetMaxHitPoints();
	   std::stringstream st; st << this->Cars[this->Car]->GetMaxTimeInInvincible();
	   std::stringstream sv; sv << this->Cars[this->Car]->GetVampiric() * 100;
	   std::stringstream sdc; sdc << this->Cars[this->Car]->GetDamageCost();
	   std::stringstream sduc; sduc << this->Cars[this->Car]->GetDurabilityCost();
	   std::stringstream stc; stc << this->Cars[this->Car]->GetTimeInInvincibleCost();
	   std::stringstream svc; svc << this->Cars[this->Car]->GetVampiricCost();
	   Text->RenderText("Money:" + ss.str(), 65.0f, 40.0f, 1.5f);
	   if (this->State == GAME_GARAGE)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(80, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->State == GAME_UPGRADE)
	   {
		   if (this->RedState == RED_DAMAGE || this->YellowState == YELLOW_DAMAGE)
		   {
			   Texture2D myTexture;
			   myTexture = ResourceManager::GetTexture("quad");
			   Renderer->DrawSprite(myTexture,
				   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(180, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
		   }
		   if (this->RedState == RED_DURABILITY || this->YellowState == YELLOW_DURABILITY)
		   {
			   Texture2D myTexture;
			   myTexture = ResourceManager::GetTexture("quad");
			   Renderer->DrawSprite(myTexture,
				   glm::vec2(Width / 2, Height / 2 + 60.0f), glm::vec2(160, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
		   }
		   if (this->RedState == RED_VAMPIRIC)
		   {
			   Texture2D myTexture;
			   myTexture = ResourceManager::GetTexture("quad");
			   Renderer->DrawSprite(myTexture,
				   glm::vec2(Width / 2, Height / 2 + 90.0f), glm::vec2(120, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
		   }
		   if (this->YellowState == YELLOW_TIME_IN_INVINCIBLE)
		   {
			   Texture2D myTexture;
			   myTexture = ResourceManager::GetTexture("quad");
			   Renderer->DrawSprite(myTexture,
				   glm::vec2(Width / 2, Height / 2 + 90.0f), glm::vec2(300, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
		   }
	   }
	   if (this->Car == 0)
	   {
		   Text->RenderText("YellowDevil ", Width / 2 + 100.0f, Height / 2, 1.0f);
		   Text->RenderText("BulletDamage: ", Width / 2, Height / 2 + 30.0f, 1.0f);
		   Text->RenderText(sd.str() + "d     " + sdc.str() + "G +5d", Width / 2 + 300.0f, Height / 2 + 30.0f, 1.0f);

		   Text->RenderText("Durability: ", Width / 2, Height / 2 + 60.0f, 1.0f); 
		   Text->RenderText(sh.str() + "hp     " + sduc.str() + "G +30hp", Width / 2 + 300.0f, Height / 2 + 60.0f, 1.0f);

		   Text->RenderText("Time in invincible: ", Width / 2, Height / 2 + 90.0f, 1.0f);
		   Text->RenderText(st.str() + "s     " + stc.str() + "G +0.1s", Width / 2 + 300.0f, Height / 2 + 90.0f, 1.0f);
	   }
	   if (this->Car == 1)
	   {
		   Text->RenderText("RedSnake ", Width / 2 + 100.0f, Height / 2, 1.0f);
		   Text->RenderText("BulletDamage: ", Width / 2, Height / 2 + 30.0f, 1.0f); 
		   Text->RenderText(sd.str() + "d     " + sdc.str() + "G +2d", Width / 2 + 300.0f, Height / 2 + 30.0f, 1.0f);

		   Text->RenderText("Durability: ", Width / 2, Height / 2 + 60.0f, 1.0f); 
		   Text->RenderText(sh.str() + "hp     " + sduc.str() + "G +30hp", Width / 2 + 300.0f, Height / 2 + 60.0f, 1.0f);

		   Text->RenderText("Vampiric: ", Width / 2, Height / 2 + 90.0f, 1.0f);
		   Text->RenderText(sv.str() + "%     " + svc.str() + "G +1%", Width / 2 + 300.0f, Height / 2 + 90.0f, 1.0f);
	   }
	   Text->RenderText("Car  : ", Width / 2, Height / 2, 1.0f);
   }
   if (this->State == GAME_PAUSE)
   {
	   Texture2D myTexture;
	   myTexture = ResourceManager::GetTexture("quad");
	   Renderer->DrawSprite(myTexture,
		   glm::vec2(Width / 2 - 100, Height / 2 - 200), glm::vec2(300, 400), 0.0f, glm::vec3(0.1f, 0.1f, 0.3f));
	   if (this->PauseState == PAUSE_CONTINUE)
	   {
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(120, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->PauseState == PAUSE_RESET)
	   {
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(120, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->PauseState == PAUSE_MENU)
	   {
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 60.0f), glm::vec2(60, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   Text->RenderText("Continue", Width / 2, Height / 2, 1.0f);
	   Text->RenderText("Reset", Width / 2, Height / 2 + 30.0f, 1.0f);
	   Text->RenderText("Menu", Width / 2, Height / 2 + 60.0f, 1.0f);

   }
   if (this->State == GAME_LOSE)
   {
	   Texture2D myTexture;
	   myTexture = ResourceManager::GetTexture("quad");
	   Renderer->DrawSprite(myTexture,
		   glm::vec2(Width / 2 - 100, Height / 2 - 200), glm::vec2(300, 400), 0.0f, glm::vec3(0.1f, 0.1f, 0.3f));
	   if (this->LoseState == LOSE_TRY_AGAIN)
	   {
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(120, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->LoseState == LOSE_MENU)
	   {
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(60, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   Text->RenderText("Try again", Width / 2, Height / 2, 1.0f);
	   Text->RenderText("Menu", Width / 2, Height / 2 + 30.0f, 1.0f);

   }
   if (this->State == GAME_OPTIONS)
   {
	   if (this->OptionsState == MUSIC_VOLUME)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(80, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->OptionsState == SOUND_VOLUME)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(80, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   std::stringstream ss; ss << (int)(this->MusicVolume * 100);
	   Text->RenderText("Music:" + ss.str(), Width / 2, Height / 2, 1.0f);
	   std::stringstream sss; sss << (int)(this->SoundVolume * 100);
	   Text->RenderText("Sound:" + sss.str(), Width / 2, Height / 2 + 30.0f, 1.0f);
   }
   if (this->State == GAME_WIN)
   {
	   Texture2D myTexture;
	   myTexture = ResourceManager::GetTexture("quad");
	   Renderer->DrawSprite(myTexture,
		   glm::vec2(Width / 2 - 100, Height / 2 - 200), glm::vec2(300, 400), 0.0f, glm::vec3(0.1f, 0.1f, 0.3f));
	   if (this->WinState == WIN_NEXT_LEVEL)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(150, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->WinState == WIN_RESET)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(150, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->WinState == WIN_MENU)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 60.0f), glm::vec2(80, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   Text->RenderText("Next level", Width / 2, Height / 2, 1.0f);
	   Text->RenderText("Reset", Width / 2, Height / 2 + 30.0f, 1.0f);
	   Text->RenderText("Menu", Width / 2, Height / 2 + 60.0f, 1.0f);
   }
   if (this->State == GAME_START)
   {
	   if (this->StartState == START_LEVEL)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2), glm::vec2(270, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->StartState == START_CAR)
	   {
		   Texture2D myTexture;
		   myTexture = ResourceManager::GetTexture("quad");
		   Renderer->DrawSprite(myTexture,
			   glm::vec2(Width / 2, Height / 2 + 30.0f), glm::vec2(270, 20), 0.0f, glm::vec3(0.0f, 0.0f, 0.5f));
	   }
	   if (this->Level == 0)
	   {
		   Text->RenderText("Level 1 ", Width / 2 + 100.0f, Height / 2, 1.0f);
	   }
	   if (this->Level == 1)
	   {
		   Text->RenderText("Level 2 ", Width / 2 + 100.0f, Height / 2, 1.0f);
	   }
	   if (this->Car == 0)
	   {
		   Text->RenderText("YellowDevil ", Width / 2 + 100.0f, Height / 2 + 30.0f, 1.0f);
	   }
	   if (this->Car == 1)
	   {
		   Text->RenderText("RedSnake ", Width / 2 + 100.0f, Height / 2 + 30.0f, 1.0f);
	   }
	   Text->RenderText("Level: ", Width / 2, Height / 2, 1.0f);
	   Text->RenderText("Car  : ", Width / 2, Height / 2 + 30.0f, 1.0f);
   }
}

void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("Levels/level1.lvl");
	else
		if (this->Level == 1)this->Levels[1].Load("Levels/level2.lvl");
	this->Time = 0;
	this->PeriodInvincible = 1;
	this->Velocity = 0;
	this->Booms.clear();
	PowerUps->PowerUpsClear();
	//else if (this->Level == 1)
	//	this->Levels[1].Load("level2.lvl");
	//else if (this->Level == 2)
	//	this->Levels[2].Load("level3.lvl");
	//else if (this->Level == 3)
	//	this->Levels[3].Load("level4.lvl");
	//this->Lives = 3;
	this->Bullets.clear();
	snd->stop();

}

void Game::ResetPlayer()
{
	// YellowDevilCar->Size = PLAYER_SIZE;
	this->Cars[this->Car]->SetPosition(glm::vec2(this->Width / 2 + PLAYER_SIZE.x, this->Height - PLAYER_SIZE.y));
	this->Cars[this->Car]->ResetCar();
	this->Cars[this->Car]->ResetPlayer();
	this->Cars[this->Car]->SetDestroyed(GL_FALSE);

	Effects->SetChaos(GL_FALSE); Effects->SetConfuse(GL_FALSE); Effects->SetShake(GL_FALSE);
	this->Cars[this->Car]->SetColor(glm::vec3(1.0f));
}

GLboolean CheckCollision(GameObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

void Game::UpdateEnemys(GLfloat dt)
{
	for (Enemy* enemy : this->Levels[this->Level].GetEnemys())
	{
		enemy->UpdateKnockback(dt, this->WidthLeft, this->WidthRight);
		enemy->UpdateEnemy(dt, this->WidthLeft, this->WidthRight, this->Width, this->Height, this->Time);
	}
}

void Game::UpdateBullets(GLfloat dt)
{
	for (Bullet& bullet : this->Bullets)
	{
		bullet.UpdatePosition(bullet.GetVelocity() * dt);
	}
	this->Bullets.erase(std::remove_if(this->Bullets.begin(), this->Bullets.end(), [](const Bullet& bullet)
		{ return bullet.GetDestroyed(); }), this->Bullets.end());
}

void Game::UpdateBooms(GLfloat dt)
{
	for (Boom& boom : this->Booms)
	{
		boom.UpdateBoom(this->Time, dt);
	}
	this->Booms.erase(std::remove_if(this->Booms.begin(), this->Booms.end(), [](const Boom& boom)
		{ return boom.GetDestroyed(); }), this->Booms.end());
}

void Game::SpawnBullets(GLfloat dt)
{
	this->Cars[this->Car]->SpawnBullets(this->Time, dt, &Bullets);

	for (Enemy* enemy : this->Levels[this->Level].GetEnemys())
	{
		enemy->SpawnBullets(this->Time, dt, this->Cars[this->Car]->GetPositionX(), this->Cars[this->Car]->GetPositionY(), &Bullets);
	}
}

void Game::SpawnBooms(GameObject& enemy, GLfloat dt)
{
	glm::vec2 boomPos(enemy.GetPositionX() - enemy.GetSizeX() * 0.1, enemy.GetPositionY() + enemy.GetSizeY() * 0.3);
	this->Booms.push_back
	(
		Boom(boomPos, glm::vec2(enemy.GetSizeY() * 0.8, enemy.GetSizeY() * 0.8), ResourceManager::GetTexture("boom1"), glm::vec2(0.0f, 0.0f))
	);
}

void Game::DoCollisions(GLfloat dt)
{
	for (Enemy* enemy : this->Levels[this->Level].GetEnemys())
	{
		if (!enemy->GetDestroyed())
		{
			for (Bullet& bullet : this->Bullets)
				// Collision collision = CheckCollision(bullet, enemy);
				if (CheckCollision(bullet, *enemy) && bullet.GetAlly())
				{
					enemy->UpdateHitPoints(this->Cars[this->Car]->GetBulletDamage());
					enemy->SetDamage(GL_TRUE);
					if (this->Cars[this->Car]->GetHitPoints() < this->Cars[this->Car]->GetMaxHitPoints())
					{
						this->Cars[this->Car]->UpdateVampiric(this->Cars[this->Car]->GetBulletDamage() * this->Cars[this->Car]->GetVampiric());
						if (this->Cars[this->Car]->GetHitPoints() > this->Cars[this->Car]->GetMaxHitPoints())
							this->Cars[this->Car]->SetHitPoints(this->Cars[this->Car]->GetMaxHitPoints());
					}

					if (enemy->GetHitPoints() <= 0)
					{
						enemy->SetDestroyed(GL_TRUE);
						this->SpawnBooms(*enemy, dt);
						//PowerUp::SpawnPowerUps(*enemy, &PowerUps);
						PowerUps->SpawnPowerUps(*enemy);
						this->Money += enemy->GetMoney();
						// ShakeTime = 0.05f;
						// Effects->Shake = GL_TRUE;
					}
					bullet.SetDestroyed(GL_TRUE);
				}
		}
		for (Bullet& bullet : this->Bullets)
			// Collision collision = CheckCollision(bullet, enemy);
			if (CheckCollision(bullet, *this->Cars[this->Car]) && bullet.GetEnemy() && !this->Cars[this->Car]->GetInvincible())
			{
				this->Cars[this->Car]->UpdateHitPoints(enemy->GetBulletDamage());
				ShakeTime = 0.1f;
				Effects->SetShake(GL_TRUE);
				if (this->Cars[this->Car]->GetHitPoints() <= 0)
				{
					this->Cars[this->Car]->SetDestroyed(GL_TRUE);
					// ShakeTime = 0.05f;
					// Effects->Shake = GL_TRUE;
				}
				bullet.SetDestroyed(GL_TRUE);
				this->Cars[this->Car]->SetInvincible(GL_TRUE);
				this->Cars[this->Car]->SetTimeInvincible(this->Time + this->Cars[this->Car]->GetMaxTimeInInvincible());
			}
		if (!enemy->GetDestroyed())
			if (CheckCollision(*enemy, *this->Cars[this->Car]) && !this->Cars[this->Car]->GetInvincible())
			{
				this->Cars[this->Car]->UpdateHitPoints(enemy->GetBulletDamage() * 2);
				ShakeTime = 0.11f;
				Effects->SetShake(GL_TRUE);
				enemy->UpdateHitPoints(this->Cars[this->Car]->GetBulletDamage() * 2);
				enemy->SetDamage(GL_TRUE);
				// glm::vec2 velocity(0.0f, 0.0f);
				float x, y;
				x = enemy->GetPositionX() - this->Cars[this->Car]->GetPositionX();
				y = enemy->GetPositionX() - this->Cars[this->Car]->GetPositionY();
				KnockbackVelocity.x = sqrt((enemy->GetBulletVelocity() * enemy->GetBulletVelocity() * x * x * 4) / (x * x + y * y));
				KnockbackVelocity.y = abs(y / x * KnockbackVelocity.x);
				if (enemy->GetPositionX() > this->Cars[this->Car]->GetPositionX())
					KnockbackVelocity.x = -KnockbackVelocity.x;
				if (enemy->GetPositionY() > this->Cars[this->Car]->GetPositionY())
					KnockbackVelocity.y = -KnockbackVelocity.y;
				enemy->SetKnockback(GL_TRUE);
				enemy->SetKnockbackVelocity(KnockbackVelocity);
				this->Cars[this->Car]->SetKnockback(GL_TRUE);
				this->Cars[this->Car]->SetKnockbackVelocity(KnockbackVelocity);

				if (enemy->GetHitPoints() <= 0)
				{
					enemy->SetDestroyed(GL_TRUE);
					enemy->SetKnockback(GL_FALSE);
					this->SpawnBooms(*enemy, dt);
					this->Money += enemy->GetMoney();
					//PowerUp::SpawnPowerUps(*enemy, &PowerUps);
					PowerUps->SpawnPowerUps(*enemy);
				}
				if (this->Cars[this->Car]->GetHitPoints() <= 0)
				{
					this->Cars[this->Car]->SetDestroyed(GL_TRUE);
				}
				this->Cars[this->Car]->SetInvincible(GL_TRUE);
				this->Cars[this->Car]->SetTimeInvincible(this->Time + 1);
			}
	}

	for (PowerUp* powerUp : PowerUps->GetPowerUps())
	{
		if (!powerUp->GetDestroyed())
		{
			if (powerUp->GetPositionY() >= this->Height)
				powerUp->SetDestroyed(GL_TRUE);
			if (CheckCollision(*this->Cars[this->Car], *powerUp))
			{
				//PowerUp::ActivatePowerUp(powerUp, *this->Cars[this->Car]);
				PowerUps->ActivatePowerUp(*powerUp, *this->Cars[this->Car]);
				powerUp->SetDestroyed(GL_TRUE);
				powerUp->SetActivated(GL_TRUE);
				//SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
			}
		}
	}

	for (Bullet& bullet : this->Bullets)
	{
		if (!bullet.GetDestroyed())
		{
			if (bullet.GetPositionY() >= this->Height || bullet.GetPositionY() <= 0)
				bullet.SetDestroyed(GL_TRUE);
		}
	}

	// Collision result = CheckCollision(*Ball, *Player);
	// if (!Ball->Stuck && std::get<0>(result))
	// {
	//   GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
	//   GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
	//   GLfloat percentage = distance / (Player->Size.x / 2);
	//
	//   GLfloat strength = 2.0f;
	//   glm::vec2 oldVelocity = Ball->Velocity;
	//   Ball->Velocity.x = ININTIAL_BALL_VELOCITY.x * percentage * strength;
	//   Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
	//   Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
	//   Ball->Stuck = Ball->Sticky;
	//
	//   SoundEngine->play2D("audio/bleep.wav", GL_FALSE);
	// }
}

GLboolean CheckCollision(GameObject& one, GameObject& two)
{
	bool collisionX = one.GetPositionX() + one.GetSizeX() - 10 >= two.GetPositionX() &&
		two.GetPositionX() + two.GetSizeX() - 10 >= one.GetPositionX();

	bool collisionY = one.GetPositionY() + one.GetSizeY() - 10 >= two.GetPositionY() &&
		two.GetPositionY() + two.GetSizeY() - 10 >= one.GetPositionY();
	return collisionX && collisionY;
}
// GLboolean CheckCollision(GameObject &one, GameObject &two)
// {
//   bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
//     two.Position.x + two.Size.x >= one.Position.x;
//
//   bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
//     two.Position.y + two.Size.y >= one.Position.y;
//   return collisionX && collisionY;
// }

// Collision CheckCollision(BallObject &one, GameObject &two)
// {
//   glm::vec2 center(one.Position + one.Radius);
//   glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
//   glm::vec2 aabb_center
//   (
//     two.Position.x + aabb_half_extents.x,
//     two.Position.y + aabb_half_extents.y
//   );
//   glm::vec2 difference = center - aabb_center;
//   glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
//   glm::vec2 closest = aabb_center + clamped;
//   difference = closest - center;
//   if (glm::length(difference) < one.Radius)
//     return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
//   else
//     return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
// }

//Direction VectorDirection(glm::vec2 target)
//{
//	glm::vec2 compass[] =
//	{
//	  glm::vec2(0.0f, 1.0f),
//	  glm::vec2(1.0f, 0.0f),
//	  glm::vec2(0.0f, -1.0f),
//	  glm::vec2(-1.0f, 0.0f)
//	};
//	GLfloat max = 0.0f;
//	GLuint best_match = -1;
//	for (GLuint i = 0; i < 4; i++)
//	{
//		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
//		if (dot_product > max)
//		{
//			max = dot_product;
//			best_match = i;
//		}
//	}
//	return (Direction)best_match;
//}


