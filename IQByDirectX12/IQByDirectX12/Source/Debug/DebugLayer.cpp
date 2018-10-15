#include <iostream>
#include <windows.h>
#include "DebugLayer.h"
#include "../ConvertString.h"



DebugLayer::DebugLayer()
	: mOutputDestination(OutputDestination::Console)
{
	FILE * fp;
	errno_t error;

	error = fopen_s(&fp, OUTPUT_FILE_NAME, "w");
	if (error != 0)
	{
		return;
	}

	fclose(fp);
}


DebugLayer::~DebugLayer()
{
}

void DebugLayer::PrintDebugMessage(const std::string & message)
{
#ifdef _DEBUG
	switch (mOutputDestination)
	{
	case OutputDestination::Console:
		std::cout << message << std::endl;
		break;
	case OutputDestination::Window:
		MessageBox(nullptr, static_cast<LPCSTR>(message.c_str()), static_cast<LPCSTR>("DebugMessage"), MB_OK);
		break;
	case OutputDestination::Text:
		FILE * fp;
		errno_t error;

		error = fopen_s(&fp, OUTPUT_FILE_NAME, "a");
		if (error != 0)
		{
			MessageBox(nullptr, static_cast<LPCSTR>("Failed Open Debug Text File."), static_cast<LPCSTR>("Failed Open File"), MB_OK);
			return;
		}

		fprintf(fp, message.c_str());
		fprintf(fp, "\n");

		fclose(fp);
		break;
	default:
		break;
	}
#endif
}


void DebugLayer::PrintDebugMessage(const std::wstring& message)
{
	PrintDebugMessage(ConvertWStringToString(message));
}

void DebugLayer::SetOutputDestination(OutputDestination outputDestination)
{
	mOutputDestination = outputDestination;
}