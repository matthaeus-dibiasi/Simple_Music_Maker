#ifndef SOUNDFUNCS_H
#define SOUNDFUNCS_H

/* Load sine wave into buffer */
void sine_wave_to_buffer(std::vector<uint8_t>& buf,
                         const int BUFSIZE,
                         const float frequency,
                         const float duration)
{
    const int samples = BUFSIZE * duration;
    buf.resize(samples);

    for (int sample = 0;
             sample < samples;
             sample++)
    {

        const float x = (float) sample / (float) BUFSIZE;
        buf[sample] = round(127.5 * (sin(2.0 * 3.14 * frequency * x) + 1.0));
    }
}

#endif
