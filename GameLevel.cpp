#include "GameLevel.h"
#include "EnemyA.h"
#include "EnemyB.h"
#include "EnemyC.h"
#include "EnemyD.h"
#include "EnemyE.h"
#include "EnemyF.h"
#include "EnemyG.h"
#include "EnemyH.h"
#include "EnemyI.h"
#include "EnemyJ.h"
#include "EnemyK.h"
#include "EnemyL.h"
#include <fstream>
#include <sstream>
#include <iostream>

void GameLevel::Load(const GLchar *file)
{
  this->Enemys.clear();
  this->NumberWaves = 0;
  GLuint tileCode;
  GameLevel level;
  std::string line;
  std::ifstream fstream(file);
  std::vector<std::vector<GLuint>> tileData;
  if (fstream)
  {
    while (std::getline(fstream, line))
    {
      std::istringstream sstream(line);
      std::vector<GLuint> row;
      while (sstream >> tileCode)
        row.push_back(tileCode);
      tileData.push_back(row);
    }
    if (tileData.size() > 0)
      this->init(tileData);
  }

}

void GameLevel::Draw(SpriteRenderer &renderer)
{
  for (Enemy *tile : this->Enemys)
    if (!tile->GetDestroyed())
      tile->Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
  if (!this->Boss.GetDestroyed())
    return GL_FALSE;
  return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData)
{
  GLuint height = tileData.size();
  GLuint width = tileData[0].size();
  for (GLuint y = 0; y < height; ++y)
  {
    for (GLuint x = 0; x < width; ++x)
    {
      if(x == 0)
        this->TimeSpawn.push_back(tileData[y][x]);
      if(x == 1)
        this->Line.push_back(tileData[y][x]);
      if(x == 2)
        this->TypeEnemy.push_back(tileData[y][x]);
      if(x == 3)
        this->NumberEnemy.push_back(tileData[y][x]);
    }
    this->NumberWaves = this->NumberWaves + 1;
  }
}

void GameLevel::SpawnEnemys(GLint wave, GLint line)
{
  for (GLint i = 0; i < this->NumberEnemy[wave]; ++i)
  {
    GLfloat posy = -100 -150*i;
    glm::vec2 pos(210 + 130 * this->Line[wave], posy);
    glm::vec2 size(50, 100);
    if (this->TypeEnemy[wave] == 1)
    {
        EnemyA* obj= new EnemyA(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 200, this->TypeEnemy[wave]);
        this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 2)
    {
      EnemyB* obj= new EnemyB(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 150, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 3)
    {
      EnemyC* obj= new EnemyC(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 150, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 4)
    {
      EnemyD* obj= new EnemyD(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 100, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 5)
    {
      EnemyE* obj= new EnemyE(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 200, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 6)
    {
      EnemyF* obj= new EnemyF(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 150, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 7)
    {
      EnemyG* obj= new EnemyG(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 150, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 8)
    {
      EnemyH* obj= new EnemyH(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 100, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 9)
    {
      EnemyI* obj= new EnemyI(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 100, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 10)
    {
      EnemyJ* obj= new EnemyJ(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 100, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 11)
    {
      EnemyK* obj= new EnemyK(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 100, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
    if (this->TypeEnemy[wave] == 12)
    {
      EnemyL* obj= new EnemyL(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 700, this->TypeEnemy[wave]);
      this->Enemys.push_back(obj);
    }
  }
}
