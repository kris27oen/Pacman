// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"

ALLEGRO_TIMER* power_up_timer;
ALLEGRO_TIMER* poison_up_timer;
Scene scene_main_create(void);

#endif
