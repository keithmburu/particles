/**
 * @file rain.cpp
 * @author Keith Mburu
 * @date 2023-04-52
 * @brief Implements rain particle system
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
  glm::vec3 p1;
  glm::vec3 p2;
  glm::vec3 vel;
  glm::vec4 color;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
    _width = _height = 500;
    srand(elapsedTime());
  }

  void setup() {
    setWindowSize(_width, _height);
    createRaindrops();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
    renderer.loadTexture("cloud", "../textures/ParticleCloudWhite.png", 0);
  }

void createRaindrops()
  {
    for (int i = 0; i < _numParticles; i++) {
      Particle particle;
      particle.p1 = vec3((rand() % 50) - 25, 35 + (rand() % 100), rand() % 5);
      float yLength = std::max(2, rand() % 4);
      float xLength = yLength / 2;
      particle.p2 = particle.p1 + vec3(-xLength, -yLength, 0);
      float xVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
      float yVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
      particle.vel = vec3(0 + xVelJitter, 5 + yVelJitter, 0);
      particle.color = vec4(0, 0, 0, 0);
      mParticles.push_back(particle);
    }
  }

  void updateRaindrops()
  {
    for (int i = 0; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.p1.y >= -35) {
        particle.p1 -= particle.vel * dt();
        particle.p2 -= particle.vel * dt();
        if (particle.color.w == 0 && particle.p1.y <= 15 + (rand() % 5)) {
            float cJitter = (rand() % 100) / 255.0f;
            float aJitter = (rand() % 3) / 10.0f;
            particle.color = vec4(1 - cJitter, 1 - cJitter, 1 - cJitter, 1 - aJitter);
        }
      } else {
        particle.p1 = vec3((rand() % 50) - 25, 35 + (rand() % 100), rand() % 5);
        float yLength = std::max(2, rand() % 4);
        float xLength = yLength / 2;
        particle.p2 = particle.p1 + vec3(-xLength, -yLength, 0);
        float xVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
        float yVelJitter = pow(-1, rand()) * ((rand() % 5) / 10.0f);
        particle.vel = vec3(0 + xVelJitter, 5 + yVelJitter, 0);
        particle.color = vec4(0, 0, 0, 0);
      }
      mParticles[i] = particle;
    }
  }

  void drawRaindrops()
  {
    vec3 cameraPos = renderer.cameraPosition();

    // sort
    for (int i = 1; i < mParticles.size(); i++)
    {
      Particle particle1 = mParticles[i];
      Particle particle2 = mParticles[i - 1];
      float dSqr1 = length2(particle1.p1 - cameraPos);
      float dSqr2 = length2(particle2.p1 - cameraPos);
      if (dSqr2 < dSqr1)
      {
        mParticles[i] = particle2;
        mParticles[i - 1] = particle1;
      }
    }

    // draw
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.line(particle.p1, particle.p2, particle.color, particle.color);
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
    renderer.beginShader("lines");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    
    updateRaindrops();
    drawRaindrops();
    renderer.beginShader("sprite");
    renderer.texture("image", "cloud");
    for (int i = 0; i <= 10; i++)
    {
      vec3 pos = vec3((i * 5) - 25, 17.5, 10);
      renderer.sprite(pos, vec4(1.0), 15.0);
    }
    renderer.endShader();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 45);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  std::vector<Particle> mParticles;
  int _numParticles = 750;
  int _width;
  int _height;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
