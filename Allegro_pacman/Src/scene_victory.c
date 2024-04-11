#define _CRT_SECURE_NO_WARNINGS
// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#include "scene_settings.h"
#include "pacman_obj.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
static ALLEGRO_SAMPLE_ID settingsBGM;
static ALLEGRO_BITMAP* victory = NULL;
static int victoryW;
static int victoryH;
static Button btnMenu;
static Button btnplay;

extern int high_score;
extern int game_main_Score;

static void init() {
	btnMenu = button_create(400, 600, 150, 150, "Assets/back_button.png", "Assets/back_button.png");
	btnplay = button_create(250, 600, 150, 150, "Assets/play_again.png", "Assets/play_again.png");
	victory = load_bitmap("Assets/victory.png");
	victoryW = al_get_bitmap_width(victory);
	victoryH = al_get_bitmap_height(victory);
	stop_bgm(settingsBGM);
	settingsBGM = play_bgm(themeMusic, music_volume);
}


// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void draw(void) {
	al_clear_to_color(al_map_rgb(200, 210, 200));

	const float scale = 1.5;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * victoryW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * victoryH;

	drawButton(btnMenu);
	drawButton(btnplay);
	al_draw_scaled_bitmap(victory, 0, 0,
		victoryW, victoryH,
		offset_w, offset_h-50,
		victoryW * scale, victoryH * scale,
		0
	);
	char str1[10];
	sprintf(str1, "%d", (int)(high_score));
	al_draw_text(menuFont, al_map_rgb(10, 0, 0), SCREEN_W / 2 + 125, 500, ALLEGRO_ALIGN_CENTER, str1);
	al_draw_text(menuFont, al_map_rgb(10, 0, 0), SCREEN_W / 2 - 3, 500, ALLEGRO_ALIGN_CENTER, "Highest score:");
	char str2[10];
	sprintf(str1, "%d", (int)(game_main_Score));
	al_draw_text(menuFont, al_map_rgb(10, 0, 0), SCREEN_W / 2 + 100, 540, ALLEGRO_ALIGN_CENTER, str2);
	al_draw_text(menuFont, al_map_rgb(10, 0, 0), SCREEN_W / 2, 540, ALLEGRO_ALIGN_CENTER, "Current score:");
}
static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnMenu.hovered = buttonHover(btnMenu, mouse_x, mouse_y);
	btnplay.hovered = buttonHover(btnplay, mouse_x, mouse_y);

}
static void on_mouse_down() {
	if (btnMenu.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		game_change_scene(scene_menu_create());
	if (btnplay.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		game_change_scene(scene_main_create());
}
static void destroy() {
	stop_bgm(settingsBGM);
	al_destroy_bitmap(btnMenu.default_img);
	al_destroy_bitmap(btnMenu.hovered_img);

}
static void on_key_down(int keycode) {
	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		game_change_scene(scene_main_create());
		break;
	default:
		break;
	}
}

// The only function that is shared across files.
Scene scene_victory_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "victory";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("victory scene created");
	return scene;
}