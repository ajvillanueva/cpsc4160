#include "hud.h"
#include "ioMod.h"

Hud::Hud() : rc(RenderContext::getInstance() ), renderer(rc->getRenderer()) {}

void  Hud::drawHud(){

SDL_Rect r;
r.x = 10;
r.y = 10;
r.w = 195;
r.h = 130;

// First set the blend mode so that alpha blending will work;
// the default blend mode is SDL_BLENDMODE_NONE!
SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND);
SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

// Render rect
SDL_RenderFillRect( renderer, &r );

SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
SDL_RenderDrawRect( renderer, &r );
 //Render the rect to the screen
//SDL_RenderPresent(renderer);

}
