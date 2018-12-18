#ifndef	__PACKET_TYPE_H__
#define	__PACKET_TYPE_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "stdint.h"
	
typedef	uint8_t					AL_BYTE;	/**< unsigned 8bit data type */
typedef	uint16_t				AL_WORD;	/**< unsigned 16bit data type */
typedef	uint32_t				AL_DWORD;	/**< unsigned 32bit data type */
typedef	void            AL_VOID;	/**< void type */
typedef	unsigned long		AL_HANDLE;	/**< handle Type */
typedef	uint8_t					AL_BOOL;	/**< unsigned 16bit data type */
typedef	signed char     AL_CHAR;

#define	AL_NULL			(0)
#define	AL_FALSE		((AL_BOOL)0)
#define	AL_TRUE			((AL_BOOL)1)
#define	AL_INVALID_HANDLE	((AL_HANDLE)0)			/**< invalid handle definition */

#define AL_SIZE_PACK      		   256               //整条报文的长度（10字节）



//??????
#define AL_INFO_DETECT_FINGER               (AL_BYTE)1      //????????,?????????
#define AL_INFO_RELEASE_FINGER               (AL_BYTE)2      //?????????????
#define AL_INFO_GET_IMAGE                   (AL_BYTE)3      //?????????????
#define AL_INFO_PROCESS_IMAGE               (AL_BYTE)4      //??????
#define AL_INFO_COMPARE_USER                (AL_BYTE)5      //??????

#define	SLEEP(n)		HAL_Delay(n)		       //??n??
#define USLEEP(us)  usleep(us)              // ?? us ??

// 系统数据报文发送与接收状态
typedef enum _AL_STATUS
{
    AL_STS_OK = 0,                      // 0 运行成功
    AL_STS_ERROR,                       // 1 运行错误
    AL_STS_ERROR_PARAMETER,             // 3 函数调用入参错误
    AL_STS_ERROR_SEND,                  // 4 发送报文出错
    AL_STS_ERROR_SEND_OVERLAOD,         // 5 发送报文过长
    AL_STS_ERROR_CHECKXOR,              // 6 报文校验异或错误
    AL_STS_ERROR_CHECKSUM,              // 7 报文校验求和错误
    AL_STS_ERROR_RCV_CMD,               // 8 接收报文中的命令码错误
		AL_STS_TIMEOUT,                     // 9 接收报文超时
	
} AL_STATUS;


/* Exported functions ------------------------------------------------------- */
void StringCpy(uint8_t *dest, uint8_t *src, int16_t length);


#ifdef __cplusplus
}
#endif
#endif	/* __DYNA_TYPE_H__ */
