// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_POWER = NULL;
ALLEGRO_SAMPLE* GHOST_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_WIN_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/horror_theme.mp3");
	PACMAN_MOVESOUND = load_audio("Assets/Music/eat_effect.mp3");
	GHOST_SOUND = load_audio("Assets/Music/eat_ghost.mp3");
	PACMAN_POWER = load_audio("Assets/Music/power_bean.mp3");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/horror_scream.mp3");
	PACMAN_WIN_SOUND = load_audio("Assets/Music/win_laugh.mp3");
}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_POWER);
	al_destroy_sample(GHOST_SOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_sample(PACMAN_WIN_SOUND);
}