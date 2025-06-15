#include "imgui.h"
#include "diaryIO.hpp"
#include "imgui_stdlib.h"
#include <string>
#include <GLFW/glfw3.h>
#include "nlohmann/json.hpp"
#include "ui.hpp"

using json = nlohmann::json;

extern bool isEncrypted = true;
extern std::string password;
bool loadDiary = true; // todo: remove later

namespace WDDInterface
{
	std::string InputedText;
	std::string InputedTitle;

	void displaySidebar()
	{
		json entries = WDDio::loadDiaryEntries();
		int entriesSize = entries["entries"].size();

		ImGui::Begin("Sidebar", nullptr);

		ImGui::Text("Entries");
		bool date = true;
		// Render Sidebar entries
		if (ImGui::BeginTable("entriesTable", 2, ImGuiTableFlags_Borders))
		{
			for (int row = 0; row < entriesSize; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 2; column++)
				{
					ImGui::TableSetColumnIndex(column);

					if (column == 0) {
						std::string e = entries["entries"][row]["title"];
						if (ImGui::Button(e.c_str())) {
							InputedText = WDDio::loadEntry(row, entries);
							InputedTitle = e;
						}
					}
					if (column == 1) {
						std::string e = entries["entries"][row]["date"];
						ImGui::Text(e.c_str());
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}

	void displayTextArea()
	{
		ImGui::Begin("Textarea", nullptr);

		ImGui::SeparatorText("Title");
		ImGui::InputText("##xx", &InputedTitle);

		ImGui::SeparatorText("Text Area");
		ImGui::InputTextMultiline("##xxx", &InputedText, ImVec2(ImGui::GetWindowWidth() * 0.95, ImGui::GetWindowHeight() * 0.6));

		if (ImGui::Button("Save"))
			WDDio::saveDiaryEntry(InputedText, InputedTitle);

		if (ImGui::Button("New"))
			WDDio::saveDiaryEntry(InputedText, InputedTitle);
		
		ImGui::Checkbox("Encryption", &isEncrypted);
		isEncrypted = false; //TODO: Make encryption (it is disabled for now)

		if(isEncrypted)
			ImGui::InputText("Password", &password, ImGuiInputTextFlags_Password);
		ImGui::End();
	}
}