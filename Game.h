#pragma once
#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "PowerUp.h"
#include "Player.h"
#include "YellowDevil.h"
#include "Bullet.h"
#include "Boom.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_PAUSE,
	GAME_LOSE,
	GAME_OPTIONS,
	GAME_START,
	GAME_GARAGE,
	GAME_UPGRADE
};

enum MenuState
{
	MENU_START,
	MENU_GARAGE,
	MENU_OPTIONS,
	MENU_EXIT
};

enum OptionsMenuState
{
	MUSIC_VOLUME,
	SOUND_VOLUME
};

enum StartMenuState
{
	START_LEVEL,
	START_CAR
};

enum PauseMenuState
{
	PAUSE_CONTINUE,
	PAUSE_RESET,
	PAUSE_MENU
};

enum LoseMenuState
{
	LOSE_TRY_AGAIN,
	LOSE_MENU
};

enum WinMenuState
{
	WIN_NEXT_LEVEL,
	WIN_RESET,
	WIN_MENU
};

enum RedUpgradeState
{
	RED_NOT,
	RED_DAMAGE,
	RED_DURABILITY,
	RED_VAMPIRIC
};

enum YellowUpgradeState
{
	YELLOW_NOT,
	YELLOW_DAMAGE,
	YELLOW_DURABILITY,
	YELLOW_TIME_IN_INVINCIBLE
};

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


const glm::vec2 PLAYER_SIZE(100, 200);
const GLfloat PLAYER_VELOCITY_X(1000.0f);
const GLfloat PLAYER_VELOCITY_Y(600.0f);
const glm::vec2 ININTIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BULLET_SIZE = 30.0f;

class Game
{
public:
	GameState State;
	MenuState MenuState;
	PauseMenuState PauseState;
	LoseMenuState LoseState;
	OptionsMenuState OptionsState;
	WinMenuState WinState;
	StartMenuState StartState;
	RedUpgradeState RedState;
	YellowUpgradeState YellowState;
	GLboolean Keys[1024];
	Game(GLuint width, GLuint height);
	~Game();

	void Init();

	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render(GLfloat dt);
	void DoCollisions(GLfloat dt);
	void ResetLevel();
	void ResetPlayer();
	void SpawnBullets(GLfloat dt);
	void UpdateBullets(GLfloat dt);
	void UpdateEnemys(GLfloat dt);
	void SpawnBooms(GameObject& enemy, GLfloat dt);
	void UpdateBooms(GLfloat dt);
	const std::vector<Bullet>& GetBullets() const { return this->Bullets; }

	GLuint Lives;
	GLboolean KeysProcessed[1024];
private:
	GLuint Level;
	GLuint Car;
	GLfloat Velocity;
	GLfloat Time;
	GLfloat PeriodBullet;
	GLfloat PeriodBulletEnemy;
	GLfloat PeriodInvincible;
	GLfloat PeriodOpen;
	GLfloat PeriodBoom;
	GLfloat TimeSpeed;
	GLfloat MusicVolume;
	GLfloat SoundVolume;
	GLboolean SlowTime;
	GLboolean Pause;
	GLuint    Width, Height;
	GLuint    Money;
	GLfloat   WidthLeft, WidthRight, HeightUp, HeightDown;
	std::vector<Player*> Cars;
	std::vector<GameLevel> Levels;

	std::vector<Bullet> Bullets;
	std::vector<Boom> Booms;
};
