#include <memory>
#include "Window.h"
#include "Application.h"


//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpStr, int nCmdWnd)
int main(void)
{
	std::unique_ptr<Window> window = std::make_unique<Window>(GetModuleHandle(0));
	std::unique_ptr<Application> app = std::make_unique<Application>();

	if (!app->Initialize(*window.get()))
	{
		return -1;
	}

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				app->Render();
			}
		}
	}

	return 0;
}