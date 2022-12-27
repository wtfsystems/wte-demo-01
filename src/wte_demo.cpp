/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  wte_demo.cpp
 *
 * See LICENSE.md for copyright information
 *
 * WTE Demo game code.
 * This extends the engine object to define a custom game.
 */

#include <wte_demo.hpp>

#include <iostream>

/*
 * Game's constructor.
 * Initialize game specific variables here.
 */
wte_demo::wte_demo(int argc, char **argv) : engine(argc, argv) {}

/*
 * Game's destructor.
 * Save game variables here.
 */
wte_demo::~wte_demo() {}

/*
 * Load the systems in order.
 */
void wte_demo::load_systems(void) {
    wte::mgr::systems::add<wte::sys::movement>();
    wte::mgr::systems::add<wte::sys::colision>();
    wte::mgr::systems::add<wte::sys::logic>();
    wte::mgr::systems::add<wte::sys::gfx::animate>();
}

/*
 * New game process.
 * Create starting world and load world into the spawner.
 */
void wte_demo::new_game(void) {
    //  Spawn starting entities
    wte::mgr::spawner::spawn("starfield", {});
    wte::mgr::spawner::spawn("score_overlay", {});
    wte::mgr::spawner::spawn("player_info_overlay", {});
    wte::mgr::spawner::spawn("game_over_overlay", {});
    wte::mgr::spawner::spawn("player", {});
    wte::mgr::spawner::spawn("main_cannon", {});
    wte::mgr::spawner::spawn("shield", {});

    //menu_counter = 0;

    //  Reset score.
    wte::mgr::variables::set("score", 0);

    //  Set number of lives.
    if(wte::mgr::variables::get<int>("max_lives") > 5 || wte::mgr::variables::get<int>("max_lives") < 3)
        wte::mgr::variables::set("max_lives", 3);
    wte::mgr::variables::set("lives", wte::mgr::variables::get<int>("max_lives"));

    wte::mgr::audio::music::a::play(wte::mgr::assets::get<ALLEGRO_AUDIO_STREAM>("music"));
}

/*
 * Stop the game.
 */
void wte_demo::end_game(void) {
    if(wte::mgr::variables::get<int>("score") > wte::mgr::variables::get<int>("hiscore"))
        wte::mgr::variables::set("hiscore", wte::mgr::variables::get<int>("score"));
    //menu_counter = 0;
}

/*
 * On menu open.
 */
void wte_demo::on_engine_pause(void) {
    wte::mgr::audio::music::a::pause();
    wte::mgr::audio::ambiance::pause();
    //menu_counter = 0;
}

/*
 * On menu close.
 */
void wte_demo::on_engine_unpause(void) {
    wte::mgr::audio::music::a::unpause();
    wte::mgr::audio::ambiance::unpause();
    //menu_counter = 0;
}
