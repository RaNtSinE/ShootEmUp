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
  GameLevel() { }
  
  GLint GetNumberWaves() { return this->NumberWaves; }
  GLfloat GetTimeSpawn(int i) { return this->TimeSpawn[i]; }
  const std::vector<Enemy*> &GetEnemys() const {return this->Enemys; }

  void Load(const GLchar *file);
  void Draw(SpriteRenderer &renderer);
  GLboolean IsCompleted();
  void SpawnEnemys(GLint wave, GLint line);

private:
  void init(std::vector<std::vector<GLuint>> tileData);
  GLint NumberWaves;
  std::vector<GLfloat> TimeSpawn;
  std::vector<GLint> Line;
  std::vector<GLint> TypeEnemy;
  std::vector<GLint> NumberEnemy;
  std::vector<Enemy*> Enemys;
  Enemy Boss;
};
