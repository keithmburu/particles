// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float size;
  float rot;
  float startTime;
  float lifetime;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(500, 500);
    createSparkleTrail();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

void createSparkleTrail()
  {
    for (int i = 1; i < _numParticles; i++) {
      renderer.loadTexture("particle", "../textures/particle.png", 0);
      Particle particle;
      // particle.pos = vec3(0.75 * width(), 0.75 * height(), 0);
      int time = elapsedTime();
      particle.pos = vec3(cos(time), sin(time), 0);
      vec3 nextPos = vec3(cos(time + 1), sin(time + 1), 0);
      if (i == 1) {
        particle.vel = nextPos - particle.pos;
      } else {
        particle.vel = particle.pos - nextPos;
      }
      particle.color = vec4(1, 1, 1, 1);
      particle.size = 0.5;
      particle.rot = 0.0;
      particle.startTime = elapsedTime();
      mParticles.push_back(particle);
    }
  }

  void updateSparkleTrail()
  {
    for (int i = 1; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.color.w > 0) {
        particle.pos += particle.vel;
        particle.color -= vec4(0, 0, 0, 0.1);
        particle.size += 0.05;
        particle.rot += 5.0;
      } else {
        particle.pos = mParticles[0].pos;
        particle.vel = -mParticles[0].vel;
        particle.color = vec4(1, 1, 1, 1);
        particle.size = 0.5;
        particle.rot = 0.0;
        particle.startTime = elapsedTime();
      }
      mParticles[i] = particle;
    }

    if (int(elapsedTime()) % 2 == 0) {
      cout << "pos " << mParticles[0].pos.x << " " << mParticles[0].pos.y << " vel " << mParticles[0].vel.x << " " << mParticles[0].vel.y << " alpha " << mParticles[0].color.w << endl;
      cout << "pos " << mParticles[9].pos.x << " " << mParticles[9].pos.y << " vel " << mParticles[9].vel.x << " " << mParticles[9].vel.y << " alpha " << mParticles[9].color.w << endl;
    }

    int time = elapsedTime();
    vec3 nextPos = vec3(cos(time + 1), sin(time + 1), 0);
    mParticles[0].vel = nextPos - mParticles[0].pos;
    mParticles[0].pos += mParticles[0].vel;
  }

  void drawSparkleTrail()
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
    renderer.texture("image", "particle");
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
    eyePos.x += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    updateSparkleTrail();
    drawSparkleTrail();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 10);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  std::vector<Particle> mParticles;
  int _numParticles = 10;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
