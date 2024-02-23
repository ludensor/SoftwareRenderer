#include "Device.h"

#include <format>

constexpr int32_t WIN_WIDTH = 1600;
constexpr int32_t WIN_HEIGHT = 900;

struct VertexData
{
	sr::math::Vector3 position;
	sr::math::Vector4 color;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = TEXT("SampleWindowClass");
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	RECT rc{ 0, 0, WIN_WIDTH, WIN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	HWND hWnd = CreateWindow(wc.lpszClassName, TEXT("Software Renderer"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	std::unique_ptr<sr::Device> device = std::make_unique<sr::Device>(hWnd, WIN_WIDTH, WIN_HEIGHT);
	if (!device->StartUp())
	{
		PostQuitMessage(1);
	}

	LARGE_INTEGER prev_time, current_time;
	QueryPerformanceCounter(&prev_time);

	LARGE_INTEGER cpu_tick;
	QueryPerformanceFrequency(&cpu_tick);

	float elapsed_time = 0.0f;
	int32_t frame_count = 0;

	constexpr VertexData vertices[]
	{
		{ sr::math::Vector3{ 0.0f, 0.0f, 0.0f }, sr::math::Vector4{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ sr::math::Vector3{ 1600.0f, 0.0f, 0.0f }, sr::math::Vector4{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ sr::math::Vector3{ 0.0f, 900.0f, 0.0f }, sr::math::Vector4{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	uint32_t byte_width = sizeof(vertices);

	std::shared_ptr<sr::Buffer> vertex_buffer = device->CreateBuffer(vertices, byte_width);

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
			QueryPerformanceCounter(&current_time);

			const float delta_time = (current_time.QuadPart - prev_time.QuadPart) / (float)cpu_tick.QuadPart;

			++frame_count;
			elapsed_time += delta_time;
			if (elapsed_time >= 1.0f)
			{
				const float fps = (float)frame_count;
				const float mspf = 1000.0f / fps;

				const std::wstring title = std::format(TEXT("{0}    fps: {1:0.2f}    mspf: {2:f}"), TEXT("Software Renderer"), fps, mspf);
				SetWindowText(hWnd, title.c_str());

				frame_count = 0;
				elapsed_time = 0.0f;
			}

			prev_time = current_time;

			constexpr sr::Vertex point_a{ sr::math::Vector4{+0.0f, +0.5f, 0.0f, 1.0f}, 1.0f, 0.0f, 0.0f, 1.0f };
			constexpr sr::Vertex point_b{ sr::math::Vector4{+0.5f, -0.5f, 0.0f, 1.0f}, 0.0f, 1.0f, 0.0f, 1.0f };
			constexpr sr::Vertex point_c{ sr::math::Vector4{-0.5f, -0.5f, 0.0f, 1.0f}, 0.0f, 0.0f, 1.0f, 1.0f };
			constexpr sr::Vertex point_d{ sr::math::Vector4{+1.0f, -1.0f, 0.0f, 1.0f}, 1.0f, 0.0f, 1.0f, 1.0f };

			device->Clear(sr::math::Vector4{ 0.2f, 0.2f, 0.2f, 1.0f });
			device->Draw(point_a, point_b, point_c);
			//device->Draw(point_b, point_d, point_c);
			device->Present();
		}
	}

	device->ShutDown();
	UnregisterClass(wc.lpszClassName, hInstance);

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

