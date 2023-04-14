#include "window.h"
#include "soundfuncs.h"


Window::Window()
{
    /* Window */
    set_default_size(800, 800);

    /* DrawingArea box */
    box_drawingarea.pack_start(drawingArea);
    drawingArea.signal_draw().connect(sigc::mem_fun(drawingArea, &DrawingArea::draw_tones));

    /* PlaySound box */
    btn_play_sound.set_label("Play Sound");
    btn_play_sound.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_btn_play_sound));

    box_play_sound.pack_start(btn_play_sound);

    /* Interface box */
    box_interface.pack_start(drawingArea_interface_1);
    /*box_interface.pack_start(drawingArea_interface_2);
    box_interface.pack_start(drawingArea_interface_3);*/

    drawingArea_interface_1.signal_draw().connect(sigc::mem_fun(drawingArea_interface_1, &DrawingArea::update_grid));
    drawingArea_interface_1.signal_button_press_event().connect(sigc::mem_fun(drawingArea_interface_1, &DrawingArea::on_click));

    /*drawingArea_interface_2.signal_draw().connect(sigc::mem_fun(drawingArea_interface_2, &DrawingArea::update_grid));
    drawingArea_interface_2.signal_button_press_event().connect(sigc::mem_fun(drawingArea_interface_2, &DrawingArea::on_click));

    drawingArea_interface_3.signal_draw().connect(sigc::mem_fun(drawingArea_interface_3, &DrawingArea::update_grid));
    drawingArea_interface_3.signal_button_press_event().connect(sigc::mem_fun(drawingArea_interface_3, &DrawingArea::on_click));*/

    drawingArea_interface_1.set_margin_start(10);
    drawingArea_interface_1.set_margin_end(10);
    drawingArea_interface_1.set_margin_top(10);
    drawingArea_interface_1.set_margin_bottom(10);

    /*drawingArea_interface_2.set_margin_start(10);
    drawingArea_interface_2.set_margin_end(10);
    drawingArea_interface_2.set_margin_top(10);
    drawingArea_interface_2.set_margin_bottom(10);

    drawingArea_interface_3.set_margin_start(10);
    drawingArea_interface_3.set_margin_end(10);
    drawingArea_interface_3.set_margin_top(10);
    drawingArea_interface_3.set_margin_bottom(10);*/

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
    sine_wave_to_buffer(buf, 44100, 500, 1);
    pa_simple_write(s, buf.data(), (size_t) buf.size(), &error);
    pa_simple_drain(s, &error);

    std::vector<int> sound_data = drawingArea_interface_1.get_sound_data();

    /*for (int i = 0; i < sound_data.size(); i++)
    {
        printf("%d\n", sound_data[i]);
    }*/

    drawingArea.set_sound_data(sound_data);
    drawingArea.queue_draw();
}
