#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
//#include "sprite.h"
//#include "shootingSprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"

bool hudIf = true;
player* p = new player("player");
//ShootingSprite* pease = new ShootingSprite("player");
class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  for (auto& it: sprites) delete it;
  delete p;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  back("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  backclose("backclose", Gamedata::getInstance().getXmlInt("backclose/factor") ),
  backfar("backfar", Gamedata::getInstance().getXmlInt("backfar/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(-1),
  makeVideo( false ),
  strategy( new PerPixelCollisionStrategy ),
  collisions(0)
{
  sprites.push_back( new Sprite("boss") );
  sprites.push_back( new Sprite("enemy1") );
  sprites.push_back( new Sprite("enemy2") );
  sprites.push_back( new Sprite("enemy3") );
  sprites.push_back( new Sprite("enemy4") );
  //sprites.push_back(pease);
  //sprites.push_back( new ShootingSprite("player"));
  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  back.draw();
  backfar.draw();
  backclose.draw();
  p->draw();
  for(auto* s : sprites) s->draw();
  Hud hud;
  if (hudIf) {
    hud.drawHud();
    std::stringstream strm;
    std::stringstream strm2;
    std::string ctrl, toggleHud;
    strm << "fps: " << clock.getFps();
    strm2 << "avgfps: " << clock.getAvgFps();
    ctrl = "wasd to move";
    toggleHud = "F1 to toggle HUD";
    SDL_Color color;
    color.r = 0; color.g = 0; color.b = 0; color.a = 0;
    std::string title = Gamedata::getInstance().getXmlStr("title");
    io.writeText(strm.str(), 15, 20, color);
    io.writeText(strm2.str(), 15, 50, color);
    io.writeText(ctrl, 15, 80, color);
    io.writeText(toggleHud, 15, 110, color);
    io.writeText(title, 10, 750);
  }
  //viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  for(auto* s : sprites) s->update(ticks);
  p->update(ticks);
  back.update();
  backfar.update();
  backclose.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  currentSprite = ++currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::checkForCollisions() {
  std::vector<Drawable*>::const_iterator it = sprites.begin();
  Drawable* player = sprites[0];
  ++it;
  while ( it != sprites.end() ) {
    if ( strategy->execute(*player, **it) ) {
      //std::cout << "collision: " << ++collisions << std::endl;
      ++collisions;
    }
    ++it;
  }
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  Viewport::getInstance().setObjectToTrack(p);
  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_S] ) {
          //clock.toggleSloMo();
        }

        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          hudIf = !hudIf;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
    if (keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D]) {
      p->stop();
    }
    if (keystate[SDL_SCANCODE_A]) {
      p->left();
    }
    if (keystate[SDL_SCANCODE_D]) {
      p->right();
    }
    if (keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S]) {
      p->stop();
    }
    if (keystate[SDL_SCANCODE_W]) {
      p->up();
    }
    if (keystate[SDL_SCANCODE_S]) {
      p->down();
    }
    if ( keystate[SDL_SCANCODE_SPACE] ) {
      p->shoot();
  }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
      draw();
      update(ticks);
      checkForCollisions();
    }
  }
}
