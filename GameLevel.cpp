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
  // for (GameObject &tile : this->Bricks)
  //   if (!tile->Destroyed)
  //     tile->Draw(renderer);
  for (Enemy *tile : this->Enemys)
    if (!tile->Destroyed)
      tile->Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
  // for (GameObject &tile : this->Bricks)
  //   if(!tile->IsSolid && !tile->Destroyed)
  //     return GL_FALSE;
  // return GL_TRUE;
  if (!this->Boss.Destroyed)
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
      // std::cout << tileData[y][x] << ' ';
    }
    this->NumberWaves = this->NumberWaves + 1;
    // std::cout << std::endl;
  }
  // width = this->TimeSpawn.size();
  // for (GLuint x = 0; x < width; ++x)
  // {
  //   std::cout << this->TimeSpawn[x] << ' ';
  // }

  // GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / height;
  //
  // for (GLuint y = 0; y < height; ++y)
  // {
  //   for (GLuint x = 0; x < width; ++x)
  //   {
  //     if (tileData[y][x] == 1)
  //     {
  //       glm::vec2 pos(unit_width * x, unit_height * y);
  //       glm::vec2 size(unit_width, unit_height);
  //       GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
  //       // obj.IsSolid = GL_TRUE;
  //       this->Enemys.push_back(obj);
  //     }
  //     else if (tileData[y][x] > 1)
  //     {
  //       glm::vec3 color = glm::vec3(1.0f);
  //       if (tileData[y][x] == 2)
  //         color = glm::vec3(0.2f, 0.6f, 1.0f);
  //       else if (tileData[y][x] == 3)
  //         color = glm::vec3(0.0f, 0.7f, 0.0f);
  //       else if (tileData[y][x] == 4)
  //         color = glm::vec3(0.8f, 0.8f, 0.4f);
  //       else if (tileData[y][x] == 5)
  //         color = glm::vec3(1.0f, 0.5f, 0.0f);
  //
  //       glm::vec2 pos(unit_width * x, unit_height * y);
  //       glm::vec2 size(unit_width, unit_height);
  //       this->Enemys.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
  //     }
  //   }
  // }
  // glm::vec2 pos(400, 100);
  // glm::vec2 size(50, 100);
  // Enemy obj(pos, size, ResourceManager::GetTexture("car"), glm::vec2(0, 50), 200);
  // this->Enemys.push_back(obj);
}

void GameLevel::SpawnEnemys(GLint wave, GLint line)
{
  for (GLint i = 0; i < this->NumberEnemy[wave]; ++i)
  {
    GLfloat posy = -100 -150*i;
    glm::vec2 pos(210 + 130 * this->Line[wave], posy);
    glm::vec2 size(50, 100);
    // switch (this->TypeEnemy[wave]) {
    //   case 1:
    //     EnemyA* obj= new EnemyA(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 200, this->TypeEnemy[wave]);
    //     this->Enemys.push_back(obj);
    //     break;
    //   default:
    //     Enemy* obj = new Enemy(pos, size, ResourceManager::GetTexture("redcar5"), glm::vec2(0, 80), 200, this->TypeEnemy[wave]);
    //     this->Enemys.push_back(obj);
    //     break;
    // }
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
