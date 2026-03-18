/*===================catalogue======================*/
// 1. 系统任务（高优先级） 
// 2. 输入任务（按键、传感器）
// 3. 处理任务（数据处理、算法）
// 4. 辅助任务（监控、调试）
// 5. 输出任务（LED、显示、通信）
/*===================catalogue======================*/
#ifndef __RTOS_TASKS_H__
#define __RTOS_TASKS_H__

//#define configUSE_STATS_FORMATTING_FUNCTIONS 1

/* ==================== 系统任务 ==================== */


/* ==================== 输入任务 ==================== */


/* ==================== 处理任务 ==================== */


/* ==================== 辅助任务 ==================== */
void RTOS_Create_All_Tasks(void *pvParameters);                  // 创建所有FreeRTOS任务


/* ==================== 输出任务 ==================== */
void RTOS_LED_P13_Task(void *pvParameters);                      // 控制PC13 LED
void RTOS_Monitoring_Task(void *pvParameters);                   // 系统监控（内存、堆）
void UART_Printf(const char *format, ...);                       // 串口DMA输出

#endif /* __RTOS_TASKS_H__ */
