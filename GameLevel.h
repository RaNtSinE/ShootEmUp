#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Resource_manager.h"
#include "Enemy.h"

class GameLevel
{
public:
  std::vector<GLfloat> TimeSpawn;
  std::vector<GLint> Line;
  std::vector<GLint> TypeEnemy;
  std::vector<GLint> NumberEnemy;
  GLint NumberWaves;
  std::vector<Enemy> Enemys;
  Enemy Boss;
  GameLevel() { }

  void Load(const GLchar *file);

  void Draw(SpriteRenderer &renderer);

  GLboolean IsCompleted();
  void SpawnEnemys();
private:
  void init(std::vector<std::vector<GLuint>> tileData);
};
