#include "gui.h"

using namespace ImGui;

int rotation_start_index;
void ui::rotate_start( )
{
    rotation_start_index = GetWindowDrawList( )->VtxBuffer.Size;
}

ImVec2 ui::rotation_center( )
{
    ImVec2 l( FLT_MAX, FLT_MAX ), u( -FLT_MAX, -FLT_MAX );

    const auto& buf = GetWindowDrawList( )->VtxBuffer;
    for ( int i = rotation_start_index; i < buf.Size; i++ )
        l = ImMin( l, buf[i].pos ), u = ImMax( u, buf[i].pos );

    return ImVec2( ( l.x + u.x ) / 2, ( l.y + u.y ) / 2 );
}

void ui::rotate_end( float rad, ImVec2 center )
{
    float s = sin( rad ), c = cos( rad );
    center = ImRotate( center, s, c) - center;

    auto& buf = GetWindowDrawList()->VtxBuffer;
    for ( int i = rotation_start_index; i < buf.Size; i++ )
        buf[i].pos = ImRotate( buf[i].pos, s, c ) - center;
}

void ui::render_step( const char* icon, const char* label, bool active ) {
	auto window = GetCurrentWindow( );
	
	ImVec2 icon_size{ GetIO( ).Fonts->Fonts[2]->CalcTextSizeA( GetIO( ).Fonts->Fonts[2]->FontSize, FLT_MAX, -1, icon ) };
	ImVec2 label_size{ GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, -1, label ) };

	ImVec2 size{ icon_size.x + 11 + label_size.x, window->Size.y };
	ImRect bb{ window->DC.CursorPos, window->DC.CursorPos + size };

	ItemSize( bb );
	ItemAdd( bb, 0 );

	float anim = ImTricks::Animations::FastFloatLerp( label, 0, active, 0.f, 1.f, anim_speed );
	auto icon_col = ImTricks::Animations::FastColorLerp( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_Scheme ), anim );
	auto label_col = ImTricks::Animations::FastColorLerp( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_Text ), anim );

    circle_glow( { bb.Min.x + icon_size.x / 2 - 2, bb.GetCenter( ).y }, 0, GetColorU32( ImGuiCol_Scheme, 0.2f * anim ), 4.f );

	window->DrawList->AddText( GetIO( ).Fonts->Fonts[2], GetIO( ).Fonts->Fonts[2]->FontSize, { bb.Min.x, bb.GetCenter( ).y - icon_size.y / 2 }, icon_col, icon );
	window->DrawList->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, { bb.Min.x + icon_size.x + 11, bb.GetCenter( ).y - label_size.y / 2 }, label_col, label );
}

void ui::render_arrow( ImGuiDir dir, float sz ) {
	auto window = GetCurrentWindow( );

	ImVec2 size{ sz, window->Size.y };
	ImRect bb{ window->DC.CursorPos, window->DC.CursorPos + size };

	ItemSize( bb );
	ItemAdd( bb, 0 );

	RenderArrow( window->DrawList, { bb.Min.x, bb.GetCenter( ).y - sz / 3.f }, GetColorU32( ImGuiCol_TextDisabled ), dir, sz );
}

void ui::circle_glow( ImVec2 center, float radius, ImColor col, float intensity ) {
    while ( true ) {
        if ( col.Value.w < 0.0019f )
            break;

        GetWindowDrawList( )->AddCircleFilled( center, radius, col, 24 );

        col.Value.w -= col.Value.w / intensity;
        radius += 2;
    }
}

void ui::rect_glow( ImDrawList* draw, ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity ) {
    while ( true ) {
        if ( col.Value.w < 0.0019f )
            break;

        draw->AddRectFilled( start, end, col, rounding );

        col.Value.w -= col.Value.w / intensity;
        start = ImVec2( start.x - 1, start.y - 1 );
        end = ImVec2( end.x + 1, end.y + 1 );
    }
}

void ui::render_background( ) {
    auto d = GetWindowDrawList( );
    ImVec2 center = { GetCurrentWindow( )->Rect( ).GetCenter( ).x, GetCurrentWindow( )->Rect( ).Max.y + 20 };

    d->AddCircle( center, 50, GetColorU32( ImGuiCol_Scheme, 0.3f ), 0, 2 );
    d->AddCircle( center, 100, GetColorU32( ImGuiCol_Scheme, 0.2f ), 0, 2 );
    d->AddCircle( center, 180, GetColorU32( ImGuiCol_Scheme, 0.1f ), 0, 2 );
    d->AddCircle( center, 260, GetColorU32( ImGuiCol_Scheme, 0.075f ), 0, 2 );
}

