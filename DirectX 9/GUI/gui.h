#pragma once

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>
#include <imgui_tricks.hpp>
#include <functional>

namespace ui {
    inline ImVec2 size{ 500, 400 };
    inline bool moving_window = true;
    inline int cur_page = 0;
    inline int next_page = 0;

    inline float content_anim = 0.f;
    inline float content_anim_dest = 1.f;
    inline float anim_speed = 0.12f;

    void rotate_start( );
    ImVec2 rotation_center( );
    void rotate_end( float rad, ImVec2 center );

    void circle_glow( ImVec2 center, float radius, ImColor col, float intensity );
    void rect_glow( ImDrawList* draw, ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity );

    void render_background( );
    void render_step( const char* icon, const char* label, bool active );
    void render_arrow( ImGuiDir dir, float sz );
    void render_gradient( ImVec2 start, ImVec2 end, ImColor col1, ImColor col2, ImColor col3, ImColor col4, float rounding );
    void info_item( const char* title, const char* value, ImColor col = ImGui::GetColorU32( ImGuiCol_SchemeLight ) );
    void spacing( float px );
    void update_item( const char* version, const char* date, std::vector< const char* > logs );
    void exit_btn( );

    inline std::vector< std::function< void( ) > > pages;
    void handle_anim( );
    void render_page( );
}
