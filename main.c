#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include <stdbool.h>
#include <stdio.h>

static char *lurendian = "rom:/lurendian.sprite";
static char *tianlu = "rom:/tianlu.sprite";
static char *jewel1s = "rom:/jewel1.sprite";
static char *jewel0s = "rom:/jewel0.sprite";
static char *tianlu2s = "rom:/tianlu2.sprite";
static sprite_t *background;
static sprite_t *jewel0;
static sprite_t *jewel1;
static sprite_t *tianlu2;

bool grid[5][5];

void flip(int x, int y) {
    grid[y][x] = !grid[y][x];
}

void swap(int x, int y) {
    flip(x, y);
    if (x - 1 >= 0) {
        flip(x - 1, y);
    }
    if (x + 1 < 5) {
        flip(x + 1, y);
    }
    if (y - 1 >= 0) {
        flip(x, y - 1);
    }
    if (y + 1 < 5) {
        flip(x, y + 1);
    }
}

int main(void) {
    console_init();

    timer_init();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
	
    dfs_init(DFS_DEFAULT_LOCATION);
	
    joypad_init();

    surface_t* disp = display_get();

    background = sprite_load(lurendian);
    graphics_draw_sprite(disp, 0, 0, background);

    graphics_draw_text(disp, 20, 0, "Tianlu's Jewel Quest 64");
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

    background = sprite_load(tianlu);
    graphics_draw_sprite(disp, 0, 0, background);

    int level = 1;

    jewel0 = sprite_load(jewel0s);
    jewel1 = sprite_load(jewel1s);
    tianlu2 = sprite_load(tianlu2s);

    while (1) {
        int px = 0;
        int py = 0;

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                grid[i][j] = false;
            }
        }

        srand(TICKS_READ());

        for (int i = 0; i < level; i++) {
            int x = rand() % 5;
            int y = rand() % 5;

            swap(x, y);
        }

        while (1) {
            graphics_draw_sprite(disp, 0, 0, background);

            graphics_draw_text(disp, 5, 5, "Level:");
            char str[255];
            sprintf(str, "%d", level);
            graphics_draw_text(disp, 5, 15, str);

            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    if (grid[y][x]) {
                        graphics_draw_sprite_trans(disp, x*25+100, y*25+50, jewel1);
                    } else {
                        graphics_draw_sprite_trans(disp, x*25+100, y*25+50, jewel0);
                    }
                    if (x == px && y == py) {
                        graphics_draw_sprite_trans(disp, x*25+100, y*25+50, tianlu2);
                    }
                }
            }

            while (1) {
                joypad_poll();
        		joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

                if (keys.d_up) {
                    if (py > 0) {
                        py--;
                    }
                    break;
                }
                if (keys.d_down) {
                    if (py < 4) {
                        py++;
                    }
                    break;
                }
                if (keys.d_right) {
                    if (px < 4) {
                        px++;
                    }
                    break;
                }
                if (keys.d_left) {
                    if (px > 0) {
                        px--;
                    }
                    break;
                }
                if (keys.a) {
                    swap(px, py);
                    break;
                }
            }

            int lit = 0;

            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    if (grid[y][x]) {
                        lit++;
                    }
                }
            }

            if (lit == 0) {
                graphics_draw_sprite(disp, 0, 0, background);
                graphics_draw_text(disp, 30, 30, "All jewels collected!");
            
                while (1) {
                    joypad_poll();
	                joypad_buttons_t keys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

                    if (keys.start) {
                        break;
                    }
                }

                level++;

                break;
            }
        }
    }
}