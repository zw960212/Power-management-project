#include "Cmd_Comm.h"
#include "stdio.h"
#include "Packet_type.h"


void DataCompress(uint8_t* src,uint16_t srcLen,uint8_t *dest)
{
	uint16_t len = 0;
	
	//���յ�������  
	if(srcLen > 0)
	{
		 while ((len < srcLen))
    {
        *dest = *src;
        len++;
				src++;
				dest++;
    }
	}
	else
	{
		return;
	}
}




