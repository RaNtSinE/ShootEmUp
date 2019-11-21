#include <algorithm>
#include <irrKlang.h>

#include <iostream>
#include <math.h>

using namespace irrklang;

#include "Game.h"
#include "Resource_manager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "TextRenderer.h"
#include "Player.h"
#include "YellowDevil.h"
#include "Enemy.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll



ISoundEngine *SoundEngine = createIrrKlangDevice();
SpriteRenderer *Renderer;
GameObject *Player;
// BallObject *Ball;
ParticleGenerator *Particles;
PostProcessor *Effects;
TextRenderer *Text;
YellowDevil *YellowDevilCar;
GLfloat ShakeTime = 0.0f;
// GLfloat Period = 1;
GLboolean GunEnable;
GLboolean Open;
GLboolean knockback = GL_FALSE;
GLboolean PlayerVisible = GL_FALSE;
GLint Tick;
glm::vec2 KnockbackVelocity(0.0f, 0.0f);

Game::Game(GLuint width, GLuint height)
      : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Level(0), Lives(3), Velocity(0), Time(0), PeriodBullet(1), PeriodBulletEnemy(1), PeriodOpen(1), PeriodInvincible(1), WidthLeft(width), HeightUp(height), WidthRight(width), HeightDown(height)
{

}

Game::~Game()
{
  delete Renderer;
  delete Player;
  // delete Ball;
  delete Particles;
  delete Effects;
  delete YellowDevilCar;
  SoundEngine->drop();
}

