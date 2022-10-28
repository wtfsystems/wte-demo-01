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

/*
 * Game's constructor.
 * Initialize game specific variables here.
 */
wte_demo::wte_demo(int argc, char **argv) : engine(argc, argv) {
    //  Init Allegro addons that the engine does not use.
    al_init_primitives_addon();
    al_init_acodec_addon();

    //  Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplAllegro5_Init(display::_display);

    ImGui_ImplAllegro5_NewFrame();
    ImGui::NewFrame();

    wte::mgr::gfx::renderer::render_gui = []() {
        ImGui::Render();
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
    };

    input::custom_input_events = [](ALLEGRO_EVENT event){
        ImGui_ImplAllegro5_ProcessEvent(&event);
    };

    /* ********************************* */
    /* *** Game variables ************** */
    /* ********************************* */
    wte::mgr::variables::reg<int>("score", 0);
    wte::mgr::variables::reg<int>("hiscore", 0);
    wte::mgr::variables::reg<int>("max_lives", 3);
    wte::mgr::variables::reg<int>("lives", 3);

    wte::mgr::variables::set_data_file("game.cfg");
    wte::mgr::variables::load<int>("max_lives");
    wte::mgr::variables::load<int>("hiscore");

    /* ********************************* */
    /* *** Input handling ************** */
    /* ********************************* */
    wte::add_handler<wte::NONGAME_HANDLES, wte::EVENT_KEY_DOWN, wte::handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        wte::mgr::messages::add(wte::message("system", "new-game", "game.sdf"));
    });

    wte::add_handler<wte::GAME_HANDLES, wte::EVENT_KEY_DOWN, wte::handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        if(key == config::controls::p1_key_up) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::y = -1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_down) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::y = 1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_left) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::x = -1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_right) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            player_pols::x = 1.0f;
            const float rad = std::atan2(player_pols::y, player_pols::x);
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->direction = rad;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 5.0f;
            wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 5.0f;
        }
        if(key == config::controls::p1_key_action1) {
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
            wte::mgr::audio::sample::play(wte::mgr::assets<wte::al_sample>::get<wte::al_sample>("laser"), "cannon_fire");
        }
        if(key == config::controls::p1_key_action2) {
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
                wte::mgr::audio::sample::play(wte::mgr::assets<wte::al_sample>::get<wte::al_sample>("shield"), "shield_sound");
            }
        }

        if(key == config::controls::key_menu) {
            if(config::flags::engine_paused) {
                config::flags::engine_paused = false;
            } else {
                config::flags::engine_paused = true;
            }
        }
    });

    wte::add_handler<wte::GAME_HANDLES, wte::EVENT_KEY_UP, wte::handler::key>([](const int& key, ALLEGRO_DISPLAY* display) {
        if(key == config::controls::p1_key_up) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::y < 0.0f) player_pols::y = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_down) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::y > 0.0f) player_pols::y = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_left) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::x < 0.0f) player_pols::x = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_right) {
            wte::entity_id player_id = wte::mgr::world::get_id("player");
            if(player_pols::x > 0.0f) player_pols::x = 0.0f;
            if(player_pols::x == 0.0f && player_pols::y == 0.0f) {
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->x_vel = 0.0f;
                wte::mgr::world::set_component<wte::cmp::motion>(player_id)->y_vel = 0.0f;
            }
        }
        if(key == config::controls::p1_key_action1) {
            //  Turn the cannon off.
            wte::entity_id can_id = wte::mgr::world::get_id("main_cannon");
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(can_id)->visible = false;
            wte::mgr::world::set_component<wte::cmp::ai>(can_id)->enabled = false;
            wte::mgr::world::set_component<wte::cmp::hitbox>(can_id)->solid = false;
            //  Stop sound effect.
            wte::mgr::audio::sample::stop("cannon_fire");
        }
        if(key == config::controls::p1_key_action2) {
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

    /* ********************************* */
    /* *** Asset loading *************** */
    /* ********************************* */
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>(
        "starfield",
        wte::al_bitmap(config::gfx::arena_w, config::gfx::arena_h)
    );
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("score_overlay", wte::al_bitmap(200, 20));
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("player_info_overlay", wte::al_bitmap(200, 20));
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("game_over_overlay", wte::al_bitmap());
    wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("game_over_overlay")->load("game_over.png");
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("ship", wte::al_bitmap());
    wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("ship")->load("ship.png");
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("cannon", wte::al_bitmap());
    wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("cannon")->load("cannon.png");
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("shield", wte::al_bitmap());
    wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("shield")->load("shield.png");
    wte::mgr::assets<wte::al_bitmap>::load<wte::al_bitmap>("asteroid", wte::al_bitmap());
    wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("asteroid")->load("asteroid.png");

    //  Load music for the demo into the asset manager.
    wte::mgr::assets<wte::al_audio>::load<wte::al_audio>("music", wte::al_audio("music.ogg"));
    //  Load some samples in the asset manager.
    wte::mgr::assets<wte::al_sample>::load<wte::al_sample>("laser", wte::al_sample("sfx/laser.wav"));
    wte::mgr::assets<wte::al_sample>::load<wte::al_sample>("shield", wte::al_sample("sfx/shield.wav"));
    wte::mgr::assets<wte::al_sample>::load<wte::al_sample>("megumin", wte::al_sample("sfx/megumin.wav"));

    /* **************************************************** */
    /* *** ENTITY CREATION ******************************** */
    /* **************************************************** */
    /* ********************************* */
    /* *** Background entity *********** */
    /* ********************************* */
    wte::mgr::spawner::add("starfield", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "starfield");
            wte::mgr::world::add_component<stars>(e_id);
            wte::mgr::world::add_component<wte::cmp::gfx::background>(e_id,
                wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("starfield"), layer::background, 0, 0,
                [](const wte::entity_id& bkg_id) {
                    //  Define the animation process for the starfield.
                    wte::mgr::world::set_component<wte::cmp::gfx::background>(bkg_id)->set_drawing();
                    al_clear_to_color(WTE_COLOR_BLACK);

                    //  Move the stars.
                    for(std::size_t i = 0; i < MAX_STARS; i++) {
                        wte::mgr::world::set_component<stars>(bkg_id)->y[i] +=
                            wte::mgr::world::get_component<stars>(bkg_id)->speed[i] * wte::mgr::world::get_component<stars>(bkg_id)->speed_mult;
                        if(wte::mgr::world::get_component<stars>(bkg_id)->y[i] > config::gfx::arena_h) {
                            //  Make a new star.
                            wte::mgr::world::set_component<stars>(bkg_id)->x[i] =
                                std::rand() % config::gfx::arena_w + 1;
                            wte::mgr::world::set_component<stars>(bkg_id)->y[i] = 0;
                            wte::mgr::world::set_component<stars>(bkg_id)->speed[i] = (std::rand() % 3 + 1) * 3;
                            wte::mgr::world::set_component<stars>(bkg_id)->color[i] = std::rand() % 4 + 1;
                        }
                    }

                    //  Draw the stars.
                    for(std::size_t i = 0; i < MAX_STARS; i++) {
                        if(wte::mgr::world::get_component<stars>(bkg_id)->color[i] == 1 ||
                        wte::mgr::world::get_component<stars>(bkg_id)->color[i] == 4)
                            al_draw_pixel(wte::mgr::world::get_component<stars>(bkg_id)->x[i],
                                        wte::mgr::world::get_component<stars>(bkg_id)->y[i], WTE_COLOR_WHITE);
                        if(wte::mgr::world::get_component<stars>(bkg_id)->color[i] == 2)
                            al_draw_pixel(wte::mgr::world::get_component<stars>(bkg_id)->x[i],
                                        wte::mgr::world::get_component<stars>(bkg_id)->y[i], WTE_COLOR_YELLOW);
                        if(wte::mgr::world::get_component<stars>(bkg_id)->color[i] == 3)
                            al_draw_pixel(wte::mgr::world::get_component<stars>(bkg_id)->x[i],
                                        wte::mgr::world::get_component<stars>(bkg_id)->y[i], WTE_COLOR_RED);
                    }
                });  //  End background rendering.

                wte::mgr::world::add_component<wte::cmp::dispatcher>(e_id,
                    [](const wte::entity_id& bkg_id, const wte::message& msg) {
                        //  Define message processing for the starfield.
                        if(msg.get_cmd() == "default") wte::mgr::world::set_component<stars>(bkg_id)->speed_mult = 1;
                        if(msg.get_cmd() == "up") wte::mgr::world::set_component<stars>(bkg_id)->speed_mult *= 2;
                        if(msg.get_cmd() == "down") wte::mgr::world::set_component<stars>(bkg_id)->speed_mult /= 2;
                        if(msg.get_cmd() == "reset") {
                            wte::mgr::world::set_component<stars>(bkg_id)->speed_mult = 1;

                            for(std::size_t i = 0; i < MAX_STARS; i++) {
                                wte::mgr::world::set_component<stars>(bkg_id)->x[i] =
                                    std::rand() % config::gfx::arena_w + 1;
                                wte::mgr::world::set_component<stars>(bkg_id)->y[i] =
                                    std::rand() % config::gfx::arena_h + 1;
                                wte::mgr::world::set_component<stars>(bkg_id)->speed[i] = (std::rand() % 3 + 1) * 3;
                                wte::mgr::world::set_component<stars>(bkg_id)->color[i] = std::rand() % 4 + 1;
                            }
                        }
                    }  //  End starfield message processing.
                );  //  End dispatcher component.
        }
    );

    /* ********************************* */
    /* *** Score overlay entity ******** */
    /* ********************************* */
    wte::mgr::spawner::add("score_overlay", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "score_overlay");
            wte::mgr::world::add_component<wte::cmp::gfx::overlay>(e_id,
                wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("score_overlay"),
                wte::mgr::assets<wte::al_font>::get<wte::al_font>("wte_default_font"),
                layer::overlay, config::gfx::arena_h - 20, 0,
                [](const wte::entity_id& ovr_id) {
                    //  Define what gets displayed on the overlay.
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->set_drawing();
                    al_clear_to_color(WTE_COLOR_TRANSPARENT);
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->draw_text("Score:  ", WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_RIGHT);
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->draw_text(std::to_string(wte::mgr::variables::get<int>("score")), WTE_COLOR_WHITE, 110, 0, ALLEGRO_ALIGN_LEFT);
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->draw_text("High Score:  ", WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_RIGHT);
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->draw_text(std::to_string(wte::mgr::variables::get<int>("hiscore")), WTE_COLOR_WHITE, 110, 10, ALLEGRO_ALIGN_LEFT);
                }
            );  //  End score overlay drawing.
        }
    );

    /* ********************************* */
    /* *** Player Info overlay entity ** */
    /* ********************************* */
    wte::mgr::spawner::add("player_info_overlay", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "player_info_overlay");
            wte::mgr::world::add_component<wte::cmp::gfx::overlay>(e_id,
                wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("player_info_overlay"),
                wte::mgr::assets<wte::al_font>::get<wte::al_font>("wte_default_font"), layer::overlay,
                config::gfx::arena_w - 200, config::gfx::arena_h - 20,
                [](const wte::entity_id& ovr_id) {
                    //  Define what gets displayed on the overlay.
                    wte::entity_id shd_id = wte::mgr::world::get_id("shield");
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->set_drawing();
                    al_clear_to_color(WTE_COLOR_TRANSPARENT);
                    al_draw_filled_rectangle((float)(120 - wte::mgr::world::get_component<energy>(shd_id)->amt), 0, 120, 10, WTE_COLOR_YELLOW);
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->draw_text("Shield", WTE_COLOR_WHITE, 200, 0, ALLEGRO_ALIGN_RIGHT);
                    wte::mgr::world::set_component<wte::cmp::gfx::overlay>(ovr_id)->draw_text("Lives:  " + std::to_string(wte::mgr::variables::get<int>("lives")), WTE_COLOR_WHITE, 200, 10, ALLEGRO_ALIGN_RIGHT);
                }
            );  //  End info overlay drawing.
        }
    );

    /* ********************************* */
    /* *** Game Over overlay entity **** */
    /* ********************************* */
    wte::mgr::spawner::add("game_over_overlay", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "game_over_overlay");
            wte::mgr::world::add_component<wte::cmp::gfx::overlay>(e_id,
                wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("game_over_overlay"),
                wte::mgr::assets<wte::al_font>::get<wte::al_font>("wte_default_font"),
                layer::overlay, (config::gfx::arena_w / 2) - 240, (config::gfx::arena_h / 2) - 66,
                [](const wte::entity_id& ovr_id) {}
            );
            wte::mgr::world::set_component<wte::cmp::gfx::overlay>(e_id)->visible = false;
        }
    );

    /* ********************************* */
    /* *** Player entity *************** */
    /* ********************************* */
    wte::mgr::spawner::add("player", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "player");
            wte::mgr::world::add_component<wte::cmp::location>(e_id,
                (config::gfx::arena_w / 2) - 5,
                config::gfx::arena_h - 40);
            wte::mgr::world::add_component<wte::cmp::hitbox>(e_id, 10, 10, 0);
            wte::mgr::world::add_component<wte::cmp::bounding_box>(e_id, 12.0f, 0.0f,
                (float)(config::gfx::arena_w - 21),
                (float)(config::gfx::arena_h - 32));
            wte::mgr::world::add_component<health>(e_id, 1, 1);
            wte::mgr::world::add_component<wte::cmp::motion>(e_id, 0.0f, 0.0f, 0.0f);
            wte::mgr::world::add_component<wte::cmp::gfx::sprite>(e_id, wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("ship"),
                                                layer::player, 32.0f, 32.0f, -11.0f, 0.0f, 1);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 3);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->add_cycle("death", 4, 7);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->set_cycle("main");

            //  Player logic.
            wte::mgr::world::add_component<wte::cmp::ai>(e_id,
                [](const wte::entity_id& plr_id) {
                    if(wte::mgr::world::get_component<health>(plr_id)->hp <= 0) {  //  Check player health.
                        wte::mgr::world::set_component<wte::cmp::ai>(plr_id)->enabled = false;
                        wte::mgr::world::set_component<health>(plr_id)->hp = wte::mgr::world::get_component<health>(plr_id)->hp_max;
                        std::string player_name = wte::mgr::world::get_name(plr_id);
                        wte::mgr::messages::add(wte::message("entities", player_name, player_name, "death", ""));
                    }
                }
            );  //  End player logic.

            //  Player message handling.
            wte::mgr::world::add_component<wte::cmp::dispatcher>(e_id,
                [](const wte::entity_id& plr_id, const wte::message& msg) {
                    //  Process player death.
                    if(msg.get_cmd() == "death") {
                        config::flags::input_enabled = false;
                        //  Make sure cannon stops firing
                        wte::entity_id cannon_id = wte::mgr::world::get_id("main_cannon");
                        wte::mgr::world::set_component<wte::cmp::gfx::sprite>(cannon_id)->visible = false;
                        wte::mgr::world::set_component<wte::cmp::ai>(cannon_id)->enabled = false;
                        wte::mgr::world::set_component<wte::cmp::hitbox>(cannon_id)->solid = false;
                        wte::mgr::audio::sample::stop("cannon_fire");

                        //  Just to make sure... turn shield off
                        wte::entity_id shield_id = wte::mgr::world::get_id("shield");
                        wte::mgr::world::set_component<wte::cmp::gfx::sprite>(shield_id)->visible = false;
                        wte::mgr::world::set_component<wte::cmp::ai>(shield_id)->enabled = false;
                        wte::mgr::world::set_component<wte::cmp::hitbox>(shield_id)->solid = false;
                        wte::mgr::audio::sample::stop("shield_sound");

                        wte::mgr::world::set_component<wte::cmp::hitbox>(plr_id)->solid = false;

                        wte::mgr::audio::sample::play(wte::mgr::assets<wte::al_sample>::get<wte::al_sample>("megumin"), "once");
                        wte::mgr::variables::set<int>("lives", wte::mgr::variables::get<int>("lives") - 1);
                        wte::mgr::world::set_component<wte::cmp::motion>(plr_id)->x_vel = 0.0f;
                        wte::mgr::world::set_component<wte::cmp::motion>(plr_id)->y_vel = 0.0f;
                        wte::mgr::world::set_component<wte::cmp::gfx::sprite>(plr_id)->set_cycle("death");
                        if(wte::mgr::variables::get<int>("lives") == 0) {
                            //  Game over!
                            wte::mgr::messages::add(wte::message(wte::engine_time::check() + 180, "system", "end-game", ""));
                            wte::entity_id go_id = wte::mgr::world::get_id("game_over_overlay");
                            wte::mgr::world::set_component<wte::cmp::gfx::overlay>(go_id)->visible = true;
                        } else {
                            std::string player_name = wte::mgr::world::get_name(plr_id);
                            wte::mgr::messages::add(
                                wte::message(wte::engine_time::check() + 120, "entities", player_name, player_name, "reset", "")
                            );
                        }
                    }

                    //  Reset player.
                    if(msg.get_cmd() == "reset") {
                        config::flags::input_enabled = true;
                        player_pols::reset();
                        wte::mgr::world::set_component<wte::cmp::motion>(plr_id)->x_vel = 0.0f;
                        wte::mgr::world::set_component<wte::cmp::motion>(plr_id)->y_vel = 0.0f;
                        wte::mgr::world::set_component<wte::cmp::location>(plr_id)->pos_x = (float)((config::gfx::arena_w / 2) - 5);
                        wte::mgr::world::set_component<wte::cmp::location>(plr_id)->pos_y = (float)(config::gfx::arena_h - 40);
                        wte::mgr::world::set_component<health>(plr_id)->hp = wte::mgr::world::get_component<health>(plr_id)->hp_max;
                        wte::mgr::world::set_component<wte::cmp::ai>(plr_id)->enabled = true;
                        wte::mgr::world::set_component<wte::cmp::gfx::sprite>(plr_id)->set_cycle("main");
                        wte::mgr::world::set_component<wte::cmp::hitbox>(plr_id)->solid = true;
                    }

                    //  Take damage.
                    if(msg.get_cmd() == "damage") {
                        wte::mgr::world::set_component<health>(plr_id)->hp -= std::stoi(msg.get_arg(0));
                    }
                }
            );  //  End player message processing.
        }
    );

    /* ********************************* */
    /* *** Main cannon entity ********** */
    /* ********************************* */
    wte::mgr::spawner::add("main_cannon", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "main_cannon");
            wte::mgr::world::add_component<wte::cmp::location>(e_id, 0, 0);
            wte::mgr::world::add_component<wte::cmp::hitbox>(e_id, 10, 200, 0, false);
            wte::mgr::world::set_component<wte::cmp::hitbox>(e_id)->solid = false;
            wte::mgr::world::add_component<damage>(e_id, 3);
            wte::mgr::world::add_component<wte::cmp::gfx::sprite>(e_id, wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("cannon"),
                                                layer::player, 10.0f, 200.0f, 0.0f, 0.0f, 2);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 3);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->set_cycle("main");
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->visible = false;

            //  Cannon logic.
            wte::mgr::world::add_component<wte::cmp::ai>(e_id,
                [](const wte::entity_id& can_id) {
                    wte::entity_id player_entity = wte::mgr::world::get_id("player");
                    //  Set the cannon's location to match the player.
                    wte::mgr::world::set_component<wte::cmp::location>(can_id)->pos_x =
                        wte::mgr::world::get_component<wte::cmp::location>(player_entity)->pos_x;
                    wte::mgr::world::set_component<wte::cmp::location>(can_id)->pos_y =
                        wte::mgr::world::get_component<wte::cmp::location>(player_entity)->pos_y -
                        wte::mgr::world::get_component<wte::cmp::hitbox>(can_id)->height;
                }
            );  //  End cannon logic.
            wte::mgr::world::set_component<wte::cmp::ai>(e_id)->enabled = false;

            //  Cannon message processing.
            wte::mgr::world::add_component<wte::cmp::dispatcher>(e_id,
                [](const wte::entity_id& can_id, const wte::message& msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        wte::mgr::messages::add(wte::message("entities", msg.get_from(), msg.get_to(),
                            "damage", std::to_string(wte::mgr::world::get_component<damage>(can_id)->dmg)));
                    }
                }
            );  //  End cannon message processing.
        }
    );

    /* ********************************* */
    /* *** Shield entity *************** */
    /* ********************************* */
    wte::mgr::spawner::add("shield", 0,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            wte::mgr::world::set_name(e_id, "shield");
            wte::mgr::world::add_component<wte::cmp::location>(e_id, 0, 0);
            wte::mgr::world::add_component<wte::cmp::hitbox>(e_id, 64, 64, 0, false);
            wte::mgr::world::set_component<wte::cmp::hitbox>(e_id)->solid = false;
            wte::mgr::world::add_component<energy>(e_id, 50, 100);
            wte::mgr::world::add_component<damage>(e_id, 100);
            wte::mgr::world::add_component<wte::cmp::gfx::sprite>(e_id, wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("shield"),
                                                layer::player, 64.0f, 64.0f, 0.0f, 0.0f, 6);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 5);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->set_cycle("main");
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->visible = false;

            //  Shield logic.
            wte::mgr::world::add_component<wte::cmp::ai>(e_id,
                //  Enabeled AI.
                [](const wte::entity_id& shd_id) {
                    wte::entity_id player_entity = wte::mgr::world::get_id("player");
                    //  Set the shield's location to match the player.
                    wte::mgr::world::set_component<wte::cmp::location>(shd_id)->pos_x =
                        wte::mgr::world::get_component<wte::cmp::location>(player_entity)->pos_x - 28.0f;
                    wte::mgr::world::set_component<wte::cmp::location>(shd_id)->pos_y =
                        wte::mgr::world::get_component<wte::cmp::location>(player_entity)->pos_y - 16.0f;

                    //  Drain the shield.
                    if(wte::mgr::world::set_component<energy>(shd_id)->amt > 0)
                        wte::mgr::world::set_component<energy>(shd_id)->amt -= 1;

                    if(wte::mgr::world::get_component<energy>(shd_id)->amt <= 0) {
                        //  Disable shield.
                        wte::mgr::world::set_component<wte::cmp::gfx::sprite>(shd_id)->visible = false;
                        wte::mgr::world::set_component<wte::cmp::ai>(shd_id)->enabled = false;
                        wte::mgr::world::set_component<wte::cmp::hitbox>(player_entity)->solid = true;
                        //  Stop sound effect.
                        wte::mgr::audio::sample::stop("shield_sound");
                    }
                },
                //  Disabeled AI.
                [](const wte::entity_id& shd_id) {
                    //  Recharge the shield.
                    if(wte::mgr::world::set_component<energy>(shd_id)->amt < wte::mgr::world::set_component<energy>(shd_id)->amt_max)
                        wte::mgr::world::set_component<energy>(shd_id)->amt += 1;
                }
            );  //  End shield logic.
            wte::mgr::world::set_component<wte::cmp::ai>(e_id)->enabled = false;

            //  Shield message processing.
            wte::mgr::world::add_component<wte::cmp::dispatcher>(e_id,
                [](const wte::entity_id& shd_id, const wte::message& msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        wte::mgr::messages::add(wte::message("entities", msg.get_from(), msg.get_to(),
                            "damage", std::to_string(wte::mgr::world::get_component<damage>(shd_id)->dmg)));
                    }
                }
            );  //  End shield message processing.
        }
    );

    /* ************************************** */
    /* Add the asteroid entity to the spawner */
    /* Arguments:                             */
    /*  (1) X location                        */
    /*  (2) Y location                        */
    /*  (3) Direction in degrees              */
    /*  (4) Velocity                          */
    /*  (5) Size                              */
    /* ************************************** */
    wte::mgr::spawner::add("asteroid", 5,
        [](const wte::entity_id& e_id, const wte::msg_args& args) {
            int temp_size = std::stoi(args[5]);
            if(temp_size < 1) temp_size = 1;
            if(temp_size > 8) temp_size = 8;

            wte::mgr::world::set_name(e_id, "asteroid" + std::to_string(e_id));
            wte::mgr::world::add_component<wte::cmp::location>(e_id, std::stof(args[1]), std::stof(args[2]));
            wte::mgr::world::add_component<wte::cmp::hitbox>(e_id, (float)(temp_size * 16), (float)(temp_size * 16), 1);
            wte::mgr::world::add_component<health>(e_id, temp_size * 10, temp_size * 10);
            wte::mgr::world::add_component<damage>(e_id, 10);
            wte::mgr::world::add_component<size>(e_id, temp_size);
            wte::mgr::world::add_component<wte::cmp::motion>(e_id, 0.0f, 0.0f, 0.0f);
            wte::mgr::world::set_component<wte::cmp::motion>(e_id)->direction = std::stof(args[3]) * (M_PI / 180);
            wte::mgr::world::set_component<wte::cmp::motion>(e_id)->x_vel = std::stof(args[4]);
            wte::mgr::world::set_component<wte::cmp::motion>(e_id)->y_vel = std::stof(args[4]);

            wte::mgr::world::add_component<wte::cmp::gfx::sprite>(e_id, wte::mgr::assets<wte::al_bitmap>::get<wte::al_bitmap>("asteroid"),
                layer::enemy, 16.0f, 16.0f, 0.0f, 0.0f, (int)(30 / std::stof(args[4])));
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->add_cycle("main", 0, 5);
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->set_cycle("main");
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->scale_factor_x = (float)temp_size;
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->scale_factor_y = (float)temp_size;
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->rotated = true;
            wte::mgr::world::set_component<wte::cmp::gfx::sprite>(e_id)->direction = std::stof(args[3]) * (M_PI / 180);

            //  Asteroid logic.
            wte::mgr::world::add_component<wte::cmp::ai>(e_id,
                [](const wte::entity_id& ast_id) {
                    //  AI for asteroids defined here.
                    //  Perform OOB check.
                    if(wte::mgr::world::get_component<wte::cmp::location>(ast_id)->pos_y > (float)(config::gfx::arena_h + 100)) {
                        wte::mgr::messages::add(wte::message("spawner", "delete", wte::mgr::world::get_name(ast_id)));
                    }

                    //  Health check.  If asteroid's HP is <= 0, reward player with points and delete the entity.
                    if(wte::mgr::world::get_component<health>(ast_id)->hp <= 0) {
                        wte::mgr::messages::add(wte::message("spawner", "delete", wte::mgr::world::get_name(ast_id)));
                        wte::mgr::audio::sample::play(wte::mgr::assets<wte::al_sample>::get<wte::al_sample>("megumin"), "once", 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.8f);

                        wte::mgr::variables::set("score",
                            (wte::mgr::variables::get<int>("score") +
                            (10 * wte::mgr::world::get_component<size>(ast_id)->the_size)));

                        //  If the asteroid was size >= 4, split into two.
                        if(wte::mgr::world::get_component<size>(ast_id)->the_size >= 4) {
                            const int new_size = wte::mgr::world::get_component<size>(ast_id)->the_size / 2;
                            float dir_a = wte::mgr::world::get_component<wte::cmp::motion>(ast_id)->direction - 90.0f;
                            if(dir_a < 0.0f) dir_a = 0.0f;
                            float dir_b = wte::mgr::world::get_component<wte::cmp::motion>(ast_id)->direction + 90.0f;
                            if(dir_b > 360.0f) dir_b = 360.0f;
                            const float new_x = wte::mgr::world::get_component<wte::cmp::location>(ast_id)->pos_x;
                            const float new_y = wte::mgr::world::get_component<wte::cmp::location>(ast_id)->pos_y;
                            const float new_vel = wte::mgr::world::get_component<wte::cmp::motion>(ast_id)->x_vel / 2;
                            std::string new_spawner_a = "asteroid;" + std::to_string(new_x) + ";" +
                                std::to_string(new_y) + ";" + std::to_string(dir_a) + ";" +
                                std::to_string(new_vel) + ";" + std::to_string(new_size);
                            std::string new_spawner_b = "asteroid;" + std::to_string(new_x) + ";" +
                                std::to_string(new_y) + ";" + std::to_string(dir_b) + ";" +
                                std::to_string(new_vel) + ";" + std::to_string(new_size);
                            wte::mgr::messages::add(wte::message("spawner", "new", new_spawner_a));
                            wte::mgr::messages::add(wte::message("spawner", "new", new_spawner_b));
                        }
                    }
                }
            );  //  End asteroid AI

            //  Asteroid message processing.
            wte::mgr::world::add_component<wte::cmp::dispatcher>(e_id,
                [](const wte::entity_id& ast_id, const wte::message& msg) {
                    if(msg.get_cmd() == "colision") {
                        //  Deal damage
                        wte::mgr::messages::add(wte::message("entities", msg.get_from(), msg.get_to(),
                            "damage", std::to_string(wte::mgr::world::get_component<damage>(ast_id)->dmg)));
                    }

                    if(msg.get_cmd() == "damage") {
                        wte::mgr::world::set_component<health>(ast_id)->hp -= std::stoi(msg.get_arg(0));
                    }
                }
            );  //  End asteroid message dispatching.
        }  //  End asteroid spawner function.
    );
    /* **************************************************** */
    /* *** END ENTITY CREATION **************************** */
    /* **************************************************** */
}

