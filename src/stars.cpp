/**
 * @file stars.cpp
 * @author Keith Mburu
 * @date 2023-04-02
 * @brief Implements particle system of stars
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
  glm::vec4 color;
  float size;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
    _width = _height = 500;
    srand(elapsedTime());
  }

  void setup() {
    setWindowSize(_width, _height);
    createStars();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
    renderer.loadTexture("star", "../textures/starGlow.png", 0);
  }

void createStars()
  {
    for (int i = 0; i < _numParticles; i++) {
      Particle particle;
      particle.pos = vec3((rand() % 50) - 25, (rand() % 50) - 25, 0);
      particle.color = vec4(1, 1, 1, 1);
      particle.size = ((rand() % 100) / 100.0f) * 4;
      mParticles.push_back(particle);
    }
  }

  void updateStars()
  {
    for (int i = 0; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.size >= 0.003) {
        particle.size -= 0.003;
      } else {
        particle.pos = vec3((rand() % 50) - 25, (rand() % 50) - 25, 0);
        particle.color = vec4(1, 1, 1, 1);
        particle.size = ((rand() % 100) / 100.0f) * 4;
      }
      mParticles[i] = particle;
    }
  }

  void drawStars()
  {
    renderer.texture("image", "star");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, 0.0);
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
    updateStars();
    drawStars();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 45);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  std::vector<Particle> mParticles;
  int _numParticles = 20;
  int _width;
  int _height;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
