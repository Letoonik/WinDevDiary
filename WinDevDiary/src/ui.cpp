#include "imgui.h"
#include "diaryIO.hpp"
#include "imgui_stdlib.h"
#include <string>
#include <GLFW/glfw3.h>


using namespace ImGui;

extern bool isEncrypted = true;
extern std::string password;

namespace WDDInterface
{
	float f = 0.0f;
	float fdisplay = 0.0f;
	std::string InputedText;
	int widthWindow, heightWindow, xPosWindow, yPosWindow;


	void getOpenGLWindowSizePos(GLFWwindow* windowToResize) 
	{
		glfwGetFramebufferSize(windowToResize, &widthWindow, &heightWindow);
		glfwGetWindowPos(windowToResize, &xPosWindow, &yPosWindow);
	}

	void updateWindowSizePos(GLFWwindow* windowToResize, int posX, int posY, float proportion)
	{
		getOpenGLWindowSizePos(windowToResize);

		ImGui::SetWindowSize(ImVec2(widthWindow * proportion, heightWindow), ImGuiCond_Always);
		ImGui::SetWindowPos(ImVec2(xPosWindow + posX * widthWindow / 1280, yPosWindow + posY), ImGuiCond_Always);
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

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f);

		End();
	}

	void displayTextArea(GLFWwindow* windowToResize)
	{
		Begin("Textarea", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		updateWindowSizePos(windowToResize, 320, 0, 0.75);

		SeparatorText("Text Area");

		InputTextMultiline(" ", &InputedText, ImVec2(GetWindowWidth() * 0.95, GetWindowHeight() * 0.6));

		if (Button("Save"))
			WDDsave::saveDiaryEntry(InputedText);

		if (Button("Load"))
			InputedText = WDDsave::loadDiaryEntry();
		
		Checkbox("Encryption", &isEncrypted);

		if(isEncrypted)
			InputText("Password", &password, ImGuiInputTextFlags_Password);


		End();
	}
}