/*
 * Game's destructor.
 * Save game variables here.
 */
wte_demo::~wte_demo() {
    wte::mgr::variables::clear_save();
    wte::mgr::variables::save<int>("max_lives");
    wte::mgr::variables::save<int>("hiscore");

    //  Shut down Dear ImGui
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    
    al_shutdown_primitives_addon();
}

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

    //  Reset score.
    wte::mgr::variables::set("score", 0);

    //  Set number of lives.
    if(wte::mgr::variables::get<int>("max_lives") > 5 || wte::mgr::variables::get<int>("max_lives") < 3)
        wte::mgr::variables::set("max_lives", 3);
    wte::mgr::variables::set("lives", wte::mgr::variables::get<int>("max_lives"));

    wte::mgr::audio::music::a::play(wte::mgr::assets<wte::al_audio>::get<wte::al_audio>("music"));
}

/*
 * Stop the game.
 */
void wte_demo::end_game(void) {
    if(wte::mgr::variables::get<int>("score") > wte::mgr::variables::get<int>("hiscore"))
        wte::mgr::variables::set("hiscore", wte::mgr::variables::get<int>("score"));
}

/*
 *
 */
void wte_demo::pre_resize_display(void) {
    ImGui_ImplAllegro5_InvalidateDeviceObjects();
}

/*
 *
 */
void wte_demo::post_resize_display(void) {
    ImGui_ImplAllegro5_CreateDeviceObjects();
}

/*
 *
 */
void wte_demo::default_state(void) {
    //ImGui::Begin("Hello, world!");
    //ImGui::Text("This is some useful text.");
    //ImGui::End();
}

/*
 * On menu open.
 */
void wte_demo::on_engine_pause(void) {
    wte::mgr::audio::music::a::pause();
    wte::mgr::audio::ambiance::pause();
}

/*
 * On menu close.
 */
void wte_demo::on_engine_unpause(void) {
    wte::mgr::audio::music::a::unpause();
    wte::mgr::audio::ambiance::unpause();
}
