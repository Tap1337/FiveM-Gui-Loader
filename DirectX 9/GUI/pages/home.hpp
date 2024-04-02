#pragma once

void pages_home( ) {
	ImGui::SetCursorPos( { 24, 104 } );
	ImGui::BeginChild( "updates", { 224, -1 }, 0, ImGuiWindowFlags_NoBackground );
	{
		ui::update_item( "1.3", "26.09", { "FIXED BUGS", "FIXED BUGS" } );
		ui::update_item( "1.2", "23.09", { "FIXED BUGS" } );
		ui::update_item( "1.1", "219.09", { "FIXED BUGS", "FIXED BUGS" } );
		
		ImGui::Dummy( { 0, 24.f - ImGui::GetStyle( ).ItemSpacing.y } );
	}
	ImGui::EndChild( );
	ImGui::SameLine( 0, 10 );
	ImGui::BeginChild( "game", { 218, -24 } );
	{
		ImGui::SetCursorPos( { 19, 24 } );

		ImGui::BeginGroup( );
		{
			ImGui::Image( banner, { 178, 104 }, ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImColor( 1.f, 1.f, 1.f, 1.f ), ImColor( 1.f, 1.f, 1.f, 0.f ), 6.f );

			ui::spacing( 20.f );

			ui::info_item( "STATUS", "UNDETECTED", { 157, 217, 30 } );
			ui::spacing( 14.f );
			ui::info_item( "SUB", "1.04.2024" );

			ui::spacing( 20.f );

			ImGui::SetCursorPosX( 0 );
			ImGui::BeginChild( "btn", { -1, -1 }, 0, ImGuiWindowFlags_NoBackground );
			{
				ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), { ImGui::GetWindowPos( ).x + ImGui::GetWindowSize( ).x, ImGui::GetWindowPos( ).y + ImGui::GetWindowSize( ).y }, ImColor{ 1.f, 1.f, 1.f, 0.035f * ImGui::GetStyle( ).Alpha }, ImGui::GetStyle( ).ChildRounding, ImDrawFlags_RoundCornersBottom );
			
				ImGui::SetCursorPos( { 10, 10 } );

				if ( ImGui::Button( "INJECT", { ImGui::GetWindowWidth( ) - 20, ImGui::GetWindowHeight( ) - 20 } ) ) {
					ui::cur_page++;
				}
			}
			ImGui::EndChild( );
		}
		ImGui::EndGroup( );
	}
	ImGui::EndChild( );
}