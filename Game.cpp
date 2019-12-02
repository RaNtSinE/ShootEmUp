#include <algorithm>
#include <irrKlang.h>

#include <iostream>
#include <math.h>

using namespace irrklang;

#include "Game.h"
#include "Resource_manager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
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
GLboolean Explore = GL_FALSE;
glm::vec2 KnockbackVelocity(0.0f, 0.0f);

Game::Game(GLuint width, GLuint height)
      : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Level(0), Lives(3), Velocity(0), Time(0), PeriodBullet(1), PeriodBulletEnemy(1), PeriodOpen(1), PeriodInvincible(1), PeriodBoom(1), WidthLeft(width), HeightUp(height), WidthRight(width), HeightDown(height)
{

}

Game::~Game()
{
  delete Renderer;
  delete Player;
  delete Particles;
  delete Effects;
  delete YellowDevilCar;
  SoundEngine->drop();
}

void Game::Init()
{
  ResourceManager::LoadShader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl", nullptr, "sprite");
  ResourceManager::LoadShader("Shaders/ParticleVertexShader.glsl", "Shaders/ParticleFragmentShader.glsl", nullptr, "particles");
  ResourceManager::LoadShader("Shaders/PostVertexShader.glsl", "Shaders/PostFragmentShader.glsl", nullptr, "postprocessing");
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
  ResourceManager::LoadTexture("Images/RedCar5Damage.png", GL_TRUE, "redcar5damage");
  ResourceManager::LoadTexture("Images/Boom1.png", GL_TRUE, "boom1");
  ResourceManager::LoadTexture("Images/Boom2.png", GL_TRUE, "boom2");
  ResourceManager::LoadTexture("Images/Boom3.png", GL_TRUE, "boom3");
  ResourceManager::LoadTexture("Images/Boom4.png", GL_TRUE, "boom4");
  ResourceManager::LoadTexture("Images/Wheel.png", GL_TRUE, "wheel");

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

  irrklang::ISound * snd = SoundEngine->play2D("audio/carpenter_brut_turbo_killer.mp3", GL_TRUE, GL_FALSE, GL_TRUE);
  if(snd)
  snd->setVolume(0.2);

  Text = new TextRenderer(this->Width, this->Height);
  Text->Load("fonts/ocraext.TTF", 24);

  this->WidthRight = this->WidthRight * 0.7657;
  this->WidthLeft = this->WidthLeft / 3.763;

  // GunEnable = GL_TRUE;
  // Open = GL_FALSE;
  this->Time = 0;
  Tick = 0;
  this->PeriodBullet = this->Time + 1;
  this->PeriodBulletEnemy = this->Time+1;
  this->PeriodOpen = this->Time + 1;
  this->PeriodInvincible = this->Time + 1;
  this->PeriodBoom = this->Time + 1;
  // myTexture = ResourceManager::GetTexture("wheel");
  YellowDevilCar->SetWheelTexture();
  // YellowDevilCar->Wheel_2.SetTexture(myTexture);
}

