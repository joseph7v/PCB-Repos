#include "rng.h"
#include "delay.h"
//��ʼ��RNG
/**
 * @description: initial the rng
 * @param void
 * @return: ���������״̬��0->�ɹ���1->�쳣
 */
u8 rng_Init(void)
{
	u16 retry = 0;

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE); //����RNGʱ��,����PLL48CLK

	RNG_Cmd(ENABLE); //ʹ��RNG

	while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET && retry < 10000) //�ȴ����������
	{
		retry++;
		delay_us(100);
	}
	if (retry >= 10000)
		return 1; //���������������������
	return 0;
}
//�õ������
//����ֵ:��ȡ���������
/**
 * @description: get random num
 * @param void
 * @return: random num -u32
 */
u32 rngGetRandomNum(void)
{
	while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET)
		;
	return RNG_GetRandomNumber();
}
/**
 * @description: ��ȡ[min,max]��Χ�������
 * @param none
 * @return: ���ط�Χ�ڵ����ֵ
 */
int rngGetRandomRange(int min, int max)
{
	return rngGetRandomNum() % (max - min + 1) + min;
}
