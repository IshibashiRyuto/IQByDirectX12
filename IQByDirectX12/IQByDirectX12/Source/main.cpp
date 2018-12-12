#include <memory>
#include "Window.h"
#include "Application.h"
#include "Debug/DebugLayer.h"
#include "Font/BMFontData.h"


//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpStr, int nCmdWnd)
int main(void)
{
	std::unique_ptr<Window> window = std::make_unique<Window>(GetModuleHandle(0));
	std::unique_ptr<Application> app = std::make_unique<Application>();

	DebugLayer::GetInstance().SetOutputDestination(OutputDestination::Console);

	BMFont::FontData fontData;

	if (!BMFont::LoadBMFont("Resource/Font/test.fnt", fontData))
	{
		return -1;
	}

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
		}
		app->Render();		
	}
	app->Terminate();

	return 0;
}