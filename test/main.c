// Copyright (c) 2022 - ∞ blurryroots innovation qanat OÜ

#include <carter/randolph.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
	if (randolph_startup())
	{
		char RandomSequence[23];
		randolph_buffer(RandomSequence, 23);
		printf("RandomSequence: %s\n", RandomSequence);
		for (size_t SequenceIndex = 0; SequenceIndex < 23; ++SequenceIndex)
		{
			printf("i(%zu): '%c'\n", SequenceIndex, RandomSequence[SequenceIndex]);
		}

		uint32_t RandomInt = randolph_int();
		printf("RandomInt: %lu\n", RandomInt);

		uint8_t Bits[8];
		const uint8_t BitsCount = sizeof(Bits) / sizeof(Bits[0]);
		for (size_t BitIndex = 0; BitIndex < BitsCount; ++BitIndex)
		{
			Bits[BitIndex] = randolph_int_uniform(2);
		}
		printf("Filled %u bits. Listing:\n", BitsCount);
		for (size_t BitIndex = 0; BitIndex < BitsCount; ++BitIndex)
		{
			printf("Bit(%zu): %u\n", BitIndex, Bits[BitIndex]);
		}

		randolph_seed(0);
		uint32_t SeededRandomInt = randolph_int();
		printf("SeededRandomInt: %lu\n", SeededRandomInt);

		randolph_shutdown();
	}

	return 0;
}