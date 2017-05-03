#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <string>
#include "frame.h"
//#include "sprite.h"
#include "drawable.h"
#include "shootingSprite.h"

class player : public Sprite {

public:
  player(const std::string& name);
  player(const player& p);
  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  virtual bool collidedWith(const Drawable*) const;

  BulletPool getBullets(){return bullets;};
  void stop();
  void right();
  void left();
  void up();
  void down();
  void reset() {bullets.reset();}

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Sprite*) const;
  Vector2f initialVelocity;
  std::string bulletName;
  BulletPool bullets;
  float minSpeed;
  player& operator=(const player&);
};

#endif
