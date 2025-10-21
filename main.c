#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

static char *lurendian = "rom:/lurendian.sprite";
static sprite_t *background;

int main(void) {
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();

    surface_t* disp = display_get();

    background = sprite_load(lurendian);
    graphics_draw_sprite(disp, 0, 0, background);

    graphics_draw_text(disp, 20, 0, "Tianlu's Gem Quest 64");
    graphics_draw_text(disp, 20, 10, "     Fan game by VegeSushi");

    display_show(disp);

    while (1) {
        joypad_poll();
		joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (keys.start) {
            graphics_draw_sprite(disp, 0, 0, background);
            break;
        }
    }
}