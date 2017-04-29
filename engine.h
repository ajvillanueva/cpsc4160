#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"

class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World back;
  World backclose;
  World backfar;
  Viewport& viewport;

  std::vector<Drawable*> sprites;

  int currentSprite;
  bool makeVideo;
  CollisionStrategy* strategy;
  int collisions;
  bool godmode;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&);
  Engine& operator=(const Engine&);
  void checkForCollisions();
};
