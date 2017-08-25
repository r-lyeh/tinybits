// tinymime. ported from https://github.com/sindresorhus/file-type (source is mit licensed)
// - rlyeh, public domain

#pragma once

static const char *tinymime( const unsigned char *buf, size_t len ) {
    if( !(buf && len > 60) ) {
        return ""; // invalid
    }

    if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF) {
        return "jpg";
    }

    if (buf[0] == 0x89 && buf[1] == 0x50 && buf[2] == 0x4E && buf[3] == 0x47) {
        return "png";
    }

    if (buf[0] == 0x47 && buf[1] == 0x49 && buf[2] == 0x46) {
        return "gif";
    }

    if (buf[8] == 0x57 && buf[9] == 0x45 && buf[10] == 0x42 && buf[11] == 0x50) {
        return "webp";
    }

    // needs to be before `tif` check
    if (((buf[0] == 0x49 && buf[1] == 0x49 && buf[2] == 0x2A && buf[3] == 0x0) || (buf[0] == 0x4D && buf[1] == 0x4D && buf[2] == 0x0 && buf[3] == 0x2A)) && buf[8] == 0x43 && buf[9] == 0x52) {
        return "cr2";
    }

    if ((buf[0] == 0x49 && buf[1] == 0x49 && buf[2] == 0x2A && buf[3] == 0x0) || (buf[0] == 0x4D && buf[1] == 0x4D && buf[2] == 0x0 && buf[3] == 0x2A)) {
        return "tif";
    }

    if (buf[0] == 0x42 && buf[1] == 0x4D) {
        return "bmp";
    }

    if (buf[0] == 0x49 && buf[1] == 0x49 && buf[2] == 0xBC) {
        return "jxr";
    }

    if (buf[0] == 0x38 && buf[1] == 0x42 && buf[2] == 0x50 && buf[3] == 0x53) {
        return "psd";
    }

    // needs to be before `zip` check
    if (buf[0] == 0x50 && buf[1] == 0x4B && buf[2] == 0x3 && buf[3] == 0x4 && buf[30] == 0x6D && buf[31] == 0x69 && buf[32] == 0x6D && buf[33] == 0x65 && buf[34] == 0x74 && buf[35] == 0x79 && buf[36] == 0x70 && buf[37] == 0x65 && buf[38] == 0x61 && buf[39] == 0x70 && buf[40] == 0x70 && buf[41] == 0x6C && buf[42] == 0x69 && buf[43] == 0x63 && buf[44] == 0x61 && buf[45] == 0x74 && buf[46] == 0x69 && buf[47] == 0x6F && buf[48] == 0x6E && buf[49] == 0x2F && buf[50] == 0x65 && buf[51] == 0x70 && buf[52] == 0x75 && buf[53] == 0x62 && buf[54] == 0x2B && buf[55] == 0x7A && buf[56] == 0x69 && buf[57] == 0x70) {
        return "epub";
    }

    // needs to be before `zip` check
    // assumes signed .xpi from addons.mozilla.org
    if (buf[0] == 0x50 && buf[1] == 0x4B && buf[2] == 0x3 && buf[3] == 0x4 && buf[30] == 0x4D && buf[31] == 0x45 && buf[32] == 0x54 && buf[33] == 0x41 && buf[34] == 0x2D && buf[35] == 0x49 && buf[36] == 0x4E && buf[37] == 0x46 && buf[38] == 0x2F && buf[39] == 0x6D && buf[40] == 0x6F && buf[41] == 0x7A && buf[42] == 0x69 && buf[43] == 0x6C && buf[44] == 0x6C && buf[45] == 0x61 && buf[46] == 0x2E && buf[47] == 0x72 && buf[48] == 0x73 && buf[49] == 0x61) {
        return "xpi";
    }

    if (buf[0] == 0x50 && buf[1] == 0x4B && (buf[2] == 0x3 || buf[2] == 0x5 || buf[2] == 0x7) && (buf[3] == 0x4 || buf[3] == 0x6 || buf[3] == 0x8)) {
        return "zip";
    }

    if( len > 261 ) 
    if (buf[257] == 0x75 && buf[258] == 0x73 && buf[259] == 0x74 && buf[260] == 0x61 && buf[261] == 0x72) {
        return "tar";
    }

    if (buf[0] == 0x52 && buf[1] == 0x61 && buf[2] == 0x72 && buf[3] == 0x21 && buf[4] == 0x1A && buf[5] == 0x7 && (buf[6] == 0x0 || buf[6] == 0x1)) {
        return "rar";
    }

    if (buf[0] == 0x1F && buf[1] == 0x8B && buf[2] == 0x8) {
        return "gz";
    }

    if (buf[0] == 0x42 && buf[1] == 0x5A && buf[2] == 0x68) {
        return "bz2";
    }

    if (buf[0] == 0x37 && buf[1] == 0x7A && buf[2] == 0xBC && buf[3] == 0xAF && buf[4] == 0x27 && buf[5] == 0x1C) {
        return "7z";
    }

    if (buf[0] == 0x78 && buf[1] == 0x01) {
        return "dmg";
    }

    if (
        (buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && (buf[3] == 0x18 || buf[3] == 0x20) && buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70) ||
        (buf[0] == 0x33 && buf[1] == 0x67 && buf[2] == 0x70 && buf[3] == 0x35) ||
        (buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && buf[3] == 0x1C && buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70 && buf[8] == 0x6D && buf[9] == 0x70 && buf[10] == 0x34 && buf[11] == 0x32 && buf[16] == 0x6D && buf[17] == 0x70 && buf[18] == 0x34 && buf[19] == 0x31 && buf[20] == 0x6D && buf[21] == 0x70 && buf[22] == 0x34 && buf[23] == 0x32 && buf[24] == 0x69 && buf[25] == 0x73 && buf[26] == 0x6F && buf[27] == 0x6D) ||
        (buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && buf[3] == 0x1C && buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70 && buf[8] == 0x69 && buf[9] == 0x73 && buf[10] == 0x6F && buf[11] == 0x6D) ||
        (buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && buf[3] == 0x1c && buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70 && buf[8] == 0x6D && buf[9] == 0x70 && buf[10] == 0x34 && buf[11] == 0x32 && buf[12] == 0x0 && buf[13] == 0x0 && buf[14] == 0x0 && buf[15] == 0x0)
    ) {
        return "mp4";
    }

    if ((buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && buf[3] == 0x1C && buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70 && buf[8] == 0x4D && buf[9] == 0x34 && buf[10] == 0x56)) {
        return "m4v";
    }

    if (buf[0] == 0x4D && buf[1] == 0x54 && buf[2] == 0x68 && buf[3] == 0x64) {
        return "mid";
    }

    // needs to be before the `webm` check
    if (buf[31] == 0x6D && buf[32] == 0x61 && buf[33] == 0x74 && buf[34] == 0x72 && buf[35] == 0x6f && buf[36] == 0x73 && buf[37] == 0x6B && buf[38] == 0x61) {
        return "mkv";
    }

    if (buf[0] == 0x1A && buf[1] == 0x45 && buf[2] == 0xDF && buf[3] == 0xA3) {
        return "webm";
    }

    if (buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x0 && buf[3] == 0x14 && buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70) {
        return "mov";
    }

    if (buf[0] == 0x52 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x46 && buf[8] == 0x41 && buf[9] == 0x56 && buf[10] == 0x49) {
        return "avi";
    }

    if (buf[0] == 0x30 && buf[1] == 0x26 && buf[2] == 0xB2 && buf[3] == 0x75 && buf[4] == 0x8E && buf[5] == 0x66 && buf[6] == 0xCF && buf[7] == 0x11 && buf[8] == 0xA6 && buf[9] == 0xD9) {
        return "wmv";
    }

    if (buf[0] == 0x0 && buf[1] == 0x0 && buf[2] == 0x1 && (buf[3] >> 4) == 0xb ) { // buf[3].toString(16)[0] === 'b') {
        return "mpg";
    }

    if ((buf[0] == 0x49 && buf[1] == 0x44 && buf[2] == 0x33) || (buf[0] == 0xFF && buf[1] == 0xfb)) {
        return "mp3";
    }

    if ((buf[4] == 0x66 && buf[5] == 0x74 && buf[6] == 0x79 && buf[7] == 0x70 && buf[8] == 0x4D && buf[9] == 0x34 && buf[10] == 0x41) || (buf[0] == 0x4D && buf[1] == 0x34 && buf[2] == 0x41 && buf[3] == 0x20)) {
        return "m4a";
    }

    // needs to be before `ogg` check
    if (buf[28] == 0x4F && buf[29] == 0x70 && buf[30] == 0x75 && buf[31] == 0x73 && buf[32] == 0x48 && buf[33] == 0x65 && buf[34] == 0x61 && buf[35] == 0x64) {
        return "opus";
    }

    if (buf[0] == 0x4F && buf[1] == 0x67 && buf[2] == 0x67 && buf[3] == 0x53) {
        return "ogg";
    }

    if (buf[0] == 0x66 && buf[1] == 0x4C && buf[2] == 0x61 && buf[3] == 0x43) {
        return "flac";
    }

    if (buf[0] == 0x52 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x46 && buf[8] == 0x57 && buf[9] == 0x41 && buf[10] == 0x56 && buf[11] == 0x45) {
        return "wav";
    }

    if (buf[0] == 0x23 && buf[1] == 0x21 && buf[2] == 0x41 && buf[3] == 0x4D && buf[4] == 0x52 && buf[5] == 0x0A) {
        return "amr";
    }

    if (buf[0] == 0x25 && buf[1] == 0x50 && buf[2] == 0x44 && buf[3] == 0x46) {
        return "pdf";
    }

    if (buf[0] == 0x4D && buf[1] == 0x5A) {
        return "exe";
    }

    if ((buf[0] == 0x43 || buf[0] == 0x46) && buf[1] == 0x57 && buf[2] == 0x53) {
        return "swf";
    }

    if (buf[0] == 0x7B && buf[1] == 0x5C && buf[2] == 0x72 && buf[3] == 0x74 && buf[4] == 0x66) {
        return "rtf";
    }

    if (
        (buf[0] == 0x77 && buf[1] == 0x4F && buf[2] == 0x46 && buf[3] == 0x46) &&
        (
            (buf[4] == 0x00 && buf[5] == 0x01 && buf[6] == 0x00 && buf[7] == 0x00) ||
            (buf[4] == 0x4F && buf[5] == 0x54 && buf[6] == 0x54 && buf[7] == 0x4F)
        )
    ) {
        return "woff";
    }

    if (
        (buf[0] == 0x77 && buf[1] == 0x4F && buf[2] == 0x46 && buf[3] == 0x32) &&
        (
            (buf[4] == 0x00 && buf[5] == 0x01 && buf[6] == 0x00 && buf[7] == 0x00) ||
            (buf[4] == 0x4F && buf[5] == 0x54 && buf[6] == 0x54 && buf[7] == 0x4F)
        )
    ) {
        return "woff2";
    }

    if (
        (buf[34] == 0x4C && buf[35] == 0x50) &&
        (
            (buf[8] == 0x00 && buf[9] == 0x00 && buf[10] == 0x01) ||
            (buf[8] == 0x01 && buf[9] == 0x00 && buf[10] == 0x02) ||
            (buf[8] == 0x02 && buf[9] == 0x00 && buf[10] == 0x02)
        )
    ) {
        return "eot";
    }

    if (buf[0] == 0x00 && buf[1] == 0x01 && buf[2] == 0x00 && buf[3] == 0x00 && buf[4] == 0x00) {
        return "ttf";
    }

    if (buf[0] == 0x4F && buf[1] == 0x54 && buf[2] == 0x54 && buf[3] == 0x4F && buf[4] == 0x00) {
        return "otf";
    }

    if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x01 && buf[3] == 0x00) {
        return "ico";
    }

    if (buf[0] == 0x46 && buf[1] == 0x4C && buf[2] == 0x56 && buf[3] == 0x01) {
        return "flv";
    }

    if (buf[0] == 0x25 && buf[1] == 0x21) {
        return "ps";
    }

    if (buf[0] == 0xFD && buf[1] == 0x37 && buf[2] == 0x7A && buf[3] == 0x58 && buf[4] == 0x5A && buf[5] == 0x00) {
        return "xz";
    }

    if (buf[0] == 0x53 && buf[1] == 0x51 && buf[2] == 0x4C && buf[3] == 0x69) {
        return "sqlite";
    }

    if (buf[0] == 0x4E && buf[1] == 0x45 && buf[2] == 0x53 && buf[3] == 0x1A) {
        return "nes";
    }

    if (buf[0] == 0x43 && buf[1] == 0x72 && buf[2] == 0x32 && buf[3] == 0x34) {
        return "crx";
    }

    if (
        (buf[0] == 0x4D && buf[1] == 0x53 && buf[2] == 0x43 && buf[3] == 0x46) ||
        (buf[0] == 0x49 && buf[1] == 0x53 && buf[2] == 0x63 && buf[3] == 0x28)
    ) {
        return "cab";
    }

    // needs to be before `ar` check
    if (buf[0] == 0x21 && buf[1] == 0x3C && buf[2] == 0x61 && buf[3] == 0x72 && buf[4] == 0x63 && buf[5] == 0x68 && buf[6] == 0x3E && buf[7] == 0x0A && buf[8] == 0x64 && buf[9] == 0x65 && buf[10] == 0x62 && buf[11] == 0x69 && buf[12] == 0x61 && buf[13] == 0x6E && buf[14] == 0x2D && buf[15] == 0x62 && buf[16] == 0x69 && buf[17] == 0x6E && buf[18] == 0x61 && buf[19] == 0x72 && buf[20] == 0x79) {
        return "deb";
    }

    if (buf[0] == 0x21 && buf[1] == 0x3C && buf[2] == 0x61 && buf[3] == 0x72 && buf[4] == 0x63 && buf[5] == 0x68 && buf[6] == 0x3E) {
        return "ar";
    }

    if (buf[0] == 0xED && buf[1] == 0xAB && buf[2] == 0xEE && buf[3] == 0xDB) {
        return "rpm";
    }

    if (
        (buf[0] == 0x1F && buf[1] == 0xA0) ||
        (buf[0] == 0x1F && buf[1] == 0x9D)
    ) {
        return "z";
    }

    if (buf[0] == 0x4C && buf[1] == 0x5A && buf[2] == 0x49 && buf[3] == 0x50) {
        return "lz";
    }

    if (buf[0] == 0xD0 && buf[1] == 0xCF && buf[2] == 0x11 && buf[3] == 0xE0 && buf[4] == 0xA1 && buf[5] == 0xB1 && buf[6] == 0x1A && buf[7] == 0xE1) {
        return "msi";
    }

    return ""; // invalid
};

#ifdef MAIN_MIME
#include <stdio.h>
void MAIN_MIME( int argc, const char **argv ) {
    if( argc == 2 ) {
        FILE *fp = fopen( argv[1], "rb" );
        if( fp ) {
            char buf[512];
            int len = fread(buf, 1, 512, fp);
            puts( tinymime(buf, len) );
            fclose(fp);
        }
    }
}
#endif
