#include <gui_vars.hpp>
#include <program.hpp>
#include <stdio.h>

int main() {
	opengl_setup();
    if (!GuiVars::glfw_window) {
	    fprintf(stderr, "opengl setup failed\n");
        return 1;
    }

	imgui_setup();
    main_loop();
    finalize();
}

