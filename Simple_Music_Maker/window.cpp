#include "window.h"
#include "soundfuncs.h"


Window::Window()
{
    /* Window */
    set_default_size(800, 800);

    /* DrawingArea box */
    box_drawingarea.pack_start(drawingArea);
    drawingArea.signal_draw().connect(sigc::mem_fun(drawingArea, &DrawingArea::draw_frequency));

    /* PlaySound box */
    btn_play_sound.set_label("Play Sound");
    btn_play_sound.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_btn_play_sound));

    entry_frequency.set_text("100");
    entry_duration.set_text("1");

    box_play_sound.pack_start(btn_play_sound);
    box_play_sound.pack_start(entry_frequency);
    box_play_sound.pack_start(entry_duration);

    /* Interface box */
    box_interface.pack_start(drawingArea_interface);
    drawingArea_interface.signal_draw().connect(sigc::mem_fun(drawingArea_interface, &DrawingArea::update_grid));
    drawingArea_interface.signal_button_press_event().connect(sigc::mem_fun(drawingArea_interface, &DrawingArea::on_click));

    /* Parent box */
    box_parent.set_orientation(Gtk::ORIENTATION_VERTICAL);
    box_parent.pack_start(box_drawingarea);
    box_parent.pack_start(box_play_sound);
    box_parent.pack_start(box_interface);

    add(box_parent);
    show_all();

    pulse_audio_init();
}

Window::~Window()
{
    pa_simple_free(s);
}

void Window::pulse_audio_init()
{
    pa_sample_spec ss =
    {
        .format = PA_SAMPLE_U8,
        .rate = 44100,
        .channels = 1
    };

    s = pa_simple_new(NULL, "PS", PA_STREAM_PLAYBACK, NULL, "PLAYBACK", &ss, NULL, NULL, &error);
}

void Window::on_btn_play_sound()
{
    const float frequency = atof(entry_frequency.get_text().data());
    const float duration = atof(entry_duration.get_text().data());

    sine_wave_to_buffer(buf, 44100, frequency, duration);

    pa_simple_write(s, buf.data(), (size_t) buf.size(), &error);
    pa_simple_drain(s, &error);

    drawingArea.set_data(frequency, duration);
    drawingArea.queue_draw();
}
