/**
 * @file snow.cpp
 * @author Keith Mburu
 * @date 2023-04-02
 * @brief Implements snowflake particle system
 */

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include "agl/window.h"
#include "unistd.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float size;
  float rot;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
    _width = _height = 500;
    background(vec3(50 / 255.0f, 120 / 255.0f, 180 / 255.0f));
    srand(elapsedTime());
  }

  void setup() {
    setWindowSize(_width, _height);
    createSnowflakes();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
    renderer.loadTexture("snowflake", "../textures/snowflake.jpg", 0);
  }

void createSnowflakes()
  {
    for (int i = 0; i < _numParticles; i++) {
      Particle particle;
      particle.size = rand() % 7;
      particle.pos = vec3((rand() % 50) - 25, 35 + (rand() % 100), rand() % 5);
      float xVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
      float yVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
      particle.vel = vec3(0 + xVelJitter, 5 + yVelJitter, 0);
      float cJitter = (rand() % 50) / 255.0f;
      float aJitter = (rand() % 3) / 10.0f;
      particle.color = vec4(0.8 - cJitter, 0.8 - cJitter, 0.8 - cJitter, 0.8 - aJitter);
      particle.rot = 0.0;
      mParticles.push_back(particle);
    }
  }

  void updateSnowflakes()
  {
    for (int i = 0; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.pos.y >= -35) {
        particle.pos -= particle.vel * dt();
      } else {
        particle.size = rand() % 7;
        particle.pos = vec3((rand() % 50) - 25, 35 + (rand() % 100), rand() % 5);
        float xVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
        float yVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
        particle.vel = vec3(0 + xVelJitter, 5 + yVelJitter, 0);
        float cJitter = (rand() % 50) / 255.0f;
        float aJitter = (rand() % 3) / 10.0f;
        particle.color = vec4(0.8 - cJitter, 0.8 - cJitter, 0.8 - cJitter, 0.8 - aJitter);
        particle.rot = 0.0;
      }
      mParticles[i] = particle;
    }
  }

  void drawSnowflakes()
  {
    vec3 cameraPos = renderer.cameraPosition();

    // sort
    for (int i = 1; i < mParticles.size(); i++)
    {
      Particle particle1 = mParticles[i];
      Particle particle2 = mParticles[i - 1];
      float dSqr1 = length2(particle1.pos - cameraPos);
      float dSqr2 = length2(particle2.pos - cameraPos);
      if (dSqr2 < dSqr1)
      {
        mParticles[i] = particle2;
        mParticles[i - 1] = particle1;
      }
    }

    // draw
    renderer.texture("image", "snowflake");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z -= dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    updateSnowflakes();
    drawSnowflakes();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 45);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  std::vector<Particle> mParticles;
  int _numParticles = 100;
  int _width;
  int _height;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
