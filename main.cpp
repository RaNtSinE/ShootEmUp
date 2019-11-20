#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game.h"
#include "Resource_manager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 1066;
const GLuint SCREEN_HEIGHT = 600;

Game ShootEmUp(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ShootEmUp", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();
  glGetError();

  glfwSetKeyCallback(window, key_callback);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ShootEmUp.Init();

  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;

  ShootEmUp.State = GAME_ACTIVE;

  while (!glfwWindowShouldClose(window))
  {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();

    ShootEmUp.ProcessInput(deltaTime);

    ShootEmUp.Update(deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ShootEmUp.Render(deltaTime);

    glfwSwapBuffers(window);
  }

  ResourceManager::Clear();

  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  // if (key == GLFW_KEY_ESCAPE && ShootEmUp.State != GAME_MENU)
  // {
  //   ShootEmUp.State = GAME_MENU;
  //   ShootEmUp.ResetLevel();
  //   ShootEmUp.ResetPlayer();
  // }
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS)
      ShootEmUp.Keys[key] = GL_TRUE;
    else if (action == GLFW_RELEASE)
      ShootEmUp.Keys[key] = GL_FALSE;
      ShootEmUp.KeysProcessed[key] = GL_FALSE;
  }
}