void Game::Init()
{
  ResourceManager::LoadShader("VertexShader.glsl", "FragmentShader.glsl", nullptr, "sprite");
  ResourceManager::LoadShader("ParticleVertexShader.glsl", "ParticleFragmentShader.glsl", nullptr, "particles");
  ResourceManager::LoadShader("PostVertexShader.glsl", "PostFragmentShader.glsl", nullptr, "postprocessing");
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

  ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  ResourceManager::GetShader("particles").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("particles").SetMatrix4("projection", projection);

  ResourceManager::LoadTexture("Images/Road.png", GL_FALSE, "background");
  ResourceManager::LoadTexture("Images/Car.png", GL_TRUE, "car");
  ResourceManager::LoadTexture("Images/Bullet.png", GL_TRUE, "bullet");
  ResourceManager::LoadTexture("Images/EnemyBullet.png", GL_TRUE, "enemyBullet");
  ResourceManager::LoadTexture("Images/Car1.png", GL_TRUE, "car1");
  ResourceManager::LoadTexture("Images/Car2.png", GL_TRUE, "car2");
  ResourceManager::LoadTexture("Images/Car3.png", GL_TRUE, "car3");
  ResourceManager::LoadTexture("Images/Car4.png", GL_TRUE, "car4");
  ResourceManager::LoadTexture("Images/Car5.png", GL_TRUE, "car5");
  ResourceManager::LoadTexture("Images/RedCar.png", GL_TRUE, "redcar");
  ResourceManager::LoadTexture("Images/RedCar1.png", GL_TRUE, "redcar1");
  ResourceManager::LoadTexture("Images/RedCar2.png", GL_TRUE, "redcar2");
  ResourceManager::LoadTexture("Images/RedCar3.png", GL_TRUE, "redcar3");
  ResourceManager::LoadTexture("Images/RedCar4.png", GL_TRUE, "redcar4");
  ResourceManager::LoadTexture("Images/RedCar5.png", GL_TRUE, "redcar5");

  // ResourceManager::LoadTexture("block_solid.png", GL_FALSE, "block_solid");
  // ResourceManager::LoadTexture("paddle.png", true, "paddle");
  // ResourceManager::LoadTexture("particle.png", GL_TRUE, "particlet");
  // ResourceManager::LoadTexture("powerup_speed.png", GL_TRUE, "powerup_speed");
  // ResourceManager::LoadTexture("powerup_sticky.png", GL_TRUE, "powerup_sticky");
  // ResourceManager::LoadTexture("powerup_increase.png", GL_TRUE, "powerup_increase");
  // ResourceManager::LoadTexture("powerup_confuse.png", GL_TRUE, "powerup_confuse");
  // ResourceManager::LoadTexture("powerup_chaos.png", GL_TRUE, "powerup_chaos");
  // ResourceManager::LoadTexture("powerup_passthrough.png", GL_TRUE, "powerup_passthrough");

  Shader myShader;

  myShader = ResourceManager::GetShader("sprite");

  Renderer = new SpriteRenderer(myShader);

  Particles = new ParticleGenerator(ResourceManager::GetShader("particles"), ResourceManager::GetTexture("particlet"), 500);
  Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

  GameLevel one; one.Load("Levels/Level1.lvl");
  // GameLevel two; two.Load("level2.lvl");
  // GameLevel three; three.Load("level3.lvl");
  // GameLevel four; four.Load("level4.lvl");

  this->Levels.push_back(one);
  // this->Levels.push_back(two);
  // this->Levels.push_back(three);
  // this->Levels.push_back(four);
  this->Level = 0;

  glm::vec2 playerPos = glm::vec2(this->Width / 2 + PLAYER_SIZE.x, this->Height - PLAYER_SIZE.y);

  Texture2D myTexture;

  myTexture = ResourceManager::GetTexture("car");

  glm::vec2 velocityDevil = glm::vec2(500, 300);

  YellowDevilCar = new YellowDevil(playerPos, PLAYER_SIZE, myTexture, velocityDevil);
  //
  // glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
  //
  // myTexture = ResourceManager::GetTexture("bullet");
  //
  // Ball = new BallObject(ballPos, BALL_RADIUS, ININTIAL_BALL_VELOCITY, myTexture);

  irrklang::ISound * snd = SoundEngine->play2D("audio/carpenter_brut_turbo_killer.mp3", GL_TRUE, GL_FALSE, GL_TRUE);
  if(snd)
  snd->setVolume(0.2);

  Text = new TextRenderer(this->Width, this->Height);
  Text->Load("fonts/ocraext.TTF", 24);

  this->WidthRight = this->WidthRight * 0.7657;
  this->WidthLeft = this->WidthLeft / 3.763;

  GunEnable = GL_TRUE;
  Open = GL_FALSE;
  this->Time = 0;
  Tick = 0;
  this->PeriodBullet = this->Time + 1;
  this->PeriodBulletEnemy = this->Time+1;
  this->PeriodOpen = this->Time + 1;
  this->PeriodInvincible = this->Time + 1;
  // myTexture = ResourceManager::GetTexture("car2");
  // Player->Sprite = myTexture;
}

void Game::Update(GLfloat dt)
{
  this->Time += dt;
  GLfloat dtBuf = dt;
  GLint lines = 1;
  // Ball->Move(dt, this->Width);
  this->DoCollisions();
  // std::cout << this->PeriodOpen << std::endl;
  // Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
  for (GLuint i = 0; i < this->Levels[this->Level].NumberWaves; ++i)
  {
    if (this->Time >= this->Levels[this->Level].TimeSpawn[i] && this->Time < this->Levels[this->Level].TimeSpawn[i] + dtBuf)
      this->Levels[this->Level].SpawnEnemys(i, lines);
  }
  this->UpdatePlayer(dt);
  this->UpdatePowerUps(dt);
  this->SpawnBullets(dt);
  this->UpdateBullets(dt);
  this->UpdateEnemys(dt);
  if (ShakeTime > 0.0f)
  {
    ShakeTime -= dt;
    if (ShakeTime <= 0.0f)
      Effects->Shake = GL_FALSE;
  }
  for (Bullet &bullet : this->Bullets)
  {
    // bullet.Position += bullet.Velocity * dt;
    // bullet.Position.y -= 100;
  }
  // if (Ball->Position.y >= this->Height)
  // {
  //   --this->Lives;
  //   if (this->Lives == 0)
  //   {
  //     this->ResetLevel();
  //     this->State = GAME_MENU;
  //   }
  //   this->ResetPlayer();
  // }
  // if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
  // {
  //   this->ResetLevel();
  //   this->ResetPlayer();
  //   Effects->Chaos = GL_TRUE;
  //   this->State = GAME_WIN;
  // }
}