void ui::render_gradient( ImVec2 start, ImVec2 end, ImColor col1, ImColor col2, ImColor col3, ImColor col4, float rounding ) {
    auto window = GetCurrentWindow( );
    auto d = window->DrawList;

    d->PushClipRect( start, start + rounding );
    d->AddCircleFilled( start + rounding, rounding, col1 );
    d->PopClipRect( );

    d->PushClipRect( { start.x, end.y - rounding }, { start.x + rounding, end.y } );
    d->AddCircleFilled( { start.x + rounding, end.y - rounding }, rounding, col4 );
    d->PopClipRect( );

    d->PushClipRect( { end.x - rounding, start.y }, { end.x, start.y + rounding } );
    d->AddCircleFilled( { end.x - rounding, start.y + rounding }, rounding, col2 );
    d->PopClipRect( );

    d->PushClipRect( end - rounding, end );
    d->AddCircleFilled( end - rounding, rounding, col3 );
    d->PopClipRect( );

    d->AddRectFilledMultiColor( { start.x, start.y + rounding }, { start.x + rounding, end.y - rounding }, col1, col1, col4, col4 );
    d->AddRectFilledMultiColor( { end.x - rounding, start.y + rounding }, { end.x, end.y - rounding }, col2, col2, col3, col3 );
    d->AddRectFilledMultiColor( { start.x + rounding, start.y }, { end.x - rounding, start.y + rounding }, col1, col2, col2, col1 );
    d->AddRectFilledMultiColor( { start.x + rounding, end.y - rounding }, { end.x - rounding, end.y }, col4, col3, col3, col4 );

    d->AddRectFilledMultiColor( start + rounding, end - rounding, col1, col2, col3, col4 );
}

void ui::handle_anim( ) {
    content_anim = ImLerp( content_anim, content_anim_dest, anim_speed );

    if ( content_anim < 0.0001f ) {
        content_anim_dest = 1.f;
        cur_page = next_page;
    }
}

void ui::render_page( ) {
    pages[cur_page]( );
}

void ui::info_item( const char* title, const char* value, ImColor col ) {
    auto window = GetCurrentWindow( );

    ImVec2 size{ window->Size.x - ( window->DC.CursorPos.x - window->Pos.x ) * 2, GImGui->FontSize };
    ImRect bb{ window->DC.CursorPos, window->DC.CursorPos + size };

	ItemSize( bb );
	ItemAdd( bb, 0 );

    RenderText( bb.Min, title );

    ui::rect_glow( window->DrawList, { bb.Max.x - CalcTextSize( value ).x, bb.Min.y + 2 }, bb.Max - ImVec2{ 0, 2 }, { col.Value.x, col.Value.y, col.Value.z, GetStyle( ).Alpha * 0.05f }, 10, 4.f );

    PushStyleColor( ImGuiCol_Text, col.Value );
    RenderText( { bb.Max.x - CalcTextSize( value ).x, bb.Min.y }, value );
    PopStyleColor( );
}

void ui::spacing( float px ) {
    GetCurrentWindow( )->DC.CursorPos.y += px - GetStyle( ).ItemSpacing.y;
}

void ui::update_item( const char* version, const char* date, std::vector< const char* > logs ) {
    auto window = GetCurrentWindow( );
    
    ImVec2 size{ window->Size.x - 14, 56 + GImGui->FontSize * logs.size( ) + 15 * ( logs.size( ) - 1 ) };
    ImRect bb{ window->DC.CursorPos, window->DC.CursorPos + size };

	ItemSize( bb );
	ItemAdd( bb, 0 );

    window->DrawList->AddRectFilled( bb.Min, bb.Max, GetColorU32( ImGuiCol_ChildBg ), GetStyle( ).ChildRounding );

    window->DrawList->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, bb.Min + ImVec2{ 15, 12 }, GetColorU32( ImGuiCol_TextDisabled ), std::string( "UPDATE " ).append( version ).c_str( ) );
    window->DrawList->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, bb.Min + ImVec2{ bb.GetWidth( ) - 15 - GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, -1, date ).x, 12 }, GetColorU32( ImGuiCol_SchemeLight ), date );

    for ( int i = 0; i < logs.size( ); ++i ) {
        window->DrawList->AddCircleFilled( { bb.Min.x + 15, bb.Min.y + 41 + ( GetIO( ).Fonts->Fonts[1]->FontSize + 14 ) * i + GetIO( ).Fonts->Fonts[1]->FontSize / 2 }, 1.5f, GetColorU32( ImGuiCol_Scheme ) );
        window->DrawList->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, { bb.Min.x + 25, bb.Min.y + 41 + ( GetIO( ).Fonts->Fonts[1]->FontSize + 14 ) * i }, GetColorU32( ImGuiCol_Text ), logs[i] );
    }
}

void draw_cross( ImVec2 center, ImColor color, float size ) {
    float thickness = size / 5.f;
    auto draw_list = GetWindowDrawList( );

    ui::rotate_start( );

    draw_list->AddLine( center - ImVec2( 0, 5 * size ), center + ImVec2( 0, 5 * size ), color, thickness );
    draw_list->AddLine( center - ImVec2( 5 * size, 0 ), center + ImVec2( 5 * size, 0 ), color, thickness );

    ui::rotate_end( IM_PI / 4, ui::rotation_center( ) );
}

void ui::exit_btn( ) {
    auto window = GetCurrentWindow( );
    auto id = window->GetID( "exit_btn" );
    
    ImVec2 size{ 20, 20 };
    ImRect bb{ window->DC.CursorPos, window->DC.CursorPos + size };

	ItemSize( bb );
	ItemAdd( bb, id );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    if ( pressed ) exit( 0 );

    float anim = ImTricks::Animations::FastFloatLerp( "exit_btn", 0, hovered || held, 0.f, 1.f, anim_speed );
    auto col = ImTricks::Animations::FastColorLerp( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_Scheme ), anim );

    draw_cross( bb.GetCenter( ), col, 1.25f );
}