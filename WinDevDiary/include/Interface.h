#pragma once

namespace WDDInterface
{
	void getOpenGLWindowSizePos(GLFWwindow* windowToResize);
	void updateWindowSizePos(GLFWwindow* windowToResize, int posX, int posY, float proportion);
	void displaySidebar(GLFWwindow* toResize);
	void displayTextArea(GLFWwindow* toResize);
	void saveInputedText();
}