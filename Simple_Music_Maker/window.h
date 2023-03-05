#ifndef GTKMM_WINDOW_H
#define GTKMM_WINDOW_H

#include <gtkmm.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include <vector>


class Window : public Gtk::Window
{
    public:
        Window();
        virtual ~Window();

    protected:
        // Signal handlers
        void on_btn_play_sound();

        // Child widgets
        Gtk::Box box;
        Gtk::Button btn_play_sound;
        Gtk::Entry entry_frequency;
        Gtk::Entry entry_duration;

    private:
        static const pa_sample_spec ss;
        pa_simple *s = NULL;
        int error;

        std::vector<uint8_t> buf;

        void pulse_audio_init();
};

#endif