void Game::ProcessInput(GLfloat dt)
{
  if (this->State == GAME_ACTIVE)
  {
    GLfloat velocityX = PLAYER_VELOCITY_X * dt;
    GLfloat velocityY = PLAYER_VELOCITY_Y * dt;

    if (this->Keys[GLFW_KEY_A])
    {
      if (YellowDevilCar->Position.x >= this->WidthLeft)
      {
        if (!YellowDevilCar->Knockback)
          YellowDevilCar->Position.x -= velocityX;
        else
          YellowDevilCar->Position.x -= velocityX / 1.5 - KnockbackVelocity.x * dt;
        // GunEnable = GL_TRUE;
      }

    }
    if (this->Keys[GLFW_KEY_D])
    {
      if (YellowDevilCar->Position.x <= (this->WidthRight - YellowDevilCar->Size.x))
      {
        if (!YellowDevilCar->Knockback)
          YellowDevilCar->Position.x += velocityX;
        else
          YellowDevilCar->Position.x += velocityX / 1.5 - KnockbackVelocity.x * dt;
      }

    }
    if (this->Keys[GLFW_KEY_W])
    {
      if (YellowDevilCar->Position.y >= 0)
      {
        if (!YellowDevilCar->Knockback)
          YellowDevilCar->Position.y -= velocityY;
        else
          YellowDevilCar->Position.y -= velocityY / 1.5 - KnockbackVelocity.y * dt;;
      }
    }
    if (this->Keys[GLFW_KEY_S])
    {
      if (YellowDevilCar->Position.y <= (this->Height - YellowDevilCar->Size.y))
      {
        if (!YellowDevilCar->Knockback)
          YellowDevilCar->Position.y += velocityY;
        else
          YellowDevilCar->Position.y += velocityY / 1.5 - KnockbackVelocity.y * dt;;
      }
    }
    if (this->Keys[GLFW_KEY_R]  && !this->KeysProcessed[GLFW_KEY_R])
    {
      if (Open == GL_FALSE)
      {
        Open = GL_TRUE;
        this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
      }
      else
      {
        Open = GL_FALSE;
        this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
      }
    }
    if (this->Keys[GLFW_KEY_SPACE])
      // Ball->Stuck = false;
      YellowDevilCar->Destroyed = GL_TRUE;
  }
  if (this->State == GAME_MENU)
  {
    if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
    {
      this->State = GAME_ACTIVE;
      this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
    }
    if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
    {
      this->Level = (this->Level + 1) % 4;
      this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
    }
    if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
    {
      if (this->Level > 0)
        --this->Level;
      else
        this->Level = 3;
      this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
    }
  }
  // if (this->State == GAME_WIN)
  // {
  //   if (this->Keys[GLFW_KEY_ENTER])
  //   {
  //     this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
  //     Effects->Chaos = GL_FALSE;
  //     this->State = GAME_MENU;
  //   }
  // }
}

