#include "RAM.h"


/* Defined for PWR uart communication */
uint8_t mSendDataBuffer[PWR_TXBUFFERSIZE];
uint8_t mSendCmdBuffer[2];                         //CmdIDÎª1¸ö×Ö½Ú
uint8_t mRcvBuffer[PWR_RXBUFFERSIZE];
