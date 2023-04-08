#ifndef GTKMM_WINDOW_H
#define GTKMM_WINDOW_H

#include <gtkmm.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include <vector>

#include "drawingarea.h"


class Window : public Gtk::Window
{
    public:
        Window();
        virtual ~Window();

    private:
        static const pa_sample_spec ss;
        int error;

        pa_simple *s = NULL;

        std::vector<uint8_t> buf;

        void pulse_audio_init();

    protected:
        // Signal handlers
        void on_btn_play_sound();

        // Child widgets
        Gtk::Box box_parent;
        Gtk::Box box_drawingarea;
        Gtk::Box box_play_sound;
        Gtk::Box box_interface;

        DrawingArea drawingArea;
        DrawingArea drawingArea_interface;
        Gtk::Button btn_play_sound;
        Gtk::Entry entry_frequency;
        Gtk::Entry entry_duration;
};

#endif // GTKMM_WINDOW_H
