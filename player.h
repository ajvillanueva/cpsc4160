#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <string>
#include "frame.h"
#include "sprite.h"
#include "drawable.h"

class player : public Sprite {

public:
  player(const std::string& name);
  player(const player& p);

  virtual void update(Uint32 ticks);
  void stop();
  void right();
  void left();
  void up();
  void down();

private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Sprite*) const;
  Vector2f initialVelocity;
};

#endif