void Game::Update(GLfloat dt)
{
  this->Time += dt;
  GLfloat dtBuf = dt;
  GLint lines = 1;
  this->DoCollisions(dt);
  // Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
  for (GLuint i = 0; i < this->Levels[this->Level].GetNumberWaves(); ++i)
  {
    if (this->Time >= this->Levels[this->Level].GetTimeSpawn(i) && this->Time < this->Levels[this->Level].GetTimeSpawn(i) + dtBuf)
      this->Levels[this->Level].SpawnEnemys(i, lines);
  }
  YellowDevilCar->UpdatePlayer(dt, this->WidthLeft, this->WidthRight, this->Height, this->Time);
  this->UpdatePowerUps(dt);
  this->SpawnBullets(dt);
  this->UpdateBullets(dt);
  this->UpdateEnemys(dt);
  this->UpdateBooms(dt);
  if (ShakeTime > 0.0f)
  {
    ShakeTime -= dt;
    if (ShakeTime <= 0.0f)
      Effects->SetShake(GL_FALSE);
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
      if (YellowDevilCar->GetPositionX() >= this->WidthLeft)
      {
        YellowDevilCar->SetRotateLeft(GL_TRUE);
        if (!YellowDevilCar->GetKnockback())
          YellowDevilCar->MoveLeft(velocityX);
        else
          YellowDevilCar->UpdatePositionX(-1 * (velocityX / 1.5 - YellowDevilCar->GetKnockbackVelocityX() * dt));
      }
    }
    else
    {
      YellowDevilCar->SetRotateLeft(GL_FALSE);
    }
    if (this->Keys[GLFW_KEY_D])
    {
      if (YellowDevilCar->GetPositionX() <= (this->WidthRight - YellowDevilCar->GetSizeX()))
      {
        YellowDevilCar->SetRotateRight(GL_TRUE);
        if (!YellowDevilCar->GetKnockback())
          YellowDevilCar->MoveRight(velocityX);
        else
          YellowDevilCar->UpdatePositionX(velocityX / 1.5 - YellowDevilCar->GetKnockbackVelocityX() * dt);
      }
    }
    else
    {
      YellowDevilCar->SetRotateRight(GL_FALSE);
    }
    if (this->Keys[GLFW_KEY_W])
    {
      if (YellowDevilCar->GetPositionY() >= 0)
      {
        if (!YellowDevilCar->GetKnockback())
          // YellowDevilCar->Position.y -= velocityY;
          YellowDevilCar->MoveForward(velocityY);
        else
          YellowDevilCar->UpdatePositionY(-1 * (velocityY / 1.5 - YellowDevilCar->GetKnockbackVelocityY() * dt));
      }
    }
    if (this->Keys[GLFW_KEY_S])
    {
      if (YellowDevilCar->GetPositionY() <= (this->Height - YellowDevilCar->GetSizeY()))
      {
        if (!YellowDevilCar->GetKnockback())
          // YellowDevilCar->Position.y += velocityY;
          YellowDevilCar->MoveBack(velocityY);
        else
          YellowDevilCar->UpdatePositionY(velocityY / 1.5 - YellowDevilCar->GetKnockbackVelocityY() * dt);
      }
    }
    if (this->Keys[GLFW_KEY_R]  && !this->KeysProcessed[GLFW_KEY_R])
    {
      if (!YellowDevilCar->GetOpen())
      {
        // Open = GL_TRUE;
        if (!YellowDevilCar->GetStartOpen())
        {
          YellowDevilCar->SetPeriodOpen(this->Time);
          YellowDevilCar->SetStartOpen(GL_TRUE);
        }
        YellowDevilCar->SetOpen(GL_TRUE);
        this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
      }
      else
      {
        // Open = GL_FALSE;
        YellowDevilCar->SetOpen(GL_FALSE);
        this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
      }
    }
    if (this->Keys[GLFW_KEY_SPACE])
      // Ball->Stuck = false;
      YellowDevilCar->SetDestroyed(GL_TRUE);
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
    this->Velocity += dt * 1000;
    GLfloat upStep = -1*this->Height;
    if (this->Velocity > this->Height)
      this->Velocity = 0;
    Renderer->DrawSprite(myTexture,
        glm::vec2(0, this->Velocity - this->Height), glm::vec2(this->Width, this->Height), 0.0f);
    Renderer->DrawSprite(myTexture,
        glm::vec2(0, 0 + this->Velocity), glm::vec2(this->Width, this->Height), 0.0f);

    this->Levels[this->Level].Draw(*Renderer);
    if(!YellowDevilCar->GetInvincible())
    {
      YellowDevilCar->DrawWheel_1(*Renderer);
      YellowDevilCar->DrawWheel_2(*Renderer);
      YellowDevilCar->Draw(*Renderer);
    }
    if(this->Time / this->PeriodInvincible >= 1)
    {
      this->PeriodInvincible += 2 * dt;
      PlayerVisible = GL_TRUE;
    }
    else
        PlayerVisible = GL_FALSE;
    if(YellowDevilCar->GetInvincible() && PlayerVisible)
    {
      YellowDevilCar->DrawWheel_1(*Renderer);
      YellowDevilCar->DrawWheel_2(*Renderer);
      YellowDevilCar->Draw(*Renderer);
    }
    // for (PowerUp &powerUp : this->PowerUps)
    //   if (!powerUp.Destroyed)
    //     powerUp.Draw(*Renderer);
    for (Bullet &bullet : this->Bullets)
      if (!bullet.GetDestroyed())
        bullet.Draw(*Renderer);
    for (Boom &boom : this->Booms)
      if (!boom.GetDestroyed())
        boom.Draw(*Renderer);
    // Particles->Draw();
    Effects->EndRender();
    Effects->Render(glfwGetTime());

    // std::stringstream ss; ss << this->Lives;
    // Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
  }
  // if (this->State == GAME_MENU)
  // {
  //   Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
  //   Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
  // }
  // if (this->State == GAME_WIN)
  // {
  //   Text->RenderText("You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
  //   Text->RenderText("Press ENTER to menu", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
  // }
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
  // YellowDevilCar->Size = PLAYER_SIZE;
  // YellowDevilCar->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
  Effects->SetChaos(GL_FALSE); Effects->SetConfuse(GL_FALSE);
  YellowDevilCar->SetColor(glm::vec3(1.0f));
}

