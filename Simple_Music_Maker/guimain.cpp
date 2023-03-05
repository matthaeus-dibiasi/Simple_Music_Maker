#include "window.h"


int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    Window window;

    return app->run(window);
}
