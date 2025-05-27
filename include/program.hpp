#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <GLFW/glfw3.h>

extern GLFWwindow* glfw_window;

void opengl_setup();
void imgui_setup();
void imgui_style();
void main_loop();
void finalize();

#endif

