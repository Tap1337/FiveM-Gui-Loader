#pragma once

void pages_login( ) { static char login[64], password[64];
	ImGui::SetCursorPos( { ImGui::GetWindowWidth( ) / 2 - ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, "SIGN IN to your account." ).x / 2, 120 } );

	ImGui::BeginGroup( );
	{
		ImGui::GetWindowDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[3], ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, ImGui::GetCurrentWindow( )->DC.CursorPos, ImGui::GetColorU32( ImGuiCol_SchemeLight ), "SIGN IN" );
		ImGui::GetWindowDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[3], ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, { ImGui::GetCurrentWindow( )->DC.CursorPos.x + ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, "SIGN IN " ).x, ImGui::GetCurrentWindow( )->DC.CursorPos.y }, ImGui::GetColorU32( ImGuiCol_Text ), "to your account" );
		ImGui::GetWindowDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[3], ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, { ImGui::GetCurrentWindow( )->DC.CursorPos.x + ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, "SIGN IN to your account" ).x, ImGui::GetCurrentWindow( )->DC.CursorPos.y }, ImGui::GetColorU32( ImGuiCol_SchemeLight ), "." );
	
		ImGui::SetCursorPosY( 160 );
		ImGui::InputTextEx( "##LOGIN", "LOGIN", login, sizeof( login ), { ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, "SIGN IN to your account." ).x, ImGui::GetFrameHeight( ) }, 0, 0, 0, "C" );
		ImGui::InputTextEx( "##PASSWORD", "PASSWORD", password, sizeof( password ), { ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, "SIGN IN to your account." ).x, ImGui::GetFrameHeight( ) }, ImGuiInputTextFlags_Password, 0, 0, "D" );
		
		if ( ImGui::Button( "SUBMIT", { ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, "SIGN IN to your account." ).x, ImGui::GetFrameHeight( ) } ) ) {
			ui::content_anim_dest = 0.f;
			ui::next_page++;
		}
	}
	ImGui::EndGroup( );
}