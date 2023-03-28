#version 400

layout (location = 0) in vec3 vPosition;

uniform vec3 CameraPos;
uniform float Size;
uniform float Rot;
uniform vec3 Offset;
uniform vec4 Color;
uniform mat4 MVP;

out vec4 color;
out vec2 uv;

void main()
{
  color = Color;
  uv = vPosition.xy;

  vec3 n = normalize(CameraPos - vPosition);
  float thetaY = atan(n.z - vPosition.z, n.x - vPosition.x);

  vec3 x = vec3(cos(thetaY), 0, sin(thetaY));
  vec3 y = vec3(0, 1, 0);
  vec3 z = vec3(-sin(thetaY), 0, cos(thetaY));
  mat3 R = mat3(x, y, z);

  vec3 eyePos = R * Size * (vPosition - vec3(0.5, 0.5, 0.0)) + Offset;
  gl_Position = MVP * vec4(eyePos, 1.0); 
}
