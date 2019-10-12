#include "ax8052.h"
#include "libmfbch.h"

uint32_t bch3121_encode_parity(uint32_t cw)
{
	cw &= 0xFFFFF800;
	cw |= bch3121_syndrome(cw) << 1;
	cw |= parity32(cw);
	return cw;
}
