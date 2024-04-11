#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
#include "scene_settings.h"

/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset 
static const int draw_region = 30;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
static ALLEGRO_SAMPLE_ID PACMAN_POWER_ID;
static ALLEGRO_SAMPLE_ID GHOST_ID;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are 
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_POWER;
extern ALLEGRO_SAMPLE* GHOST_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_WIN_SOUND;
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern bool game_win;
extern float effect_volume;
extern int curskin;

/* Declare static function */
static bool pacman_movable(Pacman* pacman, Map* M, Directions targetDirec) {
	// Determine if the current direction is movable.
	switch (targetDirec)
	{
	case UP:
		if (is_wall_block(M, pacman->objData.Coord.x, pacman->objData.Coord.y - 1) || is_room_block(M, pacman->objData.Coord.x, pacman->objData.Coord.y - 1)){
			return false;
		}
		break;
	case DOWN:
		if (is_wall_block(M, pacman->objData.Coord.x, pacman->objData.Coord.y + 1) || is_room_block(M, pacman->objData.Coord.x, pacman->objData.Coord.y + 1)) {
			return false;
		}
		break;
	case LEFT:
		if (is_wall_block(M, pacman->objData.Coord.x - 1, pacman->objData.Coord.y) || is_room_block(M, pacman->objData.Coord.x - 1, pacman->objData.Coord.y)) {
			return false;
		}
		break;
	case RIGHT:
		if (is_wall_block(M, pacman->objData.Coord.x + 1, pacman->objData.Coord.y) || is_room_block(M, pacman->objData.Coord.x + 1, pacman->objData.Coord.y)) {
			return false;
		}
		break;
	default:
		return false;
	}
	return true;
}

Pacman* pacman_create() {

	// Allocate dynamic memory for pman pointer;
	Pacman* pman = (Pacman*)malloc(sizeof(Pacman));
	if (!pman)
		return NULL;
	
	pman->objData.Coord.x = 24;
	pman->objData.Coord.y = 24;
	pman->objData.Size.x = block_width;
	pman->objData.Size.y = block_height;

	pman->objData.preMove = NONE;
	pman->objData.nextTryMove = NONE;
	pman->speed = basic_speed;

	pman->death_anim_counter = al_create_timer(1.0f / 16);
	pman->powerUp = false;
	/* load sprites */
	switch (curskin) {
	case 0:
		pman->move_sprite = load_bitmap("Assets/pacman_move.png");
		pman->die_sprite = load_bitmap("Assets/pacman_die.png");
		break;
	case 1:
		pman->move_sprite = load_bitmap("Assets/puppet_move.png");
		pman->die_sprite = load_bitmap("Assets/pacman_die.png");
		break;
	default:
		pman->move_sprite = load_bitmap("Assets/pacman_move.png");
		pman->die_sprite = load_bitmap("Assets/pacman_die.png");
		break;
	}
	return pman;

}

void pacman_destory(Pacman* pman) {
		al_destroy_bitmap(pman->move_sprite);
		al_destroy_bitmap(pman->die_sprite);
		al_destroy_timer(pman->death_anim_counter);
		//free(pman);
}


void pacman_draw(Pacman* pman) {
	/*
		[HW-TODO ]
		Draw Pacman and animations
		hint: use pman->objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
	*/
	RecArea drawArea = getDrawArea(pman->objData, GAME_TICK_CD);

	int offset = 0;
	if (game_over) {
		/*
			hint: instead of using pman->objData.moveCD, use Pacman's death_anim_counter to create animation
		*/
		int time = al_get_timer_count(pman->death_anim_counter);
		al_draw_scaled_bitmap(pman->die_sprite, 16 * time, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
	}
	else {
		switch(pman->objData.facing){
			case RIGHT:
				if (pman->objData.moveCD % 64 <= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 0, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				else if (pman->objData.moveCD % 64 >= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 16, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				break;
			case UP:
				if (pman->objData.moveCD % 64 <= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 80, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				else if (pman->objData.moveCD % 64 >= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 64, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				break;
			case DOWN:
				if (pman->objData.moveCD % 64 >= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 112, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				else if (pman->objData.moveCD % 64 <= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 96, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				break;
			case LEFT:
				if (pman->objData.moveCD % 64 <= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 32, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				else if (pman->objData.moveCD % 64 >= 32) {
					al_draw_scaled_bitmap(pman->move_sprite, 48, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
				}
				break;

		default:
			al_draw_scaled_bitmap(pman->move_sprite, 0, 0, 16, 16, drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y, draw_region, draw_region, 0);
			break;
		}
	}
}
void pacman_move(Pacman* pacman, Map* M) {
	if (!movetime(pacman->speed))
		return;
	if (game_over)
		return;

	int probe_x = pacman->objData.Coord.x, probe_y = pacman->objData.Coord.y;
	if (pacman_movable(pacman, M, pacman->objData.nextTryMove)) 
		pacman->objData.preMove = pacman->objData.nextTryMove;
	else if (!pacman_movable(pacman, M, pacman->objData.preMove)) 
		return;

	switch (pacman->objData.preMove)
	{
	case UP:
		pacman->objData.Coord.y -= 1;
		pacman->objData.preMove = UP;
		break;
	case DOWN:
		pacman->objData.Coord.y += 1;
		pacman->objData.preMove = DOWN;
		break;
	case LEFT:
		pacman->objData.Coord.x -= 1;
		pacman->objData.preMove = LEFT;
		break;
	case RIGHT:
		pacman->objData.Coord.x += 1;
		pacman->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	pacman->objData.facing = pacman->objData.preMove;
	pacman->objData.moveCD = GAME_TICK_CD;
}
void pacman_eatItem(Pacman* pacman, const char Item) {
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		break;
	case 'P':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_POWER_ID = play_audio(PACMAN_POWER, effect_volume);
		break;
	default:
		break;
	}
}

void pacman_NextMove(Pacman* pacman, Directions next) {
	pacman->objData.nextTryMove = next;
}

void pacman_die() {
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}

void pacman_win() {
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_WIN_SOUND, effect_volume);
}

void ghost_die() {
	stop_bgm(PACMAN_POWER_ID);
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(GHOST_SOUND, effect_volume);
}





