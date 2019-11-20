#pragma once
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"

const glm::vec2 SIZE_BULLET(15, 15);
const glm::vec2 VELOCITY_BULLET(0.0f, -400.0f);

class Bullet : public GameObject
{
public:
  GLboolean Ally;
  GLboolean Enemy;

  Bullet(glm::vec3 color, glm::vec2 position, Texture2D texture, glm::vec2 bulletVelocity, GLboolean ally, GLboolean enemy)
    : GameObject(position, SIZE_BULLET, texture, color, bulletVelocity), Ally(ally), Enemy(enemy) { }
};
