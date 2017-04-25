#include <iostream>
#include "world.h"
#include "renderContext.h"

World::World(const std::string& name, int fact) :
  frame( RenderContext::getInstance()->getFrame(name) ),
  factor(fact),
    worldHeight( Gamedata::getInstance().getXmlInt("world/height") ),
    frameHeight( frame->getHeight() ),
    viewX(0.0), viewY(0.0),
    view(Viewport::getInstance())
{ }

void World::update() {
  //viewX = static_cast<int>(view.getX() / factor) % frameWidth;
  //viewY = view.getY();
  viewX = view.getX();
  viewY = static_cast<int>(view.getY() / factor) % frameHeight;
}

void World::draw() const {
  frame->draw(0,0,-viewX,-viewY);
  frame->draw(0,0,-viewX,frameHeight-viewY);
}
