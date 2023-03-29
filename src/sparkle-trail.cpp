// Bryn Mawr College, alinen, 2020
//

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
  float startTime;
  float lifetime;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
    _width = _height = 500;
    // _radius = std::min(_width, _height) * 0.75;
    _radius = 20;
    // eyePos = vec3(0, 0, std::min(_width, _height));
    srand(elapsedTime());
  }

  void setup() {
    setWindowSize(_width, _height);
    createSparkleTrail();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
    renderer.loadTexture("star", "../textures/star4.png", 0);
  }

void createSparkleTrail()
  {
    for (int i = 0; i < _numParticles; i++) {
      Particle particle;
      float time = elapsedTime();
      particle.pos = vec3(_radius * cos(time), _radius * sin(time), 0);
      vec3 nextPos = vec3(_radius * cos(time + dt()), _radius * sin(time + dt()), 0);
      if (i == 0) {
        particle.vel = nextPos - particle.pos;
        particle.color = vec4(1, 1, 1, 1);
      } else {
        particle.vel = particle.pos - nextPos;
        particle.color = vec4(1, 1, 1, (rand() % 11) / 10.0f);
      }
      particle.size = 2.0;
      particle.rot = 0.0;
      particle.startTime = time;
      mParticles.push_back(particle);
    }
  }

  void updateSparkleTrail()
  {
    for (int i = 1; i < mParticles.size(); i++) {
      Particle particle = mParticles[i];
      if (particle.color.w > 0) {
        particle.pos += particle.vel;
        particle.color -= vec4((rand() % 6)/255.0f, (rand() % 6)/255.0f, (rand() % 5)/255.0f, 0.001);
        particle.size += 0.001;
        particle.rot += 0.005;
      } else {
        particle.pos = mParticles[0].pos;
        particle.vel = -mParticles[0].vel;
        particle.color = vec4(1, 1, 1, (rand() % 11) / 10.0f);
        particle.size = 2.0;
        particle.rot = 0.0;
      }
      mParticles[i] = particle;
    }

    int frame = int(elapsedTime() / dt());
    if (frame != prevFrame) {
      prevFrame = frame;
      for (int i = 0; i < 2; i++) {
        cout << "pos " << mParticles[i].pos.x << " " << mParticles[i].pos.y << " | vel " << mParticles[i].vel.x << " " << mParticles[i].vel.y << " | alpha " << mParticles[i].color.w << endl;
      }
      cout << endl;
    }

    mParticles[0].pos += mParticles[0].vel;
    float time = elapsedTime();
    vec3 nextPos = vec3(_radius * cos(time + dt()), _radius * sin(time + dt()), 0);
    mParticles[0].vel = nextPos - mParticles[0].pos;
  }

  void drawSparkleTrail()
  {
    renderer.texture("image", "star");
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

  vec3 eyePos = vec3(0, 0, 40);
  // vec3 eyePos;
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  std::vector<Particle> mParticles;
  int _numParticles = 100;
  int _width;
  int _height;
  int _radius;
  int prevFrame = 0;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
