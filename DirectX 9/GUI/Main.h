#include <d3d9.h>
#include <d3dx9.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <image.hpp>
#include <font.h>
#include <gui.h>
#include <blur.hpp>
#include <imspinner.h>

inline IDirect3DTexture9* logo;
inline IDirect3DTexture9* logo_big;
inline IDirect3DTexture9* banner;
inline LPDIRECT3DDEVICE9 device;

#include "pages/login.hpp"
#include "pages/home.hpp"
#include "pages/injecting.hpp"
#include "pages/loading.hpp"

class Main {
private:

public:
	void Initialize( ImGuiIO& io, LPDIRECT3DDEVICE9 _device ) {
		device = _device;

		io.Fonts->AddFontFromMemoryTTF( ( void* )font_binary, sizeof( font_binary ), 16.f );
		io.Fonts->AddFontFromMemoryTTF( ( void* )font_binary, sizeof( font_binary ), 14.f );
		io.Fonts->AddFontFromMemoryTTF( ( void* )icons_binary, sizeof( icons_binary ), 14.f );
		io.Fonts->AddFontFromMemoryTTF( ( void* )font_binary, sizeof( font_binary ), 20.f );

		 D3DXCreateTextureFromFileInMemory( _device, ( void* )logo_binary, sizeof( logo_binary ), &logo );  // If you want to change logo its here.
		 D3DXCreateTextureFromFileInMemory( _device, ( void* )logo_big_binary, sizeof( logo_big_binary ), &logo_big ); // If you want to change logo its here.
		 D3DXCreateTextureFromFileInMemory( _device, ( void* )banner_binary, sizeof( banner_binary ), &banner );

		ui::pages.emplace_back( pages_login );
		ui::pages.emplace_back( pages_loading );
		ui::pages.emplace_back( pages_home );
		ui::pages.emplace_back( pages_injecting );
	}

	void Render( ) {
		ImGui::SetNextWindowSize( ui::size );
		ImGui::SetNextWindowPos( { 0, 0 } );
		ImGui::Begin( "UI", 0, ImGuiWindowFlags_NoDecoration );
		{
			ui::render_background( );

			ImGui::BeginChild( "header", { -1, 40 }, 0, ImGuiWindowFlags_NoBackground );
			{
				ImGui::GetWindowDrawList( )->AddImage( logo, { 16, ImGui::GetWindowHeight( ) / 2 - 10 }, { 36, ImGui::GetWindowHeight( ) / 2 + 10 } );
				ImGui::GetWindowDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[1], ImGui::GetIO( ).Fonts->Fonts[1]->FontSize, { 45, ImGui::GetWindowHeight( ) / 2 - ImGui::GetIO( ).Fonts->Fonts[1]->FontSize / 2 }, ImGui::GetColorU32( ImGuiCol_Text ), "Fiv" );
				ImGui::GetWindowDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[1], ImGui::GetIO( ).Fonts->Fonts[1]->FontSize, { 45 + ImGui::GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, -1, "Fiv" ).x, ImGui::GetWindowHeight( ) / 2 - ImGui::GetIO( ).Fonts->Fonts[1]->FontSize / 2 }, ImGui::GetColorU32( ImGuiCol_Scheme ), "eM" );
			
				ImGui::SetCursorPos( { ImGui::GetWindowWidth( ) - ( ImGui::GetWindowHeight( ) / 2 - 10 ) - 20, ImGui::GetWindowHeight( ) / 2 - 10 } );
				ui::exit_btn( );
			}
			ImGui::EndChild( );

			ImGui::GetCurrentWindow( )->DC.CursorPos.y -= ImGui::GetStyle( ).ItemSpacing.y;
			ImGui::PushStyleVar( ImGuiStyleVar_ChildRounding, 0.f );
			ImGui::BeginChild( "header-2", { -1, 40 } );
			{
				ImGui::SetCursorPosX( 20 );
				ui::render_step( "A", "SIGN IN", ui::cur_page == 0 );
				ImGui::SameLine( 0, 17 );
				ui::render_arrow( ImGuiDir_Right, 6.f );
				ImGui::SameLine( 0, 11 );
				ui::render_step( "B", "HOME", ui::cur_page == 2 );
			}
			ImGui::EndChild( );
			ImGui::PopStyleVar( );

			ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ui::content_anim );

			ui::render_page( );

			ImGui::PopStyleVar( );
		}
		ImGui::End( );

		ui::handle_anim( );
	}
};