void Game::Render(GLfloat dt)
{

  if (this->State == GAME_ACTIVE || this->State == GAME_WIN)
  {
    Effects->BeginRender();
    Texture2D myTexture;
    myTexture = ResourceManager::GetTexture("background");
    this->Velocity += dt * 600;
    GLfloat upStep = -1*this->Height;
    if (this->Velocity > this->Height)
      this->Velocity = 0;
    Renderer->DrawSprite(myTexture,
        glm::vec2(0, this->Velocity - this->Height), glm::vec2(this->Width, this->Height), 0.0f);
    Renderer->DrawSprite(myTexture,
        glm::vec2(0, 0 + this->Velocity), glm::vec2(this->Width, this->Height), 0.0f);

    this->Levels[this->Level].Draw(*Renderer);
    // myTexture = ResourceManager::GetTexture("car2");
    // Player->Sprite = myTexture;

    if(!YellowDevilCar->Invincible)
      YellowDevilCar->Draw(*Renderer);
    if(this->Time / this->PeriodInvincible >= 1)
    {
      this->PeriodInvincible += 2 * dt;
      PlayerVisible = GL_TRUE;
    }
    else
        PlayerVisible = GL_FALSE;
    if(YellowDevilCar->Invincible && PlayerVisible)
      YellowDevilCar->Draw(*Renderer);  
    // for (PowerUp &powerUp : this->PowerUps)
    //   if (!powerUp.Destroyed)
    //     powerUp.Draw(*Renderer);
    for (Bullet &bullet : this->Bullets)
      if (!bullet.Destroyed)
        bullet.Draw(*Renderer);
    // Particles->Draw();
    // Ball->Draw(*Renderer);
    Effects->EndRender();
    Effects->Render(glfwGetTime());
    // std::stringstream ss; ss << this->Lives;
    // Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
  }
  if (this->State == GAME_MENU)
  {
    Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
    Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
  }
  if (this->State == GAME_WIN)
  {
    Text->RenderText("You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
    Text->RenderText("Press ENTER to menu", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
  }
}

void Game::ResetLevel()
{
  if (this->Level == 0)this->Levels[0].Load("level1.lvl");
  else if (this->Level == 1)
    this->Levels[1].Load("level2.lvl");
  else if (this->Level == 2)
    this->Levels[2].Load("level3.lvl");
  else if (this->Level == 3)
    this->Levels[3].Load("level4.lvl");
  this->Lives = 3;
}

void Game::ResetPlayer()
{
  YellowDevilCar->Size = PLAYER_SIZE;
  YellowDevilCar->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
  // Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), ININTIAL_BALL_VELOCITY);
  Effects->Chaos = Effects->Confuse = GL_FALSE;
  // Ball->PassThrough = Ball->Sticky = GL_FALSE;
  YellowDevilCar->Color = glm::vec3(1.0f);
  // Ball->Color = glm::vec3(1.0f);
}

GLboolean CheckCollision(GameObject &one, GameObject &two);
// Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);
GLboolean ShouldSpawn(GLuint chance);
void ActivatePowerUp(PowerUp &powerUp);
GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void Game::UpdateEnemys(GLfloat dt)
{
  // for(GLint i = 0; i < this->Levels[this->Level].Enemys.size(); ++i)
  // {
  //   this->Levels[this->Level].Enemys[i].Position +=  this->Levels[this->Level].Enemys[i].Velocity * dt;
  //   // this->Levels[this->Level].Enemys[i].Position +=  glm::vec2(0, 50) * dt;
  // }
  for (Enemy &enemy : this->Levels[this->Level].Enemys)
  {
    enemy.Position += enemy.Velocity * dt;
    if(enemy.Knockback == GL_TRUE)
    {
      if (enemy.Position.x >= this->WidthLeft && enemy.Position.x <= (this->WidthRight - enemy.Size.x))
        enemy.Position.x -= KnockbackVelocity.x * dt;
      enemy.Position.y -= KnockbackVelocity.y * dt;
      KnockbackVelocity.x -= KnockbackVelocity.x * 4 * dt;
      KnockbackVelocity.y -= KnockbackVelocity.y * 4 * dt;
      // std::cout << KnockbackVelocity.x << ' ' << KnockbackVelocity.y << std::endl;
      // if (KnockbackVelocity.x <= 1 && KnockbackVelocity.x >= -1)
      // {
      //   KnockbackVelocity.x = 0;
      // }
      // if (KnockbackVelocity.y <= 1 && KnockbackVelocity.y >= -1)
      // {
      //   // enemy.Knockback = GL_FALSE;
      //   KnockbackVelocity.y = 0;
      // }
      if (KnockbackVelocity.x == 0 && KnockbackVelocity.y ==0)
      {
        enemy.Knockback = GL_FALSE;
      }
    }
  }
}

void Game::UpdatePlayer(GLfloat dt)
{
  if(Open == GL_TRUE)
  {
    if(this->Time / this->PeriodOpen >= 1)
    {
      this->PeriodOpen += 5 * dt;
      if(Tick < 5)
        Tick++;
    }
  }
  else
  {
    if(this->Time / this->PeriodOpen >= 1)
    {
      this->PeriodOpen += 5 * dt;
      if(Tick > 0)
        Tick--;
    }
  }
  if(Tick == 0)
  {
    YellowDevilCar->Sprite = ResourceManager::GetTexture("car");
    GunEnable = GL_FALSE;
  }
  if(Tick == 1)
    YellowDevilCar->Sprite = ResourceManager::GetTexture("car1");
  if(Tick == 2)
  {
    YellowDevilCar->Sprite = ResourceManager::GetTexture("car2");
  }
  if(Tick == 3)
  {
    YellowDevilCar->Sprite = ResourceManager::GetTexture("car3");
    GunEnable = GL_FALSE;
  }
  if(Tick == 4)
  {
    YellowDevilCar->Sprite = ResourceManager::GetTexture("car4");
  }
  if(Tick == 5)
  {
    YellowDevilCar->Sprite = ResourceManager::GetTexture("car5");
    GunEnable = GL_TRUE;
  }
  if(YellowDevilCar->Knockback == GL_TRUE)
  {

    if (YellowDevilCar->Position.x >= this->WidthLeft && YellowDevilCar->Position.x <= (this->WidthRight - YellowDevilCar->Size.x))
      YellowDevilCar->Position.x += KnockbackVelocity.x * dt;
    if (YellowDevilCar->Position.y >= 0 && YellowDevilCar->Position.y <= (this->Height - YellowDevilCar->Size.y))
      YellowDevilCar->Position.y += KnockbackVelocity.y * dt;
    KnockbackVelocity.x -= KnockbackVelocity.x * 4 * dt;
    KnockbackVelocity.y -= KnockbackVelocity.y * 4 * dt;
    if (KnockbackVelocity.x <= 1 && KnockbackVelocity.x >= -1)
    {
      KnockbackVelocity.x = 0;
    }
    if (KnockbackVelocity.y <= 1 && KnockbackVelocity.y >= -1)
    {
      // YellowDevilCar->Knockback = GL_FALSE;
      KnockbackVelocity.y = 0;
    }
    if (KnockbackVelocity.x == 0 && KnockbackVelocity.y == 0)
    {
      YellowDevilCar->Knockback = GL_FALSE;
    }
  }
  if (this->Time >= YellowDevilCar->TimeInvincible)
  {
    YellowDevilCar->Invincible = GL_FALSE;
  }
}

void Game::UpdateBullets(GLfloat dt)
{
  for (Bullet &bullet : this->Bullets)
  {
    bullet.Position += bullet.Velocity * dt;
    // bullet.Position.y -= 100 * dt;
  }
  this->Bullets.erase(std::remove_if(this->Bullets.begin(), this->Bullets.end(), [](const Bullet &bullet)
  { return bullet.Destroyed; }), this->Bullets.end());
}

void Game::UpdatePowerUps(GLfloat dt)
{
  for (PowerUp &powerUp : this->PowerUps)
  {
    powerUp.Position += powerUp.Velocity * dt;
    if (powerUp.Activated)
    {
      powerUp.Duration -= dt;
      if (powerUp.Duration <= 0.0f)
      {
        powerUp.Activated = GL_FALSE;

        if (powerUp.Type == "sticky")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "sticky"))
          {
            // Ball->Sticky = GL_FALSE;
            YellowDevilCar->Color = glm::vec3(1.0f);
          }
        }
        else if (powerUp.Type == "pass-through")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
          {
            // Ball->PassThrough = GL_FALSE;
            // Ball->Color = glm::vec3(1.0f);
          }
        }
        else if (powerUp.Type == "confuse")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "confuse"))
          {
            Effects->Confuse = GL_FALSE;
          }
        }
        else if (powerUp.Type == "chaos")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "chaos"))
          {
            Effects->Chaos = GL_FALSE;
          }
        }
      }
    }
  }
  this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(), [](const PowerUp &powerUp)
  { return powerUp.Destroyed && !powerUp.Activated; }), this->PowerUps.end());
}

