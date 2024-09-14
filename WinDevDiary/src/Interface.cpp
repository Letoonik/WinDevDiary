#include "imgui.h"
#include <fstream>
#include <GLFW/glfw3.h>

#pragma warning(suppress : 6387)


using namespace ImGui;

namespace WDDInterface
{
	float f = 0.0f;
	float fdisplay = 0.0f;
	static char InputedText[128000] = "Hello, world!";
	int wWindow, hWindow, wPosWindow, hPosWindow;

	void getOpenGLWindowSizePos(GLFWwindow* windowToResize) 
	{
		glfwGetFramebufferSize(windowToResize, &wWindow, &hWindow);
		glfwGetWindowPos(windowToResize, &wPosWindow, &hPosWindow);
	}

	void updateWindowSizePos(GLFWwindow* windowToResize, int posX, int posY, float proportion)
	{
		getOpenGLWindowSizePos(windowToResize);

		float temp = (float) wWindow / 1270.0f;

		ImGui::SetWindowSize(ImVec2(wWindow * proportion, hWindow), ImGuiCond_Always);
		ImGui::SetWindowPos(ImVec2(wPosWindow + (posX * temp), hPosWindow + posY), ImGuiCond_Always);
	}

	void displaySidebar(GLFWwindow* windowToResize)
	{
		Begin("Sidebar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		updateWindowSizePos(windowToResize, 0, 0, 0.25);

		Text("%f", fdisplay);
		if (Button("+"))
			fdisplay += f;
		if (Button("-"))
			fdisplay -= f;
		Text("Input your number to add/subtract");
		SetItemTooltip("Was ist das denn");

		SliderFloat(" ", &f, 0.0f, 20.0f);

		End();
	}

	void saveInputedText()
	{
		FILE* out_file = NULL;

		fopen_s(&out_file, "hello.txt", "w");

		fprintf(out_file, InputedText);
	}

	void displayTextArea(GLFWwindow* windowToResize)
	{
		Begin("Textarea", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		updateWindowSizePos(windowToResize, 320, 0, 0.75);

		SeparatorText("Text Area");

		InputTextMultiline(" ", InputedText, IM_ARRAYSIZE(InputedText), ImVec2(GetWindowWidth() * 0.95, 300));

		if (Button("Save"))
			saveInputedText();

		End();
	}
}