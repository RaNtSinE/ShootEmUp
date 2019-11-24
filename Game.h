#pragma once
#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "PowerUp.h"
#include "BallObject.h"
#include "YellowDevil.h"
#include "Bullet.h"
#include "Boom.h"

enum GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

enum Direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


const glm::vec2 PLAYER_SIZE(50, 100);
const GLfloat PLAYER_VELOCITY_X(500.0f);
const GLfloat PLAYER_VELOCITY_Y(300.0f);
const glm::vec2 ININTIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BULLET_SIZE = 15.0f;

class Game
{
public:
  GameState State;
  GLboolean Keys[1024];
  GLuint    Width, Height;
  GLfloat   WidthLeft, WidthRight, HeightUp, HeightDown;
  std::vector<GameLevel> Levels;
  std::vector<PowerUp> PowerUps;
  std::vector<Bullet> Bullets;
  std::vector<Boom> Booms;
  GLuint Level;
  GLfloat Velocity;
  GLfloat Time;
  GLfloat PeriodBullet;
  GLfloat PeriodBulletEnemy;
  GLfloat PeriodInvincible;
  GLfloat PeriodOpen;
  GLfloat PeriodBoom;

  Game(GLuint width, GLuint height);
  ~Game();

  void Init();

  void ProcessInput(GLfloat dt);
  void Update(GLfloat dt);
  void Render (GLfloat dt);
  // GLboolean CheckCollision(GameObject &one, GameObject &two);
  void DoCollisions(GLfloat dt);
  void ResetLevel();
  void ResetPlayer();
  void SpawnPowerUps(GameObject &block);
  void UpdatePowerUps(GLfloat dt);
  void SpawnBullets(GLfloat dt);
  void UpdateBullets(GLfloat dt);
  void UpdateEnemys(GLfloat dt);
  void SpawnBooms(GameObject &enemy, GLfloat dt);
  void UpdateBooms(GLfloat dt);

  GLuint Lives;
  GLboolean KeysProcessed[1024];
};
