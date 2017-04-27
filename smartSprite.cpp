#include <cmath>
#include "viewport.h"
#include "smartSprite.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name, const player* p) :
 Sprite(name),
 enemy(*p),
 currentMode(NORMAL)
{ }

void SmartSprite::goLeft()  {
  if (getX() > 0) setVelocityX( -std::abs(getVelocityX()) );
}
void SmartSprite::goRight() { setVelocityX( fabs(getVelocityX()) ); }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) ); }

void SmartSprite::draw() const {
  Sprite::draw();
  int x = 10+ getX() - Viewport::getInstance().getX();
  int y = 10+ getY() - Viewport::getInstance().getY();
}

void SmartSprite::update(Uint32 ticks) {
  Sprite::update(ticks);
  float x= getX()+getFrame()->getWidth()/2;
  float y= getY()+getFrame()->getHeight()/2;
  float ex= enemy.getX()+enemy.getFrame()->getWidth()/2;
  float ey= enemy.getY()+enemy.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );
  float safeDistance = 100;

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = FIGHT;
  }
  else if  ( currentMode == FIGHT ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x > ex ) goLeft();
      if ( x < ex ) goRight();
      if ( y > ey ) goUp();
      if ( y < ey ) goDown();
    }
  }
}
