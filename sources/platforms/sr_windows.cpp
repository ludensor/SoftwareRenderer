#include "sr_pch.h"
#include "core/sr_application.h"
#include "core/sr_graphic_device.h"
#include <windows.h>

LPCWSTR WindowClassName = TEXT("SoftwareRendererClass");
LPCWSTR WindowTitleName = TEXT("Software Renderer");

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

float GetSecondsPerCycle();
uint32_t GetCycles();

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtDumpMemoryLeaks();

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WindowClassName;
	wcex.hIconSm = nullptr;
	RegisterClassEx(&wcex);

	Application application;
	const GraphicDevice& graphic_device = application.GetGraphicDevice();
	const int32_t screen_width = graphic_device.GetWidth();
	const int32_t screen_height = graphic_device.GetHeight();
	const int32_t num_pixels = screen_width * screen_height;

	RECT window_rect{ 0, 0, screen_width, screen_height };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);
	const int32_t width = window_rect.right - window_rect.left;
	const int32_t height = window_rect.bottom - window_rect.top;
	const int32_t pos_x = (GetSystemMetrics(SM_CXFULLSCREEN) - width) >> 1;
	const int32_t pos_y = (GetSystemMetrics(SM_CYFULLSCREEN) - height) >> 1;

	constexpr uint32_t style = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindow(WindowClassName, WindowTitleName, style, pos_x, pos_y, width, height, nullptr, nullptr, hInstance, nullptr);
	SR_ASSERT(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RECT cr{};
	GetClientRect(hWnd, &cr);

	// Initlaize application
	application.Initialize();

	HDC screen_dc = GetDC(hWnd);
	SR_ASSERT(screen_dc);

	HDC memory_dc = CreateCompatibleDC(screen_dc);
	SR_ASSERT(memory_dc);

	BITMAPINFO bitmap_info{};
	bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap_info.bmiHeader.biWidth = screen_width;
	bitmap_info.bmiHeader.biHeight = -screen_height; // top -> down
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;

	// low dynamic range
	uint8_t* ldr_buffer = nullptr;
	HBITMAP dit_bitmap = CreateDIBSection(memory_dc, &bitmap_info, DIB_RGB_COLORS, reinterpret_cast<void**>(&ldr_buffer), nullptr, 0);
	SR_ASSERT(dit_bitmap);

	HBITMAP old_bitmap = (HBITMAP)SelectObject(memory_dc, dit_bitmap);
	SR_ASSERT(old_bitmap);

	// Set background mode to transparent
	SetBkMode(memory_dc, TRANSPARENT);

	// Set text color
	SetTextColor(memory_dc, RGB(255, 255, 255));

	const float seconds_per_cycle = GetSecondsPerCycle();
	uint32_t prev_cycles = GetCycles();

	uint32_t c = 0xff005511;
	uint8_t* d = (uint8_t*)(&c);
	uint8_t a = d[0];
	a = d[1];
	a = d[2];
	a = d[3];

	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			const uint32_t current_cycles = GetCycles();
			const float delta_time = static_cast<float>(current_cycles - prev_cycles) * seconds_per_cycle;
			prev_cycles = current_cycles;

			application.Tick(delta_time);

			uint8_t* pixel_buffer = graphic_device.GetPixelBuffer();
			for (int32_t i = 0; i < num_pixels; ++i)
			{
				const int32_t index = i * 4;
				ldr_buffer[index + 0] = pixel_buffer[index + 2]; // blue
				ldr_buffer[index + 1] = pixel_buffer[index + 1]; // green
				ldr_buffer[index + 2] = pixel_buffer[index + 0]; // red
			}

			for (const auto& info : application.GetDebugInfos())
			{
				TextOut(memory_dc, info.position.x, info.position.y, info.text.c_str(), static_cast<int32_t>(info.text.length()));
			}

			// Present surface
			BitBlt(screen_dc, 0, 0, screen_width, screen_height, memory_dc, 0, 0, SRCCOPY);
		}
	}

	application.Finalize();

	SelectObject(memory_dc, old_bitmap);
	DeleteObject(dit_bitmap);
	DeleteDC(memory_dc);
	ReleaseDC(hWnd, screen_dc);

	UnregisterClass(WindowClassName, hInstance);
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostMessage(hWnd, WM_DESTROY, 0, 0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

float GetSecondsPerCycle()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return 1.0f / static_cast<float>(frequency.QuadPart);
}

uint32_t GetCycles()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);
	return static_cast<uint32_t>(cycles.QuadPart);
}
