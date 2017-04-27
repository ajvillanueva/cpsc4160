#include <iostream>
#include "sprite.h"
#include "player.h"

class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const player* p);
  void draw() const;
  void update(Uint32 ticks);
  void goLeft();
  void goRight();
  void goUp();
  void goDown();

private:
  enum MODE {NORMAL, FIGHT};

  const Sprite & enemy;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
};
