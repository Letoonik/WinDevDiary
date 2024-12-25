#include "imgui.h"
#include "diaryIO.hpp"
#include "imgui_stdlib.h"
#include <string>
#include <GLFW/glfw3.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace ImGui;

extern bool isEncrypted = true;
extern std::string password;
bool loadDiary = true; // todo: remove later

namespace WDDInterface
{
	float f = 0.0f;
	float fdisplay = 0.0f;
	std::string InputedText;
	std::string InputedTitle;
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
		json entries = WDDio::loadDiaryEntries();
		int entriesSize = entries["entries"].size();

		Begin("Sidebar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		updateWindowSizePos(windowToResize, 0, 0, 0.25);
		Text("Entries");
		bool date = true;

		if (BeginTable("table1", 3, ImGuiTableFlags_Borders))
		{
			for (int row = 0; row < entriesSize; row++)
			{
				TableNextRow();
				for (int column = 0; column < 2; column++)
				{
					TableSetColumnIndex(column);

					if (column == 0) {
						std::string e = entries["entries"][row]["title"];
						Button(e.c_str());
					}
					if (column == 1) {
						std::string e = entries["entries"][row]["date"];
						Text(e.c_str());
					}
				}
			}
			EndTable();
		}
		//Text("%f", fdisplay);
		//if (Button("+"))
		//	fdisplay += f;
		//if (Button("-"))
		//	fdisplay -= f;
		//Text("Input your number to add/subtract");
		//SetItemTooltip("Was ist das denn");

		//SliderFloat(" ", &f, 0.0f, 20.0f);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f);

		End();
	}

	void displayTextArea(GLFWwindow* windowToResize)
	{
		Begin("Textarea", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		updateWindowSizePos(windowToResize, 320, 0, 0.75);

		SeparatorText("Title");
		InputText("##xx", &InputedTitle);

		SeparatorText("Text Area");
		InputTextMultiline("##xxx", &InputedText, ImVec2(GetWindowWidth() * 0.95, GetWindowHeight() * 0.6));

		if (Button("Save"))
			WDDio::saveDiaryEntry(InputedText, InputedTitle);
		
		Checkbox("Encryption", &isEncrypted);
		isEncrypted = false;
		if(isEncrypted)
			InputText("Password", &password, ImGuiInputTextFlags_Password);
		End();
	}
}