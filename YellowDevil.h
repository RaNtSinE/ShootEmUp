#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Player.h"

class YellowDevil : public Player
{
public:
  glm::vec2 BulletVelocity;
  GLfloat BulletDamage;
  GLfloat HitPoints;
  YellowDevil()
  : Player() {}
  YellowDevil(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
  : Player(pos, size, sprite, velocity), BulletDamage(50), BulletVelocity(0, -400), HitPoints(400) { }
  // glm::vec2 Move(GLfloat dt, GLuint window_width);
  // void Reset(glm::vec2 position, glm::vec2 velocity);
};
