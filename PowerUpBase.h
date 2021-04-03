#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "YellowDevil.h"
#include "PowerUp.h"
#include "PowerUpArmor.h"
#include "PowerUpSpeed.h"
#include "PowerUpDamage.h"
#include "PowerUpTriple.h"

class PowerUpBase
{
public:
	PowerUpBase(){ }
  const std::vector<PowerUp*>& GetPowerUps() const { return this->PowerUps; }
  void PowerUpsClear() { this->PowerUps.clear(); }
  void SpawnPowerUps(GameObject& enemy)
  {
	  if (ShouldSpawn(13))
	  {
		  PowerUpSpeed* obj = new PowerUpSpeed("speed", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, enemy.GetPosition(), ResourceManager::GetTexture("powerup_speed"));
		  this->PowerUps.push_back(obj);
	  }
	  if (ShouldSpawn(15))
	  {
		  PowerUpArmor* obj = new  PowerUpArmor("armor", glm::vec3(1.0f, 1.0f, 1.0f), 10.0f, enemy.GetPosition(), ResourceManager::GetTexture("powerup_armor"));
		  this->PowerUps.push_back(obj);
	  }
	  if (ShouldSpawn(15))
	  {
		  PowerUpDamage* obj = new  PowerUpDamage("damage", glm::vec3(1.0f, 1.0f, 1.0f), 10.0f, enemy.GetPosition(), ResourceManager::GetTexture("powerup_damage"));
		  this->PowerUps.push_back(obj);
	  }
	  if (ShouldSpawn(16))
	  {
		  PowerUpTriple* obj = new PowerUpTriple("triple_shoot", glm::vec3(1.0f, 1.0f, 1.0f), 10.0f, enemy.GetPosition(), ResourceManager::GetTexture("powerup_triple"));
		  this->PowerUps.push_back(obj);
	  }
	  //if (ShouldSpawn(500))
		 // PowerUps->push_back
		 // (
			//  PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, enemy.GetPosition(), ResourceManager::GetTexture("powerup_confuse"))
		 // );
	  //if (ShouldSpawn(500))
		 // PowerUps->push_back
		 // (
			//  PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, enemy.GetPosition(), ResourceManager::GetTexture("powerup_chaos"))
		 // );
  }

  void UpdatePowerUps(GLfloat dt, Player& Car)
  {
	  for (PowerUp* powerUp : this->GetPowerUps())
	  {
		  powerUp->UpdatePosition(powerUp->GetVelocity() * dt);
		  if (powerUp->GetActivated())
		  {
			  powerUp->UpdateDuration(dt);
			  if (powerUp->GetDuration() <= 0.0f)
			  {
				  powerUp->SetActivated(GL_FALSE);
				  if(!isOtherPowerUpActive(powerUp->GetType()))
					powerUp->SetDefault(Car);
			  }
		  }
	  }
	  this->PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(), [](PowerUp* powerUp)
		  { return powerUp->GetDestroyed() && !powerUp->GetActivated(); }), PowerUps.end());
  }

  void ActivatePowerUp(PowerUp& powerUp, Player &YellowDevilCar)
  {
	  powerUp.SetBuff(YellowDevilCar);
  }

private:
  std::vector<PowerUp*> PowerUps;
  GLboolean ShouldSpawn(GLuint chance)
  {
	  GLuint random = rand() % chance;
	  return random == 0;
  }

  GLboolean isOtherPowerUpActive(std::string type)
  {
	  for (PowerUp* powerUp : this->GetPowerUps())
	  {
		  if (powerUp->GetActivated())
			  if (powerUp->GetType() == type)
				  return GL_TRUE;
	  }
	  return GL_FALSE;
  }
};
