#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
  GameObject();
  GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

  GLfloat GetPositionX() { return this->Position.x; }
  GLfloat GetPositionY() { return this->Position.y; }
  glm::vec2 GetPosition() { return this->Position; }
  GLfloat GetSizeX() { return this->Size.x; }
  GLfloat GetSizeY() { return this->Size.y; }
  glm::vec2 GetSize() { return this->Size; }
  GLfloat GetVelocityX() { return this->Velocity.x; }
  GLfloat GetVelocityY() { return this->Velocity.y; }
  glm::vec2 GetVelocity() { return this->Velocity; }
  GLboolean GetDestroyed() const { return this->Destroyed; }
  void SetPositionX(GLfloat posx) { this->Position.x = posx; }
  void SetPositionY(GLfloat posy) { this->Position.y = posy; }
  void SetColor(glm::vec3 color) { this->Color = color; }
  void SetRotation(GLfloat rotation) { this->Rotation = rotation; }
  void SetDestroyed(GLboolean destroyed) { this->Destroyed = destroyed; }
  void UpdatePositionX(GLfloat posx) { this->Position.x += posx; }
  void UpdatePositionY(GLfloat posy) { this->Position.y += posy; }
  void UpdatePosition(glm::vec2 pos) { this->Position += pos; }
  void UpdateRotation(GLfloat rotation) { this->Rotation += rotation; }

  virtual void Draw(SpriteRenderer &renderer);
  virtual void SetTexture(Texture2D sprite);

protected:
  glm::vec2 Position, Size, Velocity;
  glm::vec3 Color;
  GLfloat Rotation;
  GLboolean Destroyed;
  Texture2D Sprite;
};
