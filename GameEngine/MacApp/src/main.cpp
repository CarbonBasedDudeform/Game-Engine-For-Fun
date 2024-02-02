#include <Graphics/Window.h>

int main(int argc, char* argv[]) {
    Graphics::Window window {"Mac App Demo", 600, 800};

    window.loop();

    return 0;
}
