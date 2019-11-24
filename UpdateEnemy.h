void UpdateEnemy(Enemy &enemy, GLfloat dt, GLfloat WidthLeft, GLfloat WidthRight, GLuint Width, GLuint Height, GLfloat Time)
{
  switch(enemy.Type)
  {
    case 1:
      enemy.Position += enemy.Velocity * dt;
      break;
    case 2:
      enemy.Position.y += enemy.Velocity.y * 2 * dt;
      if (enemy.Position.x >= (WidthLeft) && enemy.Position.x <= (WidthRight - enemy.Size.x * 2) && enemy.Position.y > 0)
      {
        enemy.Position.x += enemy.Velocity.y * dt;
      }
      break;
    case 3:
      enemy.Position.y += enemy.Velocity.y * 2 * dt;
      if (enemy.Position.x > (WidthLeft + enemy.Size.x) && enemy.Position.x < (WidthRight - enemy.Size.x) && enemy.Position.y > 0)
      {
        enemy.Position.x -= enemy.Velocity.y * dt;
      }
      break;
    case 4:
      enemy.Position.y += enemy.Velocity.y * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x) && enemy.Position.y > 0)
      {
        // enemy.Position.x += sin(Time) * 150 * dt;
        enemy.Position.x += enemy.Velocity.x;
        if (enemy.Position.x <= Width / 2 - enemy.Size.x)
        {
          enemy.Velocity.x += 50 * dt;
        }
        if (enemy.Position.x > Width / 2)
        {
          enemy.Velocity.x -= 50 * dt;
        }
        if (enemy.Position.x < (WidthLeft + enemy.Size.x))
          enemy.Position.x = WidthLeft + enemy.Size.x;
        if (enemy.Position.x > (WidthRight - enemy.Size.x))
          enemy.Position.x = WidthRight - enemy.Size.x;
      }
      break;
    case 5:
      enemy.Position.y += enemy.Velocity.y * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x) && enemy.Position.y > 0)
      {
        enemy.Position.x += sin(Time) * 150 * dt;
        if (enemy.Position.x < (WidthLeft + enemy.Size.x))
          enemy.Position.x = WidthLeft + enemy.Size.x;
        if (enemy.Position.x > (WidthRight - enemy.Size.x))
          enemy.Position.x = WidthRight - enemy.Size.x;
      }
      break;
    case 6:
      if (enemy.Position.y > 0 && enemy.Position.y < Height / 4)
      {
        enemy.Velocity.y -= 20 * dt;
      }
      if ((enemy.Position.y >= Height / 4) && (enemy.Position.x < WidthRight - enemy.Size.x * 2))
      {
        enemy.Velocity.x += 80 * dt;
      }
      if (enemy.Position.x < WidthRight - enemy.Size.x && enemy.Position.x > WidthRight - enemy.Size.x * 2)
      {
        enemy.Velocity.x -= 160 * dt;
        enemy.Velocity.y += 50 * dt;
      }
      enemy.Position.y += enemy.Velocity.y * 1.5 * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x * 2) && enemy.Position.y > 0)
        enemy.Position.x += enemy.Velocity.x * dt;
      break;
    case 7:
      if (enemy.Position.y > 0 && enemy.Position.y < Height / 4)
      {
        enemy.Velocity.y -= 20 * dt;
      }
      if ((enemy.Position.y >= Height / 4) && (enemy.Position.x > WidthLeft + enemy.Size.x))
      {
        enemy.Velocity.x -= 80 * dt;
      }
      if (enemy.Position.x < WidthLeft + enemy.Size.x && enemy.Position.x > WidthLeft)
      {
        enemy.Velocity.x += 160 * dt;
        enemy.Velocity.y += 50 * dt;
      }
      enemy.Position.y += enemy.Velocity.y * 1.5 * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x) && enemy.Position.y > 0)
        enemy.Position.x += enemy.Velocity.x * dt;
      break;
    case 8:
      if (enemy.Position.y > Height - Height / 2.5 && enemy.Position.y < Height - Height / 6 - enemy.Size.y && enemy.Position.x > WidthLeft - enemy.Size.x * 2)
      {
        enemy.Velocity.y -= 100 * dt;
        enemy.Velocity.x -= 80 * dt;
      }
      if ((enemy.Position.y >= Height - Height / 6 - enemy.Size.y))
      {
        enemy.Velocity.y = 0;
        enemy.Velocity.x -= 80 * dt;
      }
      if (enemy.Position.x < WidthLeft + enemy.Size.x * 2)
      {
        enemy.Velocity.x += 300 * dt;
        enemy.Velocity.y -= 560 * dt;
      }

        enemy.Position.y += enemy.Velocity.y * 1.7 * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x) && enemy.Position.y > 0)
        enemy.Position.x += enemy.Velocity.x * dt;
      break;
    case 9:
      if (enemy.Position.y > Height - Height / 2.5 && enemy.Position.y < Height - Height / 6 - enemy.Size.y && enemy.Position.x > WidthRight - enemy.Size.x * 2)
      {
        enemy.Velocity.y -= 100 * dt;
        enemy.Velocity.x += 80 * dt;
      }
      if ((enemy.Position.y >= Height - Height / 6 - enemy.Size.y))
      {
        enemy.Velocity.y = 0;
        enemy.Velocity.x += 80 * dt;
      }
      if (enemy.Position.x < WidthRight - enemy.Size.x && enemy.Position.x > WidthRight - enemy.Size.x * 2)
      {
        enemy.Velocity.x -= 300 * dt;
        enemy.Velocity.y -= 560 * dt;
      }
        enemy.Position.y += enemy.Velocity.y * 1.7 * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x * 2) && enemy.Position.y > 0)
        enemy.Position.x += enemy.Velocity.x * dt;
      break;
    case 10:
      if (enemy.Position.y > Height - Height / 2.5 && enemy.Position.y < Height - Height / 6 - enemy.Size.y && enemy.Position.x > WidthLeft - enemy.Size.x * 2)
      {
        enemy.Velocity.y -= 100 * dt;
        enemy.Velocity.x -= 80 * dt;
      }
      if ((enemy.Position.y >= Height - Height / 6 - enemy.Size.y * 2))
      {
        enemy.Velocity.y = 0;
        enemy.Velocity.x -= 80 * dt;
      }
      if (enemy.Position.x < WidthLeft + enemy.Size.x * 2)
      {
        enemy.Velocity.x += 300 * dt;
        enemy.Velocity.y -= 560 * dt;
      }
        enemy.Position.y += enemy.Velocity.y * 1.7 * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x) && enemy.Position.y > 0)
        enemy.Position.x += enemy.Velocity.x * dt;
      break;
    case 11:
      if (enemy.Position.y > Height - Height / 2.5 && enemy.Position.y < Height - Height / 6 - enemy.Size.y && enemy.Position.x > WidthRight - enemy.Size.x * 2)
      {
        enemy.Velocity.y -= 100 * dt;
        enemy.Velocity.x += 80 * dt;
      }
      if ((enemy.Position.y >= Height - Height / 6 - enemy.Size.y * 2))
      {
        enemy.Velocity.y = 0;
        enemy.Velocity.x += 80 * dt;
      }
      if (enemy.Position.x < WidthRight - enemy.Size.x && enemy.Position.x > WidthRight - enemy.Size.x * 2)
      {
        enemy.Velocity.x -= 300 * dt;
        enemy.Velocity.y -= 560 * dt;
      }
        enemy.Position.y += enemy.Velocity.y * 1.7 * dt;
      if (enemy.Position.x >= (WidthLeft + enemy.Size.x) && enemy.Position.x <= (WidthRight - enemy.Size.x * 2) && enemy.Position.y > 0)
        enemy.Position.x += enemy.Velocity.x * dt;
      break;
    default:
      enemy.Position += enemy.Velocity * dt;
    break;
  }
}
