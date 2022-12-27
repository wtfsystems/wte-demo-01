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

    /* ********************************* */
    /* *** Input handling ************** */
    /* ********************************* */
    wte::add_handler<wte::NONGAME_HANDLES, wte::EVENT_KEY_DOWN, wte::handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        //wte::mgr::messages::add(wte::message("system", "new-game", "game.sdf"));
    });

    wte::add_handler<wte::GAME_HANDLES, wte::EVENT_KEY_DOWN, wte::handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        if(key == wte::config::controls::p1_key_up) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::y = -1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == wte::config::controls::p1_key_down) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::y = 1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == wte::config::controls::p1_key_left) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::x = -1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == wte::config::controls::p1_key_right) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::x = 1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == wte::config::controls::p1_key_action1) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            wte::entity_id can_id = wte::mgr::world::get_id("main_cannon");
            //  Set the cannon's location to match the player.
            wte::mgr::world::set_component<wte::cmp::location>(can_id)->pos_x = 
                wte::mgr::world::get_component<wte::cmp::location>(player_id)->pos_x;
            wte::mgr::world::set_component<wte::cmp::location>(can_id)->pos_y = 
                wte::mgr::world::get_component<wte::cmp::location>(player_id)->pos_y -
                wte::mgr::world::get_component<wte::cmp::hitbox>(can_id)->height;

            //  Turn the cannon on.
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(can_id)->visible = true;
            wte::mgr::world::set_component<wte::cmp::ai>(can_id)->enabled = true;
            wte::mgr::world::set_component<wte::cmp::hitbox>(can_id)->solid = true;
            //  Play sound effect.
            wte::mgr::audio::sample::play(wte::mgr::assets::get<ALLEGRO_SAMPLE>("laser"), "cannon_fire");
        }
        if(key == wte::config::controls::p1_key_action2) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            wte::entity_id shd_id = wte::mgr::world::get_id("shield");
            //  Set the shield's location to match the player
            wte::mgr::world::set_component<wte::cmp::location>(shd_id)->pos_x =
                wte::mgr::world::get_component<wte::cmp::location>(player_id)->pos_x - 28.0f;
            wte::mgr::world::set_component<wte::cmp::location>(shd_id)->pos_y =
                wte::mgr::world::get_component<wte::cmp::location>(player_id)->pos_y - 16.0f;

            if(wte::mgr::world::set_component<energy>(shd_id)->amt > 0) {
                //  Enable the shield.
                wte::mgr::world::set_component<wte::cmp::gfx::sprite>(shd_id)->visible = true;
                wte::mgr::world::set_component<wte::cmp::ai>(shd_id)->enabled = true;
                wte::mgr::world::set_component<wte::cmp::hitbox>(shd_id)->solid = true;
                wte::mgr::world::set_component<wte::cmp::hitbox>(player_id)->solid = false;
                //  Play sound effect.
                wte::mgr::audio::sample::play(wte::mgr::assets::get<ALLEGRO_SAMPLE>("shield"), "shield_sound");
            }
        }

        if(key == wte::config::controls::key_menu) {
            if(wte::config::flags::engine_paused) {
                wte::config::flags::engine_paused = false;
            } else {
                wte::config::flags::engine_paused = true;
            }
        }
    });

    wte::add_handler<wte::GAME_HANDLES, wte::EVENT_KEY_UP, wte::handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        if(key == wte::config::controls::p1_key_up) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::y < 0.0f) player_pols::y = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == wte::config::controls::p1_key_down) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::y > 0.0f) player_pols::y = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == wte::config::controls::p1_key_left) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::x < 0.0f) player_pols::x = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == wte::config::controls::p1_key_right) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::x > 0.0f) player_pols::x = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == wte::config::controls::p1_key_action1) {
            //  Turn the cannon off.
            wte::entity_id can_id = wte::mgr::world::get_id("main_cannon");
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(can_id)->visible = false;
            wte::mgr::world::set_component<wte::cmp::ai>(can_id)->enabled = false;
            wte::mgr::world::set_component<wte::cmp::hitbox>(can_id)->solid = false;
            //  Stop sound effect.
            wte::mgr::audio::sample::stop("cannon_fire");
        }
        if(key == wte::config::controls::p1_key_action2) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            wte::entity_id shd_id = wte::mgr::world::get_id("shield");
            //  Disable shield.
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(shd_id)->visible = false;
            wte::mgr::world::set_component<wte::cmp::ai>(shd_id)->enabled = false;
            wte::mgr::world::set_component<wte::cmp::hitbox>(shd_id)->solid = false;
            wte::mgr::world::set_component<wte::cmp::hitbox>(player_id)->solid = true;
            //  Stop sound effect.
            wte::mgr::audio::sample::stop("shield_sound");
        }
    });

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
