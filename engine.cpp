#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <random>
#include <algorithm>
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "smartSprite.h"
#include "explodingSprite.h"

bool hudIf = true;
player* p = new player("player");
int lives = Gamedata::getInstance().getXmlInt("numberOfLives");
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
  for (auto&& it: sprites) delete it;
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
  collisions(0),
  godmode ( false )
{
  sprites.push_back( new Sprite("boss") );
  sprites.push_back( new Sprite("enemy1") );
  sprites.push_back( new Sprite("enemy2") );
  sprites.push_back( new Sprite("enemy3") );
  sprites.push_back( new Sprite("enemy4") );
  sprites.push_back( new Sprite("enemy1b") );
  sprites.push_back( new Sprite("enemy2b") );
  sprites.push_back( new Sprite("enemy3b") );
  sprites.push_back( new Sprite("enemy4b") );
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
// std::stringstream strm3;
// strm3.clear();
// strm3.str("");
// strm3 << "collision no: " << collisions;
// io.writeText(strm3.str(), 400, 90);
  Hud hud;
  if (hudIf) {
    hud.drawHud();
    std::stringstream strm;
    std::stringstream strm2;
    std::string ctrl, toggleHud, shoot, god, r, god2;
    strm << "Fps: " << clock.getFps();
    strm2 << "Avgfps: " << clock.getAvgFps();
    ctrl = "wasd to move";
    shoot = "Space to shoot";
    toggleHud = "F1 to toggle HUD";
    god = "G for god mode";
    r = "R for reset";
    god2 = "God mode on";
    SDL_Color color;
    color.r = 0; color.g = 0; color.b = 0; color.a = 0;
    std::string title = Gamedata::getInstance().getXmlStr("title");
    io.writeText(strm.str(), 15, 20, color);
    io.writeText(strm2.str(), 15, 50, color);
    io.writeText(ctrl, 15, 80, color);
    io.writeText(shoot, 15, 110, color);
    io.writeText(toggleHud, 15, 140, color);
    io.writeText(god, 15, 170, color);
    io.writeText(r, 15, 200, color);
    if (godmode) {
      io.writeText(god2, 400, 90);
    }
    io.writeText(title, 10, 770);
  }
  //viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::redraw() {
  sprites.push_back( new Sprite("boss") );
  sprites.push_back( new Sprite("enemy1") );
  sprites.push_back( new Sprite("enemy2") );
  sprites.push_back( new Sprite("enemy3") );
  sprites.push_back( new Sprite("enemy4") );
  sprites.push_back( new Sprite("enemy1b") );
  sprites.push_back( new Sprite("enemy2b") );
  sprites.push_back( new Sprite("enemy3b") );
  sprites.push_back( new Sprite("enemy4b") );
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

    std::vector<Drawable*>::iterator it = sprites.begin();
  //  Drawable* player = sprites[0];
  while ( it != sprites.end() ) {
  if (!godmode) {
    //++it;

      if (p->collidedWith(*it)  ) {
        //std::cout << "collision: " << collisions << std::endl;
          //std::cout << "collision: " << collisions << std::endl;
      Drawable* boom =  new ExplodingSprite(*static_cast<Sprite*>(*it));
          delete *it;
          *it = boom;
        ++collisions;
      }
      if(strategy->execute(*p, **it)){
        Drawable* boom =  new ExplodingSprite(*static_cast<Sprite*>(*it));
            delete *it;
            *it = boom;
          //p = boom;
            ++collisions;
      }
    }
   else if(godmode){
      if (p->collidedWith(*it)  ) {
        //std::cout << "collision: " << collisions << std::endl;
  Drawable* boom =  new ExplodingSprite(*static_cast<Sprite*>(*it));
        delete *it;
        *it = boom;
        ++collisions;
        //delete *it;
      }
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
        if (keystate[SDL_SCANCODE_R]) {
          godmode = false;

          p->setX(Gamedata::getInstance().getXmlInt("player/startLoc/x"));
          p->setY(Gamedata::getInstance().getXmlInt("player/startLoc/y"));

          p->reset();

          std::vector<Drawable*>::iterator ptr = sprites.begin();
          while (ptr != sprites.end()) {
            delete (*ptr);
            ptr = sprites.erase(ptr);
          }

          redraw();
          // put enemies back where they were
        }
        if (keystate[SDL_SCANCODE_G]) {
          if (godmode) {
            godmode = false;
          } else {
            godmode = true;
          }
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
