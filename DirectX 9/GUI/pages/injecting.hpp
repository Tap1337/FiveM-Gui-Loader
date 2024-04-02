#pragma once

void pages_injecting( ) {
	ImGui::PushItemFlag( ImGuiItemFlags_Disabled, 1 );
	pages_home( );
	ImGui::PopItemFlag( );
	
	static bool injected = false;
	static float anim_dest = 1.f, anim = 0.f; anim = ImLerp( anim, anim_dest, ui::anim_speed );

	if ( ImGui::GetIO( ).MouseReleased[0] ) anim_dest = 0.f;

	if ( anim < 0.0001f && anim_dest == 0.f ) {
		anim_dest = 1.f;
		injected = true;
	}

	DrawBackgroundBlur( ImGui::GetForegroundDrawList( ), { 0, 0 }, ui::size, 0, 0, device, anim );

	ImGui::GetForegroundDrawList( )->AddImage( logo_big, { ImGui::GetWindowWidth( ) / 2 - 40, ImGui::GetWindowHeight( ) / 2 - 40 }, { ImGui::GetWindowWidth( ) / 2 + 40, ImGui::GetWindowHeight( ) / 2 + 40 }, { 0, 0 }, { 1, 1 }, ImColor{ 1.f, 1.f, 1.f, 1.f } );
	ImGui::GetForegroundDrawList( )->AddText( ImGui::GetIO( ).Fonts->Fonts[3], ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, { ImGui::GetWindowWidth( ) / 2 - ImGui::GetIO( ).Fonts->Fonts[3]->CalcTextSizeA( ImGui::GetIO( ).Fonts->Fonts[3]->FontSize, FLT_MAX, -1, injected ? "INJECTED." : "INJECTING..." ).x / 2, ImGui::GetWindowHeight( ) / 2 + 107 - 40 * anim }, ImGui::GetColorU32( ImGuiCol_Text, anim ), injected ? "INJECTED." : "INJECTING..." );
}