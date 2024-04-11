#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "scene_victory.h"


// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern int game_main_Score = 0;
extern int high_score = 0;
bool game_over = false;
bool game_win = false;

/* Internal variables*/
extern ALLEGRO_TIMER* power_up_timer;
extern ALLEGRO_TIMER* poison_up_timer;
static const int power_up_duration = 10;
static const int poison_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	game_over = false;
	game_win = false;
	game_main_Score = 0;
	// create map
	basic_map = create_map(NULL);
	// [TODO]
	// Create map from .txt file and design your own map !!
	// basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// Allocate dynamic memory for ghosts array.
	ghosts = (Ghost*)malloc(sizeof(Ghost) * GHOST_NUM);
	if (!ghosts) {
		game_log("We haven't create any ghosts!\n");
	}
	else {
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	poison_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	return ;
	if (!poison_up_timer)
		game_abort("Error on create timer\n");
	return;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		basic_map->beansCount--;
		game_main_Score += 10;
		break;
	case 'P':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score += 100;
		if (al_get_timer_started(power_up_timer)) {
			al_set_timer_count(power_up_timer, 0);
		}
		else {
			al_start_timer(power_up_timer);
		}
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);
		}
		break;
	case 'S':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score -= 100;
		pman->speed = 1;
		if (al_get_timer_started(poison_up_timer)) {
			al_set_timer_count(poison_up_timer, 0);
		}
		else {
			al_start_timer(poison_up_timer);
		}
		break;
	case 'G':
		pacman_eatItem(pman, basic_map->map[Grid_y][Grid_x]);
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score += 100;
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);
			ghost_collided(ghosts[i]);
			ghost_die();
		}
	default:
		break;
	}
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;

		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		
		if (!cheat_mode && RecAreaOverlap(getDrawArea(pman->objData, GAME_TICK_CD), getDrawArea(ghosts[i]->objData, GAME_TICK_CD))) {
			if (ghosts[i]->status != FLEE) {
				game_log("collide with ghost\n");
				al_rest(0.6);
				pacman_die();
				game_over = true;
				al_start_timer(pman->death_anim_counter);
				if (game_main_Score > high_score) {
					high_score = game_main_Score;
				}
				break;
			}
			else {
				ghost_collided(ghosts[i]);
				ghost_die();
				break;
			}
		}
		if (basic_map->beansCount <= 300){
			game_win = true;
		}
		if (al_get_timer_count(power_up_timer) > 10) {
			al_stop_timer(power_up_timer);
			al_set_timer_count(power_up_timer, 0);
			for (int i = 0; i < GHOST_NUM; i++) {
				ghost_toggle_FLEE(ghosts[i], false);
			}
		}
		if (al_get_timer_count(poison_up_timer) > 10) {
			al_stop_timer(poison_up_timer);
			al_set_timer_count(poison_up_timer, 0);
		}
		//win condition
	}
}

static void update(void) {

	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...)c;
		*/
		al_start_timer(pman->death_anim_counter);
		if (al_get_timer_count(pman->death_anim_counter) > 50) { 
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_text(menuFont, al_map_rgb(220, 30, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
			char str1[10];
			sprintf(str1, "%d", (int)(high_score));
			al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 125, 500, ALLEGRO_ALIGN_CENTER, str1);
			al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 25, 500, ALLEGRO_ALIGN_CENTER, "Highest score:");
			char str2[10];
			sprintf(str2, "%d", (int)(game_main_Score));
			al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 125, 550, ALLEGRO_ALIGN_CENTER, str2);
			al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 25, 550, ALLEGRO_ALIGN_CENTER, "Current score:");

			al_flip_display();
			al_rest(2.0);
			game_change_scene(scene_menu_create());
		}
		return;
	}
	else if (game_win) {
		al_rest(0.6);
		pacman_win();
		if (game_main_Score > high_score) {
			high_score = game_main_Score;
		}
		game_change_scene(scene_victory_create());
		game_win = false;
	}
	if (al_get_timer_count(power_up_timer) > 10) {
		al_start_timer(power_up_timer);
		al_set_timer_count(power_up_timer, 0);
		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], false);
		}
	}
	if (al_get_timer_count(poison_up_timer) > 10) {
		al_start_timer(poison_up_timer);
		al_set_timer_count(poison_up_timer, 0);
		pman->speed = 2;
	}
	
	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(30, 0, 0));
	static int cnt = 0;
	game_log("frames: %d\n", cnt++);
	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	char score[9];
	sprintf_s(score, 9, "%d", game_main_Score);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), 730, 700, ALLEGRO_ALIGN_CENTER, score);

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	pacman_destory(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_destory(ghosts[i]);
	}
	free(pman);
	free(ghosts);
	al_destroy_timer(power_up_timer);
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
	case ALLEGRO_KEY_W:
		pacman_NextMove(pman, UP);
		break;
	case ALLEGRO_KEY_A:
		pacman_NextMove(pman, LEFT);
		break;
	case ALLEGRO_KEY_S:
		pacman_NextMove(pman, DOWN);
		break;
	case ALLEGRO_KEY_D:
		pacman_NextMove(pman, RIGHT);
		break;
	case ALLEGRO_KEY_C:
		cheat_mode = !cheat_mode;
		if (cheat_mode)
			printf("cheat mode on\n");
		else
			printf("cheat mode off\n");
		break;
	case ALLEGRO_KEY_G:
		debug_mode = !debug_mode;
		break;
	case ALLEGRO_KEY_F:
		basic_map->beansCount=0;
	default:
		break;
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
