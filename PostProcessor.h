#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Shader.h"

class PostProcessor
{
public:
  PostProcessor(Shader shader, GLuint width, GLuint height);

  GLboolean GetConfuse() { return this->Confuse; }
  GLboolean GetChaos() { return this->Chaos; }
  GLboolean GetShake() { return this->Shake; }
  void SetConfuse(GLboolean confuse)
  {
    this->Confuse = confuse;
  }
  void SetChaos(GLboolean chaos)
  {
    this->Chaos = chaos;
  }
  void SetShake(GLboolean shake)
  {
    this->Shake = shake;
  }
  void BeginRender();
  void EndRender();
  void Render(GLfloat time);
private:
  GLuint MSFBO, FBO;
  GLuint RBO;
  GLuint VAO;
  void initRenderData();
  Shader PostProcessingShader;
  Texture2D Texture;
  GLuint Width, Height;
  GLboolean Confuse, Chaos, Shake;
};
