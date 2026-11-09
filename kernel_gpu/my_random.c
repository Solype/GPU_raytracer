// Unsigned on purpose: signed overflow is undefined and >> on a negative
// int copies the sign bit, which ruins the hash.

uint wang_hash(uint seed)
{
    seed = (seed ^ 61u) ^ (seed >> 16);
    seed *= 9u;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2du;
    seed = seed ^ (seed >> 15);
    return seed;
}

// xorshift32: advances the state (must never be 0), returns a float in [0, 1)
float rand01(uint *state)
{
    uint x = *state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return (float)(x >> 8) * (1.0f / 16777216.0f);
}
