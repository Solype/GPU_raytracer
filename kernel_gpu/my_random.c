
int pcg_hash(int input) {
    input ^= input >> 16;
    input *= 0x85ebca6b;
    input ^= input >> 13;
    input *= 0xc2b2ae35;
    input ^= input >> 16;
    return input;
}

int wang_hash(int seed) {
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

int hash1(int seed) {
    return (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
}

int randint(int seed) {
    return wang_hash(seed);
}

float rand01(int k) {
    return (float)(wang_hash(k) & 0x00FFFFFF) / (float)0x01000000;
}