GLboolean CheckCollision(GameObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);
GLboolean ShouldSpawn(GLuint chance);
void ActivatePowerUp(PowerUp &powerUp);
GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void Game::UpdateEnemys(GLfloat dt)
{
  for (Enemy *enemy : this->Levels[this->Level].GetEnemys())
  {
    if(enemy->GetKnockback())
    {
      if (enemy->GetPositionX() >= this->WidthLeft && enemy->GetPositionX() <= (this->WidthRight - enemy->GetSizeX()))
        enemy->UpdatePositionX(-KnockbackVelocity.x * dt);
      enemy->UpdatePositionY(-KnockbackVelocity.y * dt);
      KnockbackVelocity.x -= KnockbackVelocity.x * 4 * dt;
      KnockbackVelocity.y -= KnockbackVelocity.y * 4 * dt;
      if (KnockbackVelocity.x <= 1 && KnockbackVelocity.x >= -1)
      {
        KnockbackVelocity.x = 0;
      }
      if (KnockbackVelocity.y <= 1 && KnockbackVelocity.y >= -1)
      {
        // this->Knockback = GL_FALSE;
        KnockbackVelocity.y = 0;
      }
      if (KnockbackVelocity.x == 0 && KnockbackVelocity.y ==0)
      {
        enemy->SetKnockback(GL_FALSE);
      }
    }
    enemy->UpdateEnemy(dt, this->WidthLeft, this->WidthRight, this->Width, this->Height, this->Time);
  }
}

void Game::UpdateBullets(GLfloat dt)
{
  for (Bullet &bullet : this->Bullets)
  {
    bullet.UpdatePosition(bullet.GetVelocity() * dt);
    // bullet.Position.y -= 100 * dt;
  }
  this->Bullets.erase(std::remove_if(this->Bullets.begin(), this->Bullets.end(), [](const Bullet &bullet)
  { return bullet.GetDestroyed(); }), this->Bullets.end());
}

void Game::UpdateBooms(GLfloat dt)
{
  if(this->Time / this->PeriodBoom >= 1)
  {
    this->PeriodBoom += 5 * dt;
    Explore = GL_TRUE;
  }
  else
    Explore = GL_FALSE;
  for (Boom &boom : this->Booms)
  {
    // boom.Position += boom.Velocity * dt;
    // bullet.Position.y -= 100 * dt;
      if(boom.GetTick() < 5 && Explore == GL_TRUE)
        boom.UpdateBoom();
    if(boom.GetTick() == 0)
    {
      boom.SetTexture(ResourceManager::GetTexture("boom1"));
    }
    if(boom.GetTick() == 1)
    {
      boom.SetTexture(ResourceManager::GetTexture("boom2"));
    }
    if(boom.GetTick() == 2)
    {
      boom.SetTexture(ResourceManager::GetTexture("boom3"));
    }
    if(boom.GetTick() == 3)
    {
      boom.SetTexture(ResourceManager::GetTexture("boom4"));
    }
    if(boom.GetTick() == 4)
    {
      boom.SetDestroyed(GL_TRUE);
    }
  }
  this->Booms.erase(std::remove_if(this->Booms.begin(), this->Booms.end(), [](const Boom &boom)
  { return boom.GetDestroyed(); }), this->Booms.end());
}
void Game::UpdatePowerUps(GLfloat dt)
{
  for (PowerUp &powerUp : this->PowerUps)
  {
    powerUp.UpdatePosition(powerUp.GetVelocity() * dt);
    if (powerUp.GetActivated())
    {
      powerUp.UpdateDuration(dt);
      if (powerUp.GetDuration() <= 0.0f)
      {
        powerUp.SetActivated(GL_FALSE);

        if (powerUp.GetType() == "sticky")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "sticky"))
          {
            // Ball->Sticky = GL_FALSE;
            YellowDevilCar->SetColor(glm::vec3(1.0f));
          }
        }
        else if (powerUp.GetType() == "pass-through")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
          {
            // Ball->PassThrough = GL_FALSE;
            // Ball->Color = glm::vec3(1.0f);
          }
        }
        else if (powerUp.GetType() == "confuse")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "confuse"))
          {
            Effects->SetConfuse(GL_FALSE);
          }
        }
        else if (powerUp.GetType() == "chaos")
        {
          if (!isOtherPowerUpActive(this->PowerUps, "chaos"))
          {
            Effects->SetChaos(GL_FALSE);
          }
        }
      }
    }
  }
  this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(), [](const PowerUp &powerUp)
  { return powerUp.GetDestroyed() && !powerUp.GetActivated(); }), this->PowerUps.end());
}