void Game::SpawnBullets(GLfloat dt)
{
  glm::vec2 bulPos(YellowDevilCar->Position.x + PLAYER_SIZE.x / 2 - BULLET_SIZE / 2, YellowDevilCar->Position.y);
  if (this->Time / this->PeriodBullet >= 1 )
  {
    this->PeriodBullet += 20 * dt;
    if (GunEnable)
    this->Bullets.push_back
    (
      Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("bullet"), YellowDevilCar->BulletVelocity, GL_TRUE, GL_FALSE)
    );
  }
  if (this->Time / this->PeriodBulletEnemy >= 1 )
  {
    for (Enemy &enemy : this->Levels[this->Level].Enemys)
    {
      bulPos = glm::vec2(enemy.Position.x + enemy.Size.x / 2 - BULLET_SIZE, enemy.Position.y + enemy.Size.y / 1.5);
      glm::vec2 velocity(0.0f, 0.0f);
      float x, y;
      x = enemy.Position.x - YellowDevilCar->Position.x;
      y = enemy.Position.y - YellowDevilCar->Position.y;
      velocity.x = sqrt((enemy.BulletVelocity * enemy.BulletVelocity * x * x) / (x * x + y * y));
      velocity.y = abs(y / x * velocity.x);
      if (enemy.Position.x > YellowDevilCar->Position.x)
        velocity.x = -velocity.x;
      if (enemy.Position.y > YellowDevilCar->Position.y)
        velocity.y = -velocity.y;

      if (!enemy.Destroyed)
      this->Bullets.push_back
      (
        Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity, GL_FALSE, GL_TRUE)
      );
    }
      this->PeriodBulletEnemy += 100 * dt;
  }
}

