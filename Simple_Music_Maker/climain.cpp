#include <fcntl.h>

#include <pulse/simple.h>
#include <pulse/error.h>

#include <vector>
#include <cmath>
#include <regex>

#include "clifuncs.h"

#define SAMPLE_RATE 44100

int main(int argc, char* argv[])
{
    /* Sample Format for playback */
    static const pa_sample_spec ss =
    {
        .format = PA_SAMPLE_U8,
        .rate = 44100,
        .channels = 1
    };

    pa_simple *s = NULL;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error)))
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    std::vector<uint8_t> buf;
    std::vector<uint8_t> sample_buf(BUFSIZE);
    std::regex rgx_path("(\/[a-zA-Z0-9_]+)+[.]wav");

    uint8_t argument_counter = 1;
    bool found_match;
    int file_descriptor;

    // Fill buffer with frequencies or samples from wave files
    for (;;)
    {
        /* Check if the argument is a file path */
        std::string argument(argv[argument_counter]);
        found_match = std::regex_match(argument, rgx_path);

        if (found_match)
        {
            /* replace STDIN with the specified file */
            if ((file_descriptor = open(argv[argument_counter], O_RDONLY)) < 0)
            {
                fprintf(stderr, __FILE__": open() failed: %s\n", strerror(errno));
                return 1;
            }

            if (dup2(file_descriptor, STDIN_FILENO) < 0)
            {
                fprintf(stderr, __FILE__": dup2() failed: %s\n", strerror(errno));
                return 1;
            }

            close(file_descriptor);

            // Read data from wav file and load it into sample buffer
            read(STDIN_FILENO, sample_buf.data(), sample_buf.size());

            // Convert to data with only one channel
            for (int sample = 0; sample < sample_buf.size(); sample++)
            {
                sample_buf[sample] = round((sample_buf[sample] + sample_buf[sample + 1]) / 2); // average out the channels into one
                sample_buf.erase(sample_buf.begin() + sample); // remove the second channel
            }

            // Allocate more memory in buffer
            buf.resize(buf.size() + sample_buf.size());

            // Move sample data to main buffer
            for (int sample = 0; sample < sample_buf.size(); sample++)
            {
                std::move(sample_buf.begin(), sample_buf.end(), buf.end() - sample_buf.size());
            }

            // Reallocate erased memory
            sample_buf.resize(2 * sample_buf.size());

            argument_counter++;
        }
        else
        {
            const float frequency = atof(argv[argument_counter]);
            const float duration = atof(argv[argument_counter + 1]);

            sine_wave_to_buffer(buf, BUFSIZE, frequency, duration);

            argument_counter += 2;
        }

        if (argument_counter == argc) break;
    }

    /* Play the main buffer */
    if (pa_simple_write(s, buf.data(), (size_t) buf.size(), &error) < 0)
    {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
        return 1;
    }

    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0)
    {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(error));
        return 1;
    }

    pa_simple_free(s);

    return 0;
}
