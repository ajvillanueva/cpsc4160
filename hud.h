#include <vector>
#include <SDL.h>
#include <sstream>
#include "renderContext.h"


class Hud {
public:
  Hud ();
  void drawHud();

private:
  const RenderContext* rc;
  SDL_Renderer * const renderer;
};