void Game::SpawnPowerUps(GameObject &block)
{
  if (ShouldSpawn(75))
    this->PowerUps.push_back
    (
      PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed"))
    );
  if (ShouldSpawn(75))
    this->PowerUps.push_back
    (
      PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky"))
    );
  if (ShouldSpawn(75))
    this->PowerUps.push_back
    (
      PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough"))
    );
  if (ShouldSpawn(75))
    this->PowerUps.push_back
    (
      PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase"))
    );
  if (ShouldSpawn(15))
    this->PowerUps.push_back
    (
      PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse"))
    );
  if (ShouldSpawn(75))
    this->PowerUps.push_back
    (
      PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos"))
    );
}

void Game::DoCollisions()
{
  for (Enemy &enemy : this->Levels[this->Level].Enemys)
  {
    if (!enemy.Destroyed)
    {
      for (Bullet &bullet : this->Bullets)
      // Collision collision = CheckCollision(bullet, enemy);
      if (CheckCollision(bullet, enemy) && bullet.Ally)
      {
        enemy.HitPoints -= YellowDevilCar->BulletDamage;
        if (enemy.HitPoints <= 0)
        {
          enemy.Destroyed = GL_TRUE;
          // ShakeTime = 0.05f;
          // Effects->Shake = GL_TRUE;
        }
        bullet.Destroyed = GL_TRUE;
      }

    //   {
    //     // if (!box.IsSolid)
    //     // {
    //     //   box.Destroyed = GL_TRUE;
    //     //   this->SpawnPowerUps(box);
    //     //   SoundEngine->play2D("audio/bleep.mp3", GL_FALSE);
    //     // }
    //     // else
    //     // {
    //     //   ShakeTime = 0.05f;
    //     //   Effects->Shake = GL_TRUE;
    //     //   SoundEngine->play2D("audio/solid.wav", GL_FALSE);
    //     // }
    //     Direction dir = std::get<1>(collision);
    //     glm::vec2 diff_vector = std::get<2>(collision);
    //     // if (!(Ball->PassThrough && !box.IsSolid))
    //     // {
    //     //   if (dir == LEFT || dir == RIGHT)
    //     //   {
    //     //     Ball->Velocity.x = -Ball->Velocity.x;
    //     //     GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
    //     //     if (dir == LEFT)
    //     //       Ball->Position.x += penetration;
    //     //     else
    //     //       Ball->Position.x -= penetration;
    //     //   }
    //     //   else
    //     //   {
    //     //     Ball->Velocity.y = -Ball->Velocity.y;
    //     //     GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
    //     //     if (dir == UP)
    //     //       Ball->Position.y -= penetration;
    //     //     else
    //     //       Ball->Position.y += penetration;
    //     //   }
    //     // }
    //   }
    }
    for (Bullet &bullet : this->Bullets)
    // Collision collision = CheckCollision(bullet, enemy);
    if (CheckCollision(bullet, *YellowDevilCar) && bullet.Enemy && !YellowDevilCar->Invincible)
    {
      YellowDevilCar->HitPoints -= enemy.BulletDamage;
      ShakeTime = 0.1f;
      Effects->Shake = GL_TRUE;
      if (YellowDevilCar->HitPoints <= 0)
      {
        YellowDevilCar->Destroyed = GL_TRUE;
        // ShakeTime = 0.05f;
        // Effects->Shake = GL_TRUE;
      }
      bullet.Destroyed = GL_TRUE;
      YellowDevilCar->Invincible = GL_TRUE;
      YellowDevilCar->TimeInvincible = this->Time + 1;
    }
    if (!enemy.Destroyed)
      if (CheckCollision(enemy, *YellowDevilCar) && !YellowDevilCar->Invincible)
      {
        YellowDevilCar->HitPoints -= enemy.BulletDamage * 2;
        ShakeTime = 0.11f;
        Effects->Shake = GL_TRUE;
        enemy.HitPoints -= YellowDevilCar->BulletDamage * 2;
        // glm::vec2 velocity(0.0f, 0.0f);
        float x, y;
        x = enemy.Position.x - YellowDevilCar->Position.x;
        y = enemy.Position.y - YellowDevilCar->Position.y;
        KnockbackVelocity.x = sqrt((enemy.BulletVelocity * enemy.BulletVelocity * x * x * 4) / (x * x + y * y));
        KnockbackVelocity.y = abs(y / x * KnockbackVelocity.x);
        if (enemy.Position.x > YellowDevilCar->Position.x)
          KnockbackVelocity.x = -KnockbackVelocity.x;
        if (enemy.Position.y > YellowDevilCar->Position.y)
          KnockbackVelocity.y = -KnockbackVelocity.y;
        enemy.Knockback = GL_TRUE;
        YellowDevilCar->Knockback = GL_TRUE;

        if (enemy.HitPoints <= 0)
        {
          enemy.Destroyed = GL_TRUE;
          enemy.Knockback = GL_FALSE;
        }
        YellowDevilCar->Invincible = GL_TRUE;
        YellowDevilCar->TimeInvincible = this->Time + 1;
      }
  }

  for (PowerUp &powerUp : this->PowerUps)
  {
    if (!powerUp.Destroyed)
    {
      if (powerUp.Position.y >= this->Height)
        powerUp.Destroyed = GL_TRUE;
      if (CheckCollision(*YellowDevilCar, powerUp))
      {
        ActivatePowerUp(powerUp);
        powerUp.Destroyed = GL_TRUE;
        powerUp.Activated = GL_TRUE;
        SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
      }
    }
  }

  for (Bullet &bullet : this->Bullets)
  {
    if (!bullet.Destroyed)
    {
      if (bullet.Position.y >= this->Height || bullet.Position.y <= 0)
        bullet.Destroyed = GL_TRUE;
      // if (CheckCollision(*Player, powerUp))
      // {
      //   ActivatePowerUp(powerUp);
      //   powerUp.Destroyed = GL_TRUE;
      //   powerUp.Activated = GL_TRUE;
      //   SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
      // }
    }
  }

  // Collision result = CheckCollision(*Ball, *Player);
  // if (!Ball->Stuck && std::get<0>(result))
  // {
  //   GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
  //   GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
  //   GLfloat percentage = distance / (Player->Size.x / 2);
  //
  //   GLfloat strength = 2.0f;
  //   glm::vec2 oldVelocity = Ball->Velocity;
  //   Ball->Velocity.x = ININTIAL_BALL_VELOCITY.x * percentage * strength;
  //   Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
  //   Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
  //   Ball->Stuck = Ball->Sticky;
  //
  //   SoundEngine->play2D("audio/bleep.wav", GL_FALSE);
  // }
}

