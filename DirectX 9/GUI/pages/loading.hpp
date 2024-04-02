#pragma once

void pages_loading( ) {
	static const char* result;
	static float anim = 0.f; anim = ImLerp( anim, result ? 1.f : 0.f, ui::anim_speed );

	ImGui::GetForegroundDrawList( )->AddRectFilled( { 0, 0 }, ui::size, ImGui::GetColorU32( ImGuiCol_WindowBg ) );

	ImGui::SetCursorPos( { ImGui::GetWindowWidth( ) / 2 - 34, ImGui::GetWindowHeight( ) / 2 - 34 } );
	ImSpinner::SpinnerFadeBars( "Spinner", 12, ImGui::GetColorU32( ImGuiCol_Scheme ), 9.f, 3, true );

	if ( ImGui::GetIO( ).MouseReleased[0] ) {
		if ( !result ) {
			result = "SUCCESS";
		} else {
			ui::content_anim_dest = 0.f;
			ui::next_page++;
		}
	}

	if ( result ) {
		ImGui::GetForegroundDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[3], ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, { ImGui::GetWindowWidth( ) / 2 - ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, result ).x / 2, ImGui::GetWindowHeight( ) / 2 + 64 - 10 * anim }, ImGui::GetColorU32( ImGuiCol_Text, anim ), result );
	}
}