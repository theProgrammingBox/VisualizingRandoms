#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		rng(seed++);
		return true;
	}

	void rng(uint32_t seed)
	{
		uint32_t idx = 0;
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				uint32_t ruint32 = idx * 0xcc9e2d51;
				ruint32 ^= (ruint32 << 15) | (ruint32 >> 17);
				ruint32 *= 0x1b873593;
				ruint32 ^= (ruint32 << 13) | (ruint32 >> 19);
				ruint32 ^= seed * 0xcc9e2d51;
				ruint32 ^= (ruint32 << 15) | (ruint32 >> 17);
				ruint32 *= 0x1b873593;
				ruint32 ^= (ruint32 << 13) | (ruint32 >> 19);

				idx++;
				Draw(x, y, olc::PixelF(ruint32 & 0xff, ruint32 >> 8 & 0xff, ruint32 >> 16 & 0xff));
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