#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

inline uint64_t rotl64(uint64_t x, int8_t r)
{
	return (x << r) | (x >> (64 - r));
}

inline uint64_t getblock64(const uint64_t* p, int i)
{
	return p[i];
}

inline uint64_t fmix64(uint64_t k)
{
	k ^= k >> 33;
	k *= 0xff51afd7ed558ccdULL;
	k ^= k >> 33;
	k *= 0xc4ceb9fe1a85ec53ULL;
	k ^= k >> 33;

	return k;
}

void MurmurHash3_128(const void* key, const int len, const uint32_t seed, void* out)
{
	const uint8_t* data = (const uint8_t*)key;
	const int nblocks = len / 16;

	uint64_t h1 = seed;
	uint64_t h2 = seed;

	const uint64_t c1 = 0x87c37b91114253d5ULL;
	const uint64_t c2 = 0x4cf5ad432745937fULL;

	const uint64_t* blocks = (const uint64_t*)(data);
	printf("%d\n", nblocks);
	for (int i = 0; i < nblocks; i++)
	{
		uint64_t k1 = getblock64(blocks, i * 2 + 0);
		uint64_t k2 = getblock64(blocks, i * 2 + 1);
		printf("%llu %llu\n", k1, k2);

		k1 *= c1; k1 = rotl64(k1, 31); k1 *= c2; h1 ^= k1;

		h1 = rotl64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

		k2 *= c2; k2 = rotl64(k2, 33); k2 *= c1; h2 ^= k2;

		h2 = rotl64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
	}

	const uint8_t* tail = (const uint8_t*)(data + nblocks * 16);

	uint64_t k1 = 0;
	uint64_t k2 = 0;

	switch (len & 15)
	{
	case 15: k2 ^= ((uint64_t)tail[14]) << 48;
	case 14: k2 ^= ((uint64_t)tail[13]) << 40;
	case 13: k2 ^= ((uint64_t)tail[12]) << 32;
	case 12: k2 ^= ((uint64_t)tail[11]) << 24;
	case 11: k2 ^= ((uint64_t)tail[10]) << 16;
	case 10: k2 ^= ((uint64_t)tail[9]) << 8;
	case  9: k2 ^= ((uint64_t)tail[8]) << 0;
		k2 *= c2; k2 = rotl64(k2, 33); k2 *= c1; h2 ^= k2;

	case  8: k1 ^= ((uint64_t)tail[7]) << 56;
	case  7: k1 ^= ((uint64_t)tail[6]) << 48;
	case  6: k1 ^= ((uint64_t)tail[5]) << 40;
	case  5: k1 ^= ((uint64_t)tail[4]) << 32;
	case  4: k1 ^= ((uint64_t)tail[3]) << 24;
	case  3: k1 ^= ((uint64_t)tail[2]) << 16;
	case  2: k1 ^= ((uint64_t)tail[1]) << 8;
	case  1: k1 ^= ((uint64_t)tail[0]) << 0;
		k1 *= c1; k1 = rotl64(k1, 31); k1 *= c2; h1 ^= k1;
	};

	h1 ^= len; h2 ^= len;

	h1 += h2;
	h2 += h1;

	h1 = fmix64(h1);
	h2 = fmix64(h2);

	h1 += h2;
	h2 += h1;

	((uint64_t*)out)[0] = h1;
	((uint64_t*)out)[1] = h2;
}

class Example : public olc::PixelGameEngine
{
public:
	uint64_t seed;

	Example()
	{
		sAppName = "Example";
	}

	bool OnUserCreate() override
	{
		seed = 0;
		uint64_t result[2];
		uint128_t idx = 345;
		MurmurHash3_128(&idx, sizeof(idx), seed, result);
		printf("%llu %llu\n", result[0], result[1]);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//rng(seed++);
		return true;
	}

	void rng(uint32_t seed)
	{
		uint32_t idx = 0;
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				//uint32_t result = MurmurHash3_x86_32(idx++, seed);
				//Draw(x, y, olc::PixelF(result & 0xff, result >> 8 & 0xff, result >> 16 & 0xff));

				// using the 128 bit version
				uint64_t result[2];
				MurmurHash3_128(&idx, 8, seed, result);
				Draw(x, y, olc::PixelF(result[0] & 0xff, result[0] >> 8 & 0xff, result[0] >> 16 & 0xff));
				idx++;
			}
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1440, 810, 1, 1))
		demo.Start();
	return 0;
}