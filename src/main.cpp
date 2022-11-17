/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  main.cpp
 *
 * See LICENSE.md for copyright information
 *
 * Main file for WTEngine demo
 * Initializes engine and starts the game loop.
 */

#include <wtengine/wtengine.hpp>

#include <wte_demo.hpp>

int main(int argc, char **argv) {
    //  Set locations to load game data from.
    wte::engine::add_file_location("data.zip");

    //  Set key bindings
    wte::config::controls::p1_key_up = ALLEGRO_KEY_W;
    wte::config::controls::p1_key_down = ALLEGRO_KEY_S;
    wte::config::controls::p1_key_left = ALLEGRO_KEY_A;
    wte::config::controls::p1_key_right = ALLEGRO_KEY_D;
    wte::config::controls::p1_key_action1 = ALLEGRO_KEY_RCTRL;
    wte::config::controls::p1_key_action2 = ALLEGRO_KEY_ALTGR;

    //  Load settings.
    wte::config::load();

    //  Set the title screen.
    wte::mgr::gfx::renderer::set_title_screen("title.bmp");

    //  Set the size of the play area.
    wte::mgr::gfx::renderer::set_viewport_size(768, 1024);

    //  Initialize game object.
    wte_demo my_game(argc, argv);
    //  Run the game loop.
    my_game.do_game();

    //  Save settings.
    wte::config::save();

    return 0; //  Exit program.
}
