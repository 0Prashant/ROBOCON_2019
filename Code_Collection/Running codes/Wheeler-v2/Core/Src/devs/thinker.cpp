#include "devs/thinker.h"
#include "utils/queue.h"
#include "robot/defines.h"
#include "periphs/usart.h"

#define NUM_OMEGAS_RECEIVED	(8)

static uint8_t gThinker_Byte;
static bool gStart_Byte_Received = false;
static uint8_t gNum_Data_Received = 0;
static int16_t gOmega;

static Queue<uint8_t, NUM_OMEGAS_RECEIVED> gThinker_Bytes;
static Queue<int16_t, 1> gThinker_Omegas[4];

void Thinker_Init(void)
{
	HAL_UART_Receive_DMA(&huart3, &gThinker_Byte, 1);
}

void get_Thinker(float arr[4])
{
	for (uint8_t i= 0; i < 4; ++i) {
		arr[i] = gThinker_Omegas[i].lookup();
		arr[i] *= (MAX_POSSIBLE_OMEGA / 32768.0);
	}
}

bool Thinker_isEmpty()
{
    if (gThinker_Omegas[0].length()) {
        return false;
    }
    return true;
}

void Thinker_UART_RxCpltCallback()
{
        if(!gStart_Byte_Received)
        {
                if (gThinker_Byte == 0xA5) {
                        gStart_Byte_Received = true;
                }
        }
        else
        {
                gThinker_Bytes.insert(gThinker_Byte);
                ++gNum_Data_Received;
                if (gNum_Data_Received == NUM_OMEGAS_RECEIVED) {
                        gNum_Data_Received = 0;
                        gStart_Byte_Received = false;
                        // Omega calculation using big-endian
                        for (uint8_t i = 0; i < 4; ++i) {
                                gOmega = gThinker_Bytes.lookup() << 8;
                                gOmega += gThinker_Bytes.lookup();
                                gThinker_Omegas[i].insert(gOmega);
                        }
                }
        }
}
