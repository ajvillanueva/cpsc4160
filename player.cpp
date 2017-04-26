#include <cmath>
#include "player.h"
#include "gamedata.h"
#include "renderContext.h"
#include "clock.h"
#include "sprite.h"

player::player( const std::string& name) :
  ShootingSprite(name),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  initialVelocity(getVelocity()) { }

player::player( const player& p) :
  ShootingSprite(p),
  frame(p.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(p.getFrame()->getWidth()),
  frameHeight(p.getFrame()->getHeight()),
  initialVelocity(p.initialVelocity) { }

void player::stop() {
  setVelocityX(0); //slowDown*getVelocityX() );
  setVelocityY(0);
}

void player::right() {
  if ( getX() < worldWidth-frameWidth) {
    setVelocityX(initialVelocity[0]);
  }

  // if ( getX() < worldWidth-frameWidth) {
  //   setVelocityX( std::abs( getVelocityX() + 1 ) );
  // }
}

void player::left() {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
  // if ( getX() > 0) {
  //   setVelocityX(- std::abs( getVelocityX() - 1) );
  // }
}

void player::up() {
  if ( getY() > 0) {
    setVelocityY(-initialVelocity[1]);
  }
  // if ( getY() > 0) {
  //   setVelocityY(- std::abs( getVelocityY() ) );
  // }
}

void player::down() {
  if ( getY() < worldHeight-frameHeight) {
    setVelocityY(initialVelocity[1]);
  }
  // if ( getY() < worldHeight-frameHeight) {
  //   setVelocityY( std::abs( getVelocityY() ) );
  // }
}

void player::update(Uint32 ticks) {

   Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
   setPosition(getPosition() + incr);
   if (getY() < 0) {
     setVelocityY(fabs(getVelocityY()));
   }
   if (getY() > worldHeight-frameHeight) {
     setVelocityY(-fabs(getVelocityY()));
   }
  if (getX() <= 0) {
      setVelocityX( fabs( getVelocityX() ) );
  }
  if (getX() >= worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );;
  }
  stop();
}
