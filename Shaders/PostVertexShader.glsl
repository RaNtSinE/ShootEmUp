#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
  gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
  vec2 texture = vertex.zw;
  if(chaos)
  {
    float strength = 0.3;
    vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
    TexCoords = pos;
    // TexCoords = texture;
  }
  else if(confuse)
  {
    // vec2 bufTexture = vec2(0.0f, 0.0f);
    // bufTexture.x = bufTexture.x / time;
    // bufTexture.y = bufTexture.y / time;
    // if ((bufTexture.x / time > texture.x) && (bufTexture.y / time > texture.y));
    // {
    //   bufTexture.x = texture.x;
    //   bufTexture.y = texture.y;
    // }

    TexCoords = vec2(1.0 - texture.x, 1.0 - texture.y);
  }
  else
  {
    TexCoords = texture;
  }
  if (shake)
  {
    float strength = 0.01;
    gl_Position.x += cos(time * 10) * strength;
    gl_Position.y += cos(time * 15) * strength;
  }
}
