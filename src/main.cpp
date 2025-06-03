#include <SDL.h>
#include <SDL_mixer.h>
#include <gui_vars.hpp>
#include <program.hpp>
#include <stdio.h>

int main() {
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 1024);

	opengl_setup();
    if (!GuiVars::glfw_window) {
	    fprintf(stderr, "opengl setup failed\n");
        return 1;
    }

	imgui_setup();
    main_loop();
    finalize();
    
    Mix_FreeChunk(bad_apple_music);
    Mix_CloseAudio();
    SDL_Quit();
}