void Game::SpawnBullets(GLfloat dt)
{
  glm::vec2 bulPos(YellowDevilCar->GetPositionX() + PLAYER_SIZE.x / 2 - BULLET_SIZE / 2, YellowDevilCar->GetPositionY());
  if (this->Time / YellowDevilCar->GetPeriodBullet() >= 1 )
  {
    YellowDevilCar->UpdatePeriodBullet(20 * dt);
    if (YellowDevilCar->GetGunEnable())
    this->Bullets.push_back
    (
      Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("bullet"), YellowDevilCar->GetBulletVelocity(), GL_TRUE, GL_FALSE)
    );
  }
  // if (this->Time / this->PeriodBulletEnemy >= 1 )
  // {
    for (Enemy *enemy : this->Levels[this->Level].GetEnemys())
    {
      // bulPos = glm::vec2(enemy->Position.x + enemy->Size.x / 2 - BULLET_SIZE, enemy->Position.y + enemy->Size.y / 1.5);
      // glm::vec2 velocity(0.0f, 0.0f);
      // float x, y;
      // x = enemy->Position.x - YellowDevilCar->Position.x;
      // y = enemy->Position.y - YellowDevilCar->Position.y;
      // velocity.x = sqrt((enemy->BulletVelocity * enemy->BulletVelocity * x * x) / (x * x + y * y));
      // velocity.y = abs(y / x * velocity.x);
      // if (enemy->Position.x > YellowDevilCar->Position.x)
      //   velocity.x = -velocity.x;
      // if (enemy->Position.y > YellowDevilCar->Position.y)
      //   velocity.y = -velocity.y;
      //
      // if (!enemy->Destroyed)
      // this->Bullets.push_back
      // (
      //   Bullet(glm::vec3(1.0f, 1.0f, 1.0f), bulPos, ResourceManager::GetTexture("enemyBullet"), velocity, GL_FALSE, GL_TRUE)
      // );

      enemy->SpawnBullets(this->Time, dt, YellowDevilCar->GetPositionX(), YellowDevilCar->GetPositionY(), &Bullets);
    }
      // this->PeriodBulletEnemy += 100 * dt;

}

void Game::SpawnBooms(GameObject &enemy, GLfloat dt)
{
    glm::vec2 boomPos(enemy.GetPositionX() - enemy.GetSizeX() * 0.1, enemy.GetPositionY() + enemy.GetSizeY() * 0.3);
    this->Booms.push_back
    (
      Boom(boomPos, glm::vec2(enemy.GetSizeY() * 0.8, enemy.GetSizeY() * 0.8), ResourceManager::GetTexture("boom1"), glm::vec2(0.0f, 0.0f))
    );
}

void Game::SpawnPowerUps(GameObject &block)
{
  // if (ShouldSpawn(75))
  //   this->PowerUps.push_back
  //   (
  //     PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed"))
  //   );
  // if (ShouldSpawn(75))
  //   this->PowerUps.push_back
  //   (
  //     PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky"))
  //   );
  // if (ShouldSpawn(75))
  //   this->PowerUps.push_back
  //   (
  //     PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough"))
  //   );
  // if (ShouldSpawn(75))
  //   this->PowerUps.push_back
  //   (
  //     PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase"))
  //   );
  // if (ShouldSpawn(15))
  //   this->PowerUps.push_back
  //   (
  //     PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse"))
  //   );
  // if (ShouldSpawn(75))
  //   this->PowerUps.push_back
  //   (
  //     PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos"))
  //   );
}

