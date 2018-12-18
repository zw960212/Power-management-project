#include "Packet_type.h"


/*String Copy Function*/
void StringCpy(uint8_t *dest, uint8_t *src, int16_t length)
{
  uint16_t len = 0;
	if(length > 0)
	{
    while ((len < length))
    {
        *dest = *src;
        len++;
        src++;
        dest++;
    }
		
	}
	else if(length < 0)
	{
		while (*src != '\0')
    {
        *dest = *src;
        len++;
        src++;
        dest++;
    }
		*dest = '\0';
	}
	else
	{
		return;
	}
}

