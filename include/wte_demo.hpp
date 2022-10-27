/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  wte_demo.hpp
 *
 * See LICENSE.txt for copyright information
 *
 * Header file for the wte_demo object
 */

#ifndef WTEDEMO_WTE_DEMO_HPP
#define WTEDEMO_WTE_DEMO_HPP

#include <numeric>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>

#include <imgui.h>
#include <imgui_impl_allegro5.h>

#include <wtengine/wtengine.hpp>

#include <my_components.hpp>

namespace layer {
    inline const std::size_t background = 0;
    inline const std::size_t enemy = 1;
    inline const std::size_t player = 2;
    inline const std::size_t overlay = 0;
}

namespace player_pols {
    inline static float x = 0.0f;
    inline static float y = 0.0f;
    inline static void reset(void) { x = y = 0.0f; };
}

class wte_demo final : public wte::engine {
    public:
        wte_demo(int argc, char **argv);
        ~wte_demo();

    private:
        //  Define this to load all systems to be used by the game
        void load_systems(void) override;
        //  Define what gets loaded when a game starts
        void new_game(void) override;
        //  Define what happens at the end of a game
        void end_game(void) override;
        //  On menu open
        void on_engine_pause(void) override;
        //  On menu close
        void on_engine_unpause(void) override;

        void pre_resize_display(void) override;
        void post_resize_display(void) override;
        void default_state(void) override;
};

#endif
