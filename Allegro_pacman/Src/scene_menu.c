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

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_BITMAP* decoration = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW ;
static int gameTitleH ;
static int decorationW;
static int decorationH;

// use a clickable button to enter setting scene.

// Declare variable for button
static Button btnSettings;


static void init() {
	// Create button to settings
	btnSettings = button_create(730, 20, 50, 50, "Assets/settings.png", "Assets/settings2.png");

	gameTitle = load_bitmap("Assets/pacman_title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	decoration = load_bitmap("Assets/interface.jpg");
	decorationW = al_get_bitmap_width(decoration);
	decorationH = al_get_bitmap_height(decoration);
	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);

}


static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;
	const float offsetdecor_w = (SCREEN_W >> 1) - 0.5 * scale * decorationW;
	const float offsetdecor_h = (SCREEN_H >> 1) - 0.5 * scale * decorationH;

	//draw title image
	al_draw_scaled_bitmap(
		decoration,
		0, 0,
		decorationW, decorationH,
		-150, 150,
		decorationW * 0.3, decorationH * 0.3,
		0
	);
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w+80, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);
		// Draw button
		drawButton(btnSettings);

}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	// Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
}


// When btnSettings clicked, switch to settings scene
static void on_mouse_down() {
	if (btnSettings.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		game_change_scene(scene_settings_create());
}

static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(gameTitle);
	al_destroy_bitmap(decoration);
	// Destroy button images
	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
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

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Menu";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	// Register on_mouse_down.
	// Register more event callback functions such as keyboard, mouse, ...
	scene.on_mouse_down = &on_mouse_down;
	game_log("Menu scene created");
	return scene;
}