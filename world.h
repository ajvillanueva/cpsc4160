#include <string>
#include "frame.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  // ~World() { } // The frame will be deleted by the FrameFactory
  void update();
  void draw() const;
private:
  Frame* const frame;
  int factor;
  unsigned worldHeight;
  unsigned frameHeight;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
