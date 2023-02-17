#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include <vector>
#include <cmath>

#include "clifuncs.h"

#define BUFSIZE 44100

int main(int argc, char* argv[]) {

    /* The Sample format to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_U8,
        .rate = 44100,
        .channels = 1
    };

    pa_simple *s = NULL;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    /* Create buffer ... */
    std::vector<uint8_t> buf;

    /* Fill buffer with arguments from cli */

    for (uint8_t tone = 0; tone < (argc - 1) / 2; tone ++) {
        const float frequency = atof(argv[(tone + 1) * 2 - 1]);
        const float duration = atof(argv[(tone + 1) * 2]);

        sine_wave_to_buffer(buf, BUFSIZE, frequency, duration);
    }

    /* ... and play it */
    if (pa_simple_write(s, buf.data(), (size_t) buf.size(), &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        return 1;
    }

    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        return 1;
    }

    pa_simple_free(s);

    return 0;
}
