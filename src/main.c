#include <os.h>
#include <lib_mem.h>

#define OS_TICKS_PER_SEC OS_CFG_TICK_RATE_HZ

static OS_TCB  task_control_block;
static CPU_STK task_stack[0x100];

void task(void *p_arg)
{
    OS_ERR uce;
    while (1u)
    {
        OSTimeDly(OS_TICKS_PER_SEC, OS_OPT_TIME_DLY, &uce);
    }
}

int main(void)
{
    OS_ERR uce;

    CPU_Init();
    Mem_Init();

    OSInit(&uce);

    OSTaskCreate(&task_control_block,                       /*  (1) Address of TCB assigned to the task      */
                 "task1",                                   /*  (2) Name you want to give the task           */
                 task,                                      /*  (3) Address of the task itself               */
                 (void *)0,                                 /*  (4) "p_arg" is not used                      */
                 12,                                        /*  (5) Priority you want to assign to the task  */
                 &task_stack[0],                            /*  (6) Base address of task's stack             */
                 10,                                        /*  (7) Watermark limit for stack growth         */
                 sizeof(task_stack) / 4u,                        /*  (8) Stack size in number of CPU_STK elements */
                 5,                                         /*  (9) Size of task message queue               */
                 10,                                        /* (10) Time quanta (in number of ticks)         */
                 (void *)0,                                 /* (11) Extension pointer is not used            */
                 OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR, /* (12) Options               */
                 &uce);                                     /* (13) Error code            */


    // OSTaskCreate(&task_control_block2,                       /*  (1) Address of TCB assigned to the task      */
    //              "task2",                                   /*  (2) Name you want to give the task           */
    //              task,                                      /*  (3) Address of the task itself               */
    //              (void *)0,                                 /*  (4) "p_arg" is not used                      */
    //              12,                                        /*  (5) Priority you want to assign to the task  */
    //              &task_stack2[0],                            /*  (6) Base address of task's stack             */
    //              10,                                        /*  (7) Watermark limit for stack growth         */
    //              sizeof(task_stack2),                        /*  (8) Stack size in number of CPU_STK elements */
    //              5,                                         /*  (9) Size of task message queue               */
    //              10,                                        /* (10) Time quanta (in number of ticks)         */
    //              (void *)0,                                 /* (11) Extension pointer is not used            */
    //              OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR, /* (12) Options               */
    //              &uce);                                     /* (13) Error code            */

    // while (1u)
    // {
    //     OSTimeDly(OS_TICKS_PER_SEC, OS_OPT_TIME_DLY, &uce);
    // }
    
    OSStart(&uce);

    return 1;
}