GLboolean CheckCollision(GameObject &one, GameObject &two)
{
  bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
    two.Position.x + two.Size.x >= one.Position.x;

  bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
    two.Position.y + two.Size.y >= one.Position.y;
  return collisionX && collisionY;
}


// Collision CheckCollision(BallObject &one, GameObject &two)
// {
//   glm::vec2 center(one.Position + one.Radius);
//   glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
//   glm::vec2 aabb_center
//   (
//     two.Position.x + aabb_half_extents.x,
//     two.Position.y + aabb_half_extents.y
//   );
//   glm::vec2 difference = center - aabb_center;
//   glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
//   glm::vec2 closest = aabb_center + clamped;
//   difference = closest - center;
//   if (glm::length(difference) < one.Radius)
//     return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
//   else
//     return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
// }

Direction VectorDirection(glm::vec2 target)
{
  glm::vec2 compass[] =
  {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, -1.0f),
    glm::vec2(-1.0f, 0.0f)
  };
  GLfloat max = 0.0f;
  GLuint best_match = -1;
  for (GLuint i = 0; i < 4; i++)
  {
    GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
    if (dot_product > max)
    {
      max = dot_product;
      best_match = i;
    }
  }
  return (Direction)best_match;
}

GLboolean ShouldSpawn(GLuint chance)
{
  GLuint random = rand() % chance;
  return random == 0;
}

void ActivatePowerUp(PowerUp &powerUp)
{
  // if (powerUp.Type == "speed")
  // {
  //   Ball->Velocity *= 1.2;
  // }
  // else if (powerUp.Type == "sticky")
  // {
  //   Ball->Sticky = GL_TRUE;
  //   Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
  // }
  // else if (powerUp.Type == "pass-through")
  // {
  //   Ball->PassThrough = GL_TRUE;
  //   Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
  // }
  // else if (powerUp.Type == "pad-size-increase")
  // {
  //   Player->Size.x += 50;
  // }
  // else if (powerUp.Type == "confuse")
  // {
  //   if (!Effects->Chaos)
  //     Effects->Confuse = GL_TRUE;
  // }
  // else if (powerUp.Type == "chaos")
  // {
  //   if (!Effects->Confuse)
  //     Effects->Chaos = GL_TRUE;
  // }
}

GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
  for (const PowerUp &powerUp : powerUps)
  {
    if (powerUp.Activated)
      if (powerUp.Type == type)
        return GL_TRUE;
  }
  return GL_FALSE;
}
