#include "window.h"
#include "soundfuncs.h"


Window::Window()
{
    set_default_size(800, 800);

    btn_play_sound.set_label("Play Sound");
    btn_play_sound.set_margin_top(350);
    btn_play_sound.set_margin_bottom(350);
    btn_play_sound.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_btn_play_sound));

    entry_frequency.set_text("100");
    entry_frequency.set_margin_top(350);
    entry_frequency.set_margin_bottom(350);

    entry_duration.set_text("1");
    entry_duration.set_margin_top(350);
    entry_duration.set_margin_bottom(350);

    add(box);

    box.pack_start(btn_play_sound);
    btn_play_sound.show();

    box.pack_start(entry_frequency);
    entry_frequency.show();

    box.pack_start(entry_duration);
    entry_duration.show();

    box.show();

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
}
