#include <os.h>

#define OS_TICKS_PER_SEC OS_CFG_TICK_RATE_HZ

int main(void)
{
    OS_ERR uce;
    OSInit(&uce);

    OSStart(&uce);

    for (;;)
    {
        OSTimeDly(OS_TICKS_PER_SEC, OS_OPT_TIME_DLY, &uce);
    }

    return 0;
}