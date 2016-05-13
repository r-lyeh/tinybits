// Tiny TGA writer: original code by jon olick, public domain
// Elder C version + return codes by rlyeh, public domain
#include <stdio.h>
int tinytga(const char *filename, void *rgba, int width, int height, int numChannels) {
    FILE *fp = fopen(filename, "wb");
    if( fp ) {
        // Swap RGBA to BGRA if using 3 or more channels
        int x, i, y, j, bpc = numChannels * 8; // 8 bits per channel
        int remap[4] = {numChannels >= 3 ? 2 : 0, 1, numChannels >= 3 ? 0 : 2, 3};
        char *s = (char *)rgba;
        // Header
        fwrite("\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00", 12, 1, fp);
        fwrite(&width, 2, 1, fp);
        fwrite(&height, 2, 1, fp);
        fwrite(&bpc, 2, 1, fp);
        for(y = height-1; y >= 0; --y) {
            i = (y * width) * numChannels;
            for(x = i; x < i+width*numChannels; x += numChannels) {
                for(j = 0; j < numChannels; ++j) {
                    fputc(s[x+remap[j]], fp);
                }
            }
        }
        fclose(fp);
        return 1;
    }
    return 0;
}
