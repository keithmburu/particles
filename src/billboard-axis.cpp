// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
    _radius = 2;
    _azimuth = _elevation = 0;
    _leftClick = false;
  }

  void setup() {
    setWindowSize(500, 500);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");
      renderer.loadShader("billboard-axis",
      "../shaders/billboard-axis.vs",
      "../shaders/billboard-axis.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // TODO: Use the width and the height of the image to scale the billboard

    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
      if (_leftClick) {
        // orbit
        float ONE_DEG = 0.017; // one degree in radians
        _elevation += dy * (M_PI / 180);
        if (_elevation > (M_PI / 2) - ONE_DEG) {
           _elevation = (M_PI / 2) - ONE_DEG;
        } else if (_elevation < -((M_PI / 2) - ONE_DEG)) {
           _elevation = -((M_PI / 2) - ONE_DEG);
        }
        _azimuth -= dx * (M_PI / 180);
        if (_azimuth > 2 * M_PI) {
           _azimuth = 0;
        } else if (_azimuth < 0) {
           _azimuth = 2 * M_PI;
        }
      }
   }

   void mouseDown(int button, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
         _leftClick = true;
      }
   }

   void mouseUp(int button, int mods) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
         _leftClick = false;
      }
   }

   void scroll(float dx, float dy) {
   }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    float camPosX = _radius * sin(_azimuth) * cos(_elevation);
    float camPosY = _radius * sin(_elevation);
    float camPosZ = _radius * cos(_azimuth) * cos(_elevation);
    eyePos = vec3(camPosX, camPosY, camPosZ);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.beginShader("billboard-axis");
    renderer.texture("Image", "tree");
    renderer.push();
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();
    renderer.endShader();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  float _radius;
  float _azimuth;
  float _elevation;
  bool _leftClick;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
