/**
 * @file stars.cpp
 * @author Keith Mburu
 * @date 2023-04-02
 * @brief Implements explosion, flame, and fireball animations 
 * using sprite sheets
 */

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct spriteSheet {
    string textureFile;
    int numRows;
    int numCols;
};

class Viewer : public Window {
public:
  Viewer(string animationType, const struct spriteSheet s) {
    this->animationType = animationType;
    numRows = s.numRows;
    numCols = s.numCols;
    textureFile = s.textureFile;
    cout << animationType << " " << textureFile << " " << numRows << " rows " << numCols << " cols" << endl;
  }

  void setup() {
    setWindowSize(500, 500);
    renderer.loadShader("billboard-animated", 
      "../shaders/billboard-animated.vs", 
      "../shaders/billboard-animated.fs");

    renderer.loadTexture(animationType, 
      textureFile, 0);

    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void draw() {
    renderer.beginShader("billboard-animated");
    renderer.texture("image", animationType);

    time = time + dt();
    int frame = int(time * 30) % (numRows * numCols);
    renderer.setUniform("Frame", frame);
    renderer.setUniform("Rows", numRows);
    renderer.setUniform("Cols", numCols);
    bool topToBottom = (animationType == "flame");
    renderer.setUniform("TopToBottom", topToBottom);
    
    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    renderer.sprite(vec3(0.0), vec4(1.0f), 1.0);

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);

  float time = 0.0f;
  int numRows;
  int numCols;
  string animationType;
  string textureFile;
};

int main(int argc, char** argv)
{
  map<string, spriteSheet> spriteSheets;
  spriteSheets["explosion-2"] = {"../textures/explosion2.png", 8, 8};
  spriteSheets["explosion-3"] = {"../textures/explosion3.png", 8, 16};
  spriteSheets["flame"] = {"../textures/ParticleFlamesSheet.png", 4, 8};
  spriteSheets["fireball-1"] = {"../textures/fireball.png", 2, 8};
  spriteSheets["fireball-2"] = {"../textures/ParticleFireballSheet.png", 4, 1};
  for (auto it = spriteSheets.begin(); it != spriteSheets.end(); it++) {
    Viewer viewer(it->first, it->second);
    viewer.run();
  }
  return 0;
}
