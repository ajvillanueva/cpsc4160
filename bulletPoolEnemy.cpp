#include <iostream>
#include <sstream>
#include <cmath>
#include "ioMod.h"
#include "gamedata.h"
#include "bulletPool.h"

CollisionStrategy* getStrategy(const string& name) {
  std::string sName = Gamedata::getInstance().getXmlStr(name+"/strategy");
  if ( sName == "midpoint" ) return new MidPointCollisionStrategy;
  if ( sName == "rectangular" ) return new RectangularCollisionStrategy;
  if ( sName == "perpixel" ) return new PerPixelCollisionStrategy;
  throw std::string("No strategy in getStrategy");
}

BulletPoolEnemy::~BulletPoolEnemy() {
  delete strategy;
}

BulletPoolEnemy::BulletPoolEnemy(const std::string& n) :
  name(n),
  strategy( getStrategy(name) ),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList()
{ }

BulletPoolEnemy::BulletPoolEnemy(const BulletPool& b) :
  name(b.name),
  strategy(b.strategy),
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList),
  freeList(b.freeList)
{ }

bool BulletPoolEnemy::collidedWith(const Drawable* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void BulletPoolEnemy::shoot(const Vector2f& position, const Vector2f& velocity) {
	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    // MAKE IT SO THAT THEY STOP SHOOTING AFTER 10 CONSECUTIVE SHOTS
    if ( freeList.empty() ) {
      Bullet b(name);
      b.setPosition( position );
      b.setVelocity( velocity );
      bulletList.push_back( b );
    }
    else {
      Bullet b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity(velocity);
      b.setPosition(position);
      bulletList.push_back(b);
    }
		timeSinceLastFrame = 0;
	}
}

void BulletPoolEnemy::draw() const {
  std::stringstream stream;
  stream << "Active bullets: " << bulletList.size();
  IOmod::getInstance().
    writeText(stream.str(), 400, 30);
  stream.clear();
  stream.str("");
  stream << "Bullet pool: " << freeList.size();
  IOmod::getInstance().
    writeText(stream.str(), 400, 60);
  for ( Bullet bullet : bulletList ) {
    bullet.draw();
  }
}

void BulletPoolEnemy::update(Uint32 ticks) {
	timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else ++ptr;
  }
}
