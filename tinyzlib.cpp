// tiny zlib inflater. extracted from tigr (credits to Richard Mitton). @todo: remove exceptions
// - rlyeh, public domain | wtrmrkrlyeh

static bool tinyzlib(void *out, unsigned outlen, const void *in, unsigned inlen) {
    struct State {
        unsigned bits, count;
        const unsigned char *in, *inend;
        unsigned char *out, *outend;
        unsigned litcodes[288], distcodes[32], lencodes[19];
        int tlit, tdist, tlen;
    };

    // Built-in DEFLATE standard tables.
    const char order[] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
    const char lenBits[29+2] = { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,  0,0 };
    const int lenBase[29+2] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258,  0,0 };
    const char distBits[30+2] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,  0,0 };
    const int distBase[30+2] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577 };

    auto emit = [](State *s, int len) -> unsigned char * {
        s->out += len;
        if(!(s->out <= s->outend)) throw;
        return s->out-len;
    };

    auto bits = [](State *s, int n) {
        int v = s->bits & ((1 << n)-1);
        s->bits >>= n;
        s->count -= n;
        while (s->count < 16) {
            if(!(s->in != s->inend)) throw;
            s->bits |= (*s->in++) << s->count;
            s->count += 8;
        }
        return v;
    };

    auto copy = [&](State *s, const unsigned char *src, int len) {
        unsigned char *dest = emit(s, len);
        while (len--) *dest++ = *src++;
    };

    auto decode = [&](State *s, unsigned tree[], int max) {
        auto rev16_src = [](unsigned n) -> unsigned {
            // Table to .
            static const unsigned char reverseTable[256] = {
            #define R2(n)    n,     n + 128,     n + 64,     n + 192
            #define R4(n) R2(n), R2(n +  32), R2(n + 16), R2(n +  48)
            #define R6(n) R4(n), R4(n +   8), R4(n +  4), R4(n +  12)
                R6(0), R6(2), R6(1), R6(3)
            };
            return (reverseTable[n&0xff] << 8) | reverseTable[(n>>8)&0xff];
        };

        auto rev16 = [](unsigned n) -> unsigned { // bit-reverse two bytes
            return (((((n   )&0xff) * 0x0202020202ULL & 0x010884422010ULL) % 1023) << 8)
                 |  ((((n>>8)&0xff) * 0x0202020202ULL & 0x010884422010ULL) % 1023);
        };

        // Find the next prefix code.
        unsigned lo = 0, hi = max, key;
        unsigned search = (rev16(s->bits) << 16) | 0xffff;
        while (lo < hi) {
            unsigned guess = (lo + hi) / 2;
            if (search < tree[guess]) hi = guess;
            else lo = guess + 1;
        }

        // Pull out the key and check it.
        key = tree[lo-1];
        if(!(((search^key) >> (32-(key&0xf))) == 0)) throw;

        bits(s, key & 0xf);
        return (key >> 4) & 0xfff;
    };

    auto build = [&](State *s, unsigned *tree, unsigned char *lens, int symcount) -> int {
        int n, codes[16], first[16], counts[16]={0};

        // Frequency count.
        for (n=0;n<symcount;n++) counts[lens[n]]++;

        // Distribute codes.
        counts[0] = codes[0] = first[0] = 0;
        for (n=1;n<=15;n++) {
            codes[n] = (codes[n-1] + counts[n-1]) << 1;
            first[n] = first[n-1] + counts[n-1];
        }
        if(!(first[15]+counts[15] <= symcount)) throw;

        // Insert keys into the tree for each symbol.
        for (n=0;n<symcount;n++) {
            int len = lens[n];
            if (len != 0) {
                int code = codes[len]++, slot = first[len]++;
                tree[slot] = (code << (32-len)) | (n << 4) | len;
            }
        }

        return first[15];
    };

    auto block = [&](State *s) {
        auto run = [&](State *s, int sym) {
            int length = bits(s, lenBits[sym]) + lenBase[sym];
            int dsym = decode(s, s->distcodes, s->tdist);
            int offs = bits(s, distBits[dsym]) + distBase[dsym];
            copy(s, s->out - offs, length);
        };
        for (;;) {
            int sym = decode(s, s->litcodes, s->tlit);
                 if (sym < 256) *emit(s, 1) = (unsigned char)sym;
            else if (sym > 256) run(s, sym-257);
            else break;
        }
    };

    auto stored = [&](State *s) { // Uncompressed data block
        int len; 
        bits(s, s->count & 7);
        len = bits(s, 16);
        if(!(((len^s->bits)&0xffff) == 0xffff)) throw;
        if(!(s->in + len <= s->inend)) throw;

        copy(s, s->in, len);
        s->in += len;
        bits(s, 16);
    };

    auto fixed = [&](State *s) { // Fixed set of Huffman codes
        int n;
        unsigned char lens[288+32];
        for (n=  0;n<=143;n++) lens[n] = 8;
        for (n=144;n<=255;n++) lens[n] = 9;
        for (n=256;n<=279;n++) lens[n] = 7;
        for (n=280;n<=287;n++) lens[n] = 8;
        for (n=0;n<32;n++) lens[288+n] = 5;

        // Build lit/dist trees.
        s->tlit  = build(s, s->litcodes, lens, 288);
        s->tdist = build(s, s->distcodes, lens+288, 32);
    };

    auto dynamic = [&](State *s) {
        int n, i, nlit, ndist, nlen;
        unsigned char lenlens[19] = {0}, lens[288+32];
        nlit = 257 + bits(s, 5);
        ndist = 1 + bits(s, 5);
        nlen = 4 + bits(s, 4);
        for (n=0;n<nlen;n++) {
            lenlens[order[n]] = (unsigned char)bits(s, 3);
        }

        // Build the tree for decoding code lengths.
        s->tlen = build(s, s->lencodes, lenlens, 19);

        // Decode code lengths.
        for (n=0;n<nlit+ndist;) {
            int sym = decode(s, s->lencodes, s->tlen);
            switch (sym) {
            case 16: for (i =  3+bits(s,2); i; i--,n++) lens[n] = lens[n-1]; break;
            case 17: for (i =  3+bits(s,3); i; i--,n++) lens[n] = 0; break;
            case 18: for (i = 11+bits(s,7); i; i--,n++) lens[n] = 0; break;
            default: lens[n++] = (unsigned char)sym; break;
            }
        }

        // Build lit/dist trees.
        s->tlit  = build(s, s->litcodes, lens, nlit);
        s->tdist = build(s, s->distcodes, lens+nlit, ndist);
    };

    struct State s0 = {}, *s = &s0;

    // We assume we can buffer 2 extra bytes from off the end of 'in'.
    s->in  = (unsigned char *)in;  s->inend  = s->in  + inlen + 2;
    s->out = (unsigned char *)out; s->outend = s->out + outlen;
    s->bits = 0; s->count = 0; bits(s, 0);

    try {
        for( int last = 0; !last;  ) {
            last = bits(s, 1);
            switch (bits(s, 2)) {
            case 0: stored(s); break;
            case 1: fixed(s); block(s); break;
            case 2: dynamic(s); block(s); break;
            case 3: return 0;
            }
        }
        return true;
    }
    catch(...) 
    {}

    return false;
}

