#pragma once

namespace WDDInterface
{
	void updateWindowSizePos(GLFWwindow* windowToResize, int posX, int posY, float proportion);
	void displaySidebar(GLFWwindow* toResize);
	void displayTextArea(GLFWwindow* toResize);
}