void Game::DoCollisions(GLfloat dt)
{
  for (Enemy *enemy : this->Levels[this->Level].GetEnemys())
  {
    if (!enemy->GetDestroyed())
    {
      for (Bullet &bullet : this->Bullets)
      // Collision collision = CheckCollision(bullet, enemy);
      if (CheckCollision(bullet, *enemy) && bullet.GetAlly())
      {
        enemy->UpdateHitPoints(YellowDevilCar->GetBulletDamage());
        enemy->SetDamage(GL_TRUE);
        if (enemy->GetHitPoints() <= 0)
        {
          enemy->SetDestroyed(GL_TRUE);
          this->SpawnBooms(*enemy, dt);
          // ShakeTime = 0.05f;
          // Effects->Shake = GL_TRUE;
        }
        bullet.SetDestroyed(GL_TRUE);
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
    if (CheckCollision(bullet, *YellowDevilCar) && bullet.GetEnemy() && !YellowDevilCar->GetInvincible())
    {
      YellowDevilCar->UpdateHitPoints(enemy->GetBulletDamage());
      ShakeTime = 0.1f;
      Effects->SetShake(GL_TRUE);
      if (YellowDevilCar->GetHitPoints() <= 0)
      {
        YellowDevilCar->SetDestroyed(GL_TRUE);
        // ShakeTime = 0.05f;
        // Effects->Shake = GL_TRUE;
      }
      bullet.SetDestroyed(GL_TRUE);
      YellowDevilCar->SetInvincible(GL_TRUE);
      YellowDevilCar->SetTimeInvincible(this->Time + 1);
    }
    if (!enemy->GetDestroyed())
      if (CheckCollision(*enemy, *YellowDevilCar) && !YellowDevilCar->GetInvincible())
      {
        YellowDevilCar->UpdateHitPoints(enemy->GetBulletDamage() * 2);
        ShakeTime = 0.11f;
        Effects->SetShake(GL_TRUE);
        enemy->UpdateHitPoints(YellowDevilCar->GetBulletDamage() * 2);
        enemy->SetDamage(GL_TRUE);
        // glm::vec2 velocity(0.0f, 0.0f);
        float x, y;
        x = enemy->GetPositionX() - YellowDevilCar->GetPositionX();
        y = enemy->GetPositionX() - YellowDevilCar->GetPositionY();
        KnockbackVelocity.x = sqrt((enemy->GetBulletVelocity() * enemy->GetBulletVelocity() * x * x * 4) / (x * x + y * y));
        KnockbackVelocity.y = abs(y / x * KnockbackVelocity.x);
        if (enemy->GetPositionX() > YellowDevilCar->GetPositionX())
          KnockbackVelocity.x = -KnockbackVelocity.x;
        if (enemy->GetPositionY() > YellowDevilCar->GetPositionY())
          KnockbackVelocity.y = -KnockbackVelocity.y;
        enemy->SetKnockback(GL_TRUE);
        YellowDevilCar->SetKnockback(GL_TRUE);
        YellowDevilCar->SetKnockbackVelocity(KnockbackVelocity);

        if (enemy->GetHitPoints() <= 0)
        {
          enemy->SetDestroyed(GL_TRUE);
          enemy->SetKnockback(GL_FALSE);
          this->SpawnBooms(*enemy, dt);
        }
        if (YellowDevilCar->GetHitPoints() <= 0)
        {
          YellowDevilCar->SetDestroyed(GL_TRUE);
        }
        YellowDevilCar->SetInvincible(GL_TRUE);
        YellowDevilCar->SetTimeInvincible(this->Time + 1);
      }
  }

  for (PowerUp &powerUp : this->PowerUps)
  {
    if (!powerUp.GetDestroyed())
    {
      if (powerUp.GetPositionY() >= this->Height)
        powerUp.SetDestroyed(GL_TRUE);
      if (CheckCollision(*YellowDevilCar, powerUp))
      {
        ActivatePowerUp(powerUp);
        powerUp.SetDestroyed(GL_TRUE);
        powerUp.SetActivated(GL_TRUE);
        SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
      }
    }
  }

  for (Bullet &bullet : this->Bullets)
  {
    if (!bullet.GetDestroyed())
    {
      if (bullet.GetPositionY() >= this->Height || bullet.GetPositionY() <= 0)
        bullet.SetDestroyed(GL_TRUE);
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
  bool collisionX = one.GetPositionX() + one.GetSizeX() >= two.GetPositionX() &&
    two.GetPositionX() + two.GetSizeX() >= one.GetPositionX();

  bool collisionY = one.GetPositionY() + one.GetSizeY() >= two.GetPositionY() &&
    two.GetPositionY() + two.GetSizeY() >= one.GetPositionY();
  return collisionX && collisionY;
}
// GLboolean CheckCollision(GameObject &one, GameObject &two)
// {
//   bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
//     two.Position.x + two.Size.x >= one.Position.x;
//
//   bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
//     two.Position.y + two.Size.y >= one.Position.y;
//   return collisionX && collisionY;
// }

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
    if (powerUp.GetActivated())
      if (powerUp.GetType() == type)
        return GL_TRUE;
  }
  return GL_FALSE;
}
