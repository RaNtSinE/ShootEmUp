#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

class Boom : public GameObject
{
public:
  Boom()
  : GameObject() {}
  Boom(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : GameObject(pos, size, sprite, glm::vec3(1.0f), velocity), Period(1), Tick(0), Explosive(GL_FALSE) { }
  
  GLfloat GetPeriod() { return this->Period; }
  GLint GetTick() { return this->Tick; }
  //void StartUpdate(GLfloat time) 
  //{ 

  //}
  void ResetBoom()
  {
	  this->Period = 1;
	  this->Tick = 0;
	  this->Explosive = GL_FALSE;
  }
  void UpdateBoom(GLfloat time, GLfloat dt) 
  {
	  if (time >= 3 && this->Explosive == GL_FALSE)
	  {
		  this->Period = time;
		  this->Explosive = GL_TRUE;
	  }

	  if (this->Explosive == GL_TRUE && time / this->Period >= 1)
	  {
		  this->Period += 5 * dt;
		  this->Tick++;
	  }
	  if (this->GetTick() == 0)
	  {
		  this->SetTexture(ResourceManager::GetTexture("boom1"));
	  }
	  if (this->GetTick() == 1)
	  {
		  this->SetTexture(ResourceManager::GetTexture("boom2"));
	  }
	  if (this->GetTick() == 2)
	  {
		  this->SetTexture(ResourceManager::GetTexture("boom3"));
	  }
	  if (this->GetTick() == 3)
	  {
		  this->SetTexture(ResourceManager::GetTexture("boom4"));
	  }
	  if (this->GetTick() == 4)
	  {
		  this->SetDestroyed(GL_TRUE);
	  }
  }
private:
  GLfloat Period;
  GLint Tick;
  GLboolean Explosive;
};
