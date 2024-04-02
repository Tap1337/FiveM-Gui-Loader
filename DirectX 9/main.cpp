#include <Main.h>

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D( HWND hWnd );
void CleanupDeviceD3D( );
void ResetDevice( );
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

Main GUI;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
    WNDCLASSEXW wc = { sizeof( wc ), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, L"DirectX 9", NULL };
    ::RegisterClassExW( &wc );
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Fivem", WS_POPUP, GetSystemMetrics( SM_CXSCREEN ) / 2 - ui::size.x / 2, GetSystemMetrics( SM_CYSCREEN ) / 2 - ui::size.y / 2, ui::size.x, ui::size.y, NULL, NULL, wc.hInstance, NULL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    SetWindowRgn( hwnd, CreateRoundRectRgn( 0, 0, ui::size.x, ui::size.y, ImGui::GetStyle( ).WindowRounding, ImGui::GetStyle( ).WindowRounding ), true );

    CreateDeviceD3D( hwnd );

    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( );
    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX9_Init( g_pd3dDevice );

    ImGui::StyleColorsDark( );

    GUI.Initialize( io, g_pd3dDevice );

    ::ShowWindow( hwnd, SW_SHOWDEFAULT );
    ::UpdateWindow( hwnd );

    bool done = false;
    while ( !done ) {
        MSG msg;
        while ( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            if ( msg.message == WM_QUIT )
                done = true;
        }
        if ( done )
            break;

        ImGui_ImplDX9_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        RECT rect;
        GetWindowRect( hwnd, &rect );

        if ( ui::moving_window && GImGui->IO.MouseDown[0] ) {
            MoveWindow( hwnd, rect.left + ImGui::GetMouseDragDelta( ).x, rect.top + ImGui::GetMouseDragDelta( ).y, ui::size.x, ui::size.y, TRUE );
        }

        GUI.Render( );

        ImGui::EndFrame( );
        g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
        g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        g_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( int( ImGui::GetStyleColorVec4( ImGuiCol_WindowBg ).x * 255 ), int( ImGui::GetStyleColorVec4( ImGuiCol_WindowBg ).y * 255 ), int( ImGui::GetStyleColorVec4( ImGuiCol_WindowBg ).z * 255 ), 255 );
        g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
        if ( g_pd3dDevice->BeginScene( ) >= 0 )
        {
            ImGui::Render( );
            ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
            g_pd3dDevice->EndScene( );
        }
        HRESULT result = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

        if ( result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
            ResetDevice( );
    }

    ImGui_ImplDX9_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    CleanupDeviceD3D( );
    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
}

bool CreateDeviceD3D( HWND hWnd )
{
    if ( ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
        return false;

    ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if ( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice ) < 0 )
        return false;

    return true;
}

void CleanupDeviceD3D( )
{
    if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
    if ( g_pD3D ) { g_pD3D->Release( ); g_pD3D = NULL; }
}

void ResetDevice( )
{
    ImGui_ImplDX9_InvalidateDeviceObjects( );
    HRESULT hr = g_pd3dDevice->Reset( &g_d3dpp );
    if ( hr == D3DERR_INVALIDCALL )
        IM_ASSERT( 0 );
    ImGui_ImplDX9_CreateDeviceObjects( );
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
        return true;

    switch ( msg )
    {
    case WM_SIZE:
        if ( g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED )
        {
            g_d3dpp.BackBufferWidth = LOWORD( lParam );
            g_d3dpp.BackBufferHeight = HIWORD( lParam );
            ResetDevice( );
        }
        return 0;
    /*case WM_NCHITTEST:
    {
        ImVec2 shit = ImGui::GetMousePos();
        if (shit.y < 50)
        {
            LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
            if (hit == HTCLIENT) hit = HTCAPTION;
            return hit;
        }
        else break;
    }*/
    case WM_SYSCOMMAND:
        if ( ( wParam & 0xfff0 ) == SC_KEYMENU )
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage( 0 );
        return 0;
    }
    return ::DefWindowProc( hWnd, msg, wParam, lParam );
}