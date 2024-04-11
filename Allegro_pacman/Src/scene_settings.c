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
static Button btnMenu;
static Button btnVoldown;
static Button btnVolup;
static Button btnSFXdown;
static Button btnSFXup;
static Button original;
static Button puppet;
static Button skinputih;

extern int high_score;
extern int curskin = 0;

static void init() {
	btnMenu = button_create(20, 20, 60, 60, "Assets/back_button.png", "Assets/back_button.png");
	btnVoldown = button_create(SCREEN_W / 3.3, SCREEN_H - 710, 60, 45, "Assets/button_vdown.png", "Assets/button_vdown.png");
	btnVolup = button_create(SCREEN_W / 1.6, SCREEN_H - 710, 60, 45, "Assets/button_vup.png", "Assets/button_vup.png");
	btnSFXdown = button_create(SCREEN_W / 3.3, SCREEN_H - 610, 60, 45, "Assets/button_vdown.png", "Assets/button_vdown.png");
	btnSFXup = button_create(SCREEN_W / 1.6, SCREEN_H - 610, 60, 45, "Assets/button_vup.png", "Assets/button_vup.png");
	original = button_create(SCREEN_W / 3.3, SCREEN_H - 500, 100, 100, "Assets/pacman.png", "Assets/pacman.png");
	puppet = button_create(SCREEN_W / 3.3, SCREEN_H - 350, 100, 100, "Assets/puppet.png", "Assets/puppet.png");
	stop_bgm(settingsBGM);
	settingsBGM = play_bgm(themeMusic, music_volume);
}


// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void draw(void) {
	al_clear_to_color(al_map_rgb(85, 85, 85));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 775, ALLEGRO_ALIGN_CENTER, "SETTING");
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 8, SCREEN_H - 700, ALLEGRO_ALIGN_CENTER, "VOLUME");
	//al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 3, SCREEN_H - 700, ALLEGRO_ALIGN_CENTER, "-");
	//al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 1.5, SCREEN_H - 700, ALLEGRO_ALIGN_CENTER, "+");
	char str1[10];
	sprintf(str1, "%d", (int)(20 * music_volume));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 700, ALLEGRO_ALIGN_CENTER, str1);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 8, SCREEN_H - 600, ALLEGRO_ALIGN_CENTER, "SFX");
	//al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 3, SCREEN_H - 600, ALLEGRO_ALIGN_CENTER, "-");
	//al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 1.5, SCREEN_H - 600, ALLEGRO_ALIGN_CENTER, "+");
	char str2[10];
	sprintf(str2, "%d", (int)(20 * effect_volume));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 600, ALLEGRO_ALIGN_CENTER, str2);
	al_draw_text(menuFont,al_map_rgb(255, 255, 255),SCREEN_W / 8,SCREEN_H - 500,ALLEGRO_ALIGN_CENTER,"SKINS");
	drawButton(btnMenu);
	drawButton(btnSFXup);
	drawButton(btnVolup);
	drawButton(btnVoldown);
	drawButton(btnSFXdown);
	drawButton(original);
	drawButton(puppet);
	char str3[10];
	sprintf(str3, "%d", (int)(high_score));
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 130, 700, ALLEGRO_ALIGN_CENTER, str3);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 -10, 700, ALLEGRO_ALIGN_CENTER, "Highest score:");
}
static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnMenu.hovered = buttonHover(btnMenu, mouse_x, mouse_y);
	btnVoldown.hovered = buttonHover(btnVoldown, mouse_x, mouse_y);
	btnVolup.hovered = buttonHover(btnVolup, mouse_x, mouse_y);
	btnSFXdown.hovered = buttonHover(btnSFXdown, mouse_x, mouse_y);
	btnSFXup.hovered = buttonHover(btnSFXup, mouse_x, mouse_y);
	original.hovered = buttonHover(original, mouse_x, mouse_y);
	puppet.hovered = buttonHover(puppet, mouse_x, mouse_y);

}
static void on_mouse_down() {
	if (btnMenu.hovered && ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		game_change_scene(scene_menu_create());
	if (btnVoldown.hovered && music_volume > 0) {
		music_volume -= 0.5;
		stop_bgm(settingsBGM);
		settingsBGM = play_bgm(themeMusic, music_volume);
	}
	if (btnVolup.hovered && music_volume < 5) {
		music_volume += 0.5;
		stop_bgm(settingsBGM);
		settingsBGM = play_bgm(themeMusic, music_volume);
	}
	if (btnSFXdown.hovered && effect_volume > 0) {
		effect_volume -= 0.5;
		stop_bgm(settingsBGM);
		settingsBGM = play_bgm(themeMusic, music_volume);
	}
	if (btnSFXup.hovered && effect_volume < 5) {
		effect_volume += 0.5;
		stop_bgm(settingsBGM);
		settingsBGM = play_bgm(themeMusic, music_volume);
	}
	if (original.hovered) {
	 curskin = 0;
	}
	if (puppet.hovered) {
	 curskin = 1;
	}

}
static void destroy() {
	stop_bgm(settingsBGM);
	al_destroy_bitmap(btnMenu.default_img);
	al_destroy_bitmap(btnMenu.hovered_img);
	al_destroy_bitmap(btnVoldown.default_img);
	al_destroy_bitmap(btnVoldown.hovered_img);
	al_destroy_bitmap(btnSFXdown.default_img);
	al_destroy_bitmap(btnSFXdown.hovered_img);
	al_destroy_bitmap(btnSFXup.default_img);
	al_destroy_bitmap(btnSFXup.hovered_img);
	al_destroy_bitmap(original.default_img);
	al_destroy_bitmap(original.hovered_img);
	al_destroy_bitmap(puppet.default_img);
	al_destroy_bitmap(puppet.hovered_img);
	
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
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}