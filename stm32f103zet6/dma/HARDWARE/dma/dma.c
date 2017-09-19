#include"dma.h"

void dma_init()
{
	DMA_InitTypeDef DMA_InitTypeD;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel4);
	
	DMA_InitTypeD.DMA_BufferSize=
	DMA_InitTypeD.DMA_DIR=
	DMA_InitTypeD.DMA_M2M=
	DMA_InitTypeD.DMA_MemoryBaseAddr=
	DMA_InitTypeD.DMA_MemoryDataSize=
	DMA_Init(DMAy_Channel4,&DMA_InitTypeD);
}