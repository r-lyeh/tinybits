// Tiny WAV writer: original code by jon olick, public domain
// Floating point support + pure C version by rlyeh, public domain | wtrmrkrlyeh
#include <stdio.h>
int tinywav(const char *filename, short numChannels, short bitsPerSample, int sampleRateHz, const void *data, int size, int is_floating) {
    short bpsamp;
    int length, bpsec;
    FILE *fp = fopen(filename, "wb");
    if(!fp) {
        return 0;
    }
    fwrite("RIFF", 1, 4, fp);
    length = size + 44 - 8;
    fwrite(&length, 1, 4, fp);
    fwrite(is_floating ? "WAVEfmt \x10\x00\x00\x00\x03\x00" : "WAVEfmt \x10\x00\x00\x00\x01\x00", 1, 14, fp);
    fwrite(&numChannels, 1, 2, fp);
    fwrite(&sampleRateHz, 1, 4, fp);
    bpsec = numChannels * sampleRateHz * bitsPerSample/8;
    fwrite(&bpsec, 1, 4, fp);
    bpsamp = numChannels * bitsPerSample/8;
    fwrite(&bpsamp, 1, 2, fp);
    fwrite(&bitsPerSample, 1, 2, fp);
    fwrite("data", 1, 4, fp);
    fwrite(&size, 1, 4, fp);
    fwrite(data, 1, size, fp);
    fclose(fp);
    return 1;
}
