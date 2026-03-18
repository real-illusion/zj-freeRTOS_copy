/*===================catalogue======================*/
// 1. 系统任务（高优先级） 
// 2. 输入任务（按键、传感器）
// 3. 处理任务（数据处理、算法）
// 4. 辅助任务（监控、调试）
// 5. 输出任务（LED、显示、通信）

/*===================catalogue======================*/

/*===============头文件包含区========================*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"
#include "RTOS_Tasks.h"  // 包含任务头文件
#include "timers.h"
#include "event_groups.h"
#include "portable.h"  // 用于内存监测函数
#include "semphr.h"

//硬件设备
#include "oled.h"			//oled库

#include "gpio.h"
#include "usart.h"
#include "dma.h"
//------------------- uart printf -------------------
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//------------------- uart printf -------------------




/*===============头文件包含区========================*/

/*===============所有参数配置区=====================*/

//<<软件定时器配置>> 
// 定义系统周期性任务的定时器参数

//<<事件组配置>> 
// 定义任务间同步使用的事件标志位

//<<信号量任务配置>> 
// 定义互斥锁、信号量等同步机制

//<<队列任务配置>> 
// 定义任务间通信使用的消息队列
/* 在 FreeRTOS.h 文件中找 configUSE_QUEUE_SETS 设置为 1，不然无法使用 */


//1.系统监控任务 - 最高优先级，负责系统状态监
void RTOS_Monitoring_Task(void *pvParameters) ; // 系统监控任务函数声明
#define RTOS_Monitoring_STACK_SIZE 256   // 堆栈大小：256字（1024字节）
#define RTOS_Monitoring_PRIORITY osPriorityNormal // 优先级：普通优先级+1
TaskHandle_t RTOS_Monitoring_Handle=NULL;     // 系统监控任务句柄初始化

static char uart_tx_buffer[512];    // 串口发送缓冲区
//2.输入任务 - 普通优先级- 处理外部输入


//3.处理任务 - 普通优先级 - 核心业务逻辑


//4.辅助任务 - 普通优先级- 显示、通信等


//2.输入处理任务 - 处理按键、传感器等输入信号


//3.数据处理任务 - 核心算法和业务逻辑处理


//4.辅助功能任务 - 显示、通信等辅助功能
void RTOS_OLED_Task(void *pvParameters);  //OLED任务函数 - 控制OLED显示
#define RTOS_OLED_STACK_SIZE 128   //堆栈大小：128字（512字节）
#define RTOS_OLED_PRIORITY osPriorityNormal //优先级：普通优先级
TaskHandle_t RTOS_OLED_Handle=NULL;    //OLED任务句柄
// OLED显示控制标志位定义
volatile uint8_t OLED_Display_Enable = 1;  // OLED显示使能标志：1-启用，0-禁用


//5.输出任务 - 控制LED、显示、执行器等输出设备
/* RTOS_LED_P13_Task 参数配置 */
void RTOS_LED_P13_Task(void *pvParameters);  //LED_PC13任务函数 - 控制PC13引脚LED闪烁
#define RTOS_LED_STACK_SIZE 128   //堆栈大小：128字（512字节）
#define RTOS_LED_PRIORITY osPriorityNormal //优先级：普通优先级
TaskHandle_t RTOS_LED_P13_Handle=NULL;    //LED_PC13任务句柄


/* RTOS_Monitoring_Task 参数配置 - 系统监控任务详细配置 */




/*===============所有参数配置区=====================*/


// ======= System task (with high priority) =========
// --------------RTOS_Monitoring_Task task-----------
// Monitoring task to print heap size information
//串口为：串口1
//PA9----TX
//PA10---RX 111
void RTOS_Monitoring_Task(void *pvParameters) 
{ 
    static uint32_t last_heap_free = 0; // Stores the last heap free size for calculating memory changes
    static uint32_t monitor_counter = 0; // Monitoring counter to track task execution cycles
    static uint8_t initialized = 0;// Initialization flag to ensure proper first-time setup
    
    // Minimum stack remaining in current monitoring period
    // Initialized to maximum value 0xFFFFFFFF for comparison
    static UBaseType_t min_stack_this_period = 0xFFFFFFFF;
    
    // First run initialization
    if(!initialized) {
        last_heap_free = xPortGetFreeHeapSize();
        initialized = 1;
    }
    
	while(1) 
	{
    monitor_counter++;
    
    uint32_t heap_free = xPortGetFreeHeapSize();
    uint32_t heap_min = xPortGetMinimumEverFreeHeapSize();
    UBaseType_t task_count = uxTaskGetNumberOfTasks();
    
    // Obtain the current task stack high watermark mark
    TaskHandle_t current_task = xTaskGetCurrentTaskHandle();
    UBaseType_t stack_mark = uxTaskGetStackHighWaterMark(current_task);
    
    // Minimum stack value within the update period (only when the current task stack is lower than the recorded value)
    if(stack_mark < min_stack_this_period) {
        min_stack_this_period = stack_mark;
    }
    UBaseType_t min_stack_remaining = stack_mark; // Use the current task stack as the minimum value
    uint8_t low_stack_tasks = (stack_mark < 50) ? 1 : 0;
    
    // Calculate memory changes 
    int32_t heap_change = (int32_t)heap_free - (int32_t)last_heap_free;
    uint32_t heap_change_abs = (heap_change > 0) ? heap_change : -heap_change;
    
    // Determine whether to print
    uint8_t should_print = (monitor_counter % 10 == 0) || (heap_change_abs > 500) || (stack_mark < 50);
    
    // Print memory information
    if(should_print) {
        char condition_info[20] = "";
        if(monitor_counter % 10 == 0) strcpy(condition_info, "[Condition 1 ]");
        else if(heap_change_abs > 500) strcpy(condition_info, "[Condition 2 ]");
        else if(stack_mark < 50) strcpy(condition_info, "[Condition 3 ]");
        
        UART_Printf("%s [%lu] H=%lu(%+ld) T=%d Smin=%u Slow=%u PeriodMin=%u\r\n",
                    condition_info, monitor_counter, heap_free, heap_change,
                    task_count, min_stack_remaining, low_stack_tasks, min_stack_this_period);
        
        // Condition 1 triggers: reset the period counter
        if(monitor_counter % 10 == 0) {
            monitor_counter = 0;
            min_stack_this_period = 0xFFFFFFFF;
        }
    } else {
        UART_Printf("System OK:[%lu] H=%lu T=%d Smin=%u Slow=%u\r\n",
                    monitor_counter, heap_free, task_count, min_stack_remaining, low_stack_tasks);
    }
    
    last_heap_free = heap_free;
    vTaskDelay(pdMS_TO_TICKS(5000)); // Ensure there is sufficient delay
	}
}
// --------------RTOS_Monitoring_Task task-----------

// ================ 输入任务区域 =================== 
/*
 * 输入任务负责处理所有外部输入信号
 * 优先级建议：中等优先级，确保及时响应外部事件
 * 堆栈大小：根据具体任务复杂度调整
 */

// ================= 数据处理任务区域 ==================== 
/*
 * 数据处理任务负责核心业务逻辑
 * 优先级建议：根据处理复杂度调整，避免阻塞高优先级任务
 * 堆栈大小：根据算法复杂度适当增大
 */

// ================= 输出任务区域 ====================
/*
 * 输出任务负责控制所有输出设备
 * 优先级建议：较低优先级，确保系统稳定性
 * 堆栈大小：根据设备控制复杂度调整
 */

//-------------------LED_PC13 任务-------------------
//LED_PC13任务
void RTOS_LED_P13_Task(void *pvParameters)   
{
  while(1)
  {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(500));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
//-------------------LED_PC13 任务-------------------

//-----------------串口发送任务-------------------
//----- UART_Printf - DMA发送 -----
void UART_Printf(const char *format, ...) {
    // 等待DMA完成
    while(__HAL_DMA_GET_COUNTER(&hdma_usart1_tx) > 0) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    va_list args;
    va_start(args, format);
    int len = vsnprintf(uart_tx_buffer, sizeof(uart_tx_buffer) - 1, format, args);
    va_end(args);
    
    if(len > 0) {
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart_tx_buffer, len);
    }
}
//-----------------串口发送任务-------------------

// =============== 辅助任务 ===================
/*
 * 辅助任务功能说明：
 * 此区域定义所有非关键的系统辅助功能任务
 * 这些任务为系统提供额外的服务和诊断功能
 * 但不影响系统的核心业务逻辑运行
 */
//-----------------OLED 更新任务-------------------
//OLED 更新任务
void RTOS_OLED_Task(void *pvParameters)   
{
  while(1)
  {
    if(OLED_Display_Enable) //查询是否需要更新
    {
      OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();

            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
            OLED_ShowString(0,0,"Welcome BASEDATA!",OLED_6X8);
      OLED_Update();
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
//-----------------OLED 更新任务-------------------

void RTOS_Create_All_Tasks(void *pvParameters)	//700ms 任务创建函数
{
  //*------------串口初始化（最高优先级）------------*//
//	UART_Init();  // 必须首先初始化UART，确保互斥锁就绪
  //*--------------------------------------------*//
  
  //*------------外设初始化------------*//
	OLED_Init();

  //*------------外设初始化------------*//
  
  // 串口内存状态检查 - 系统启动诊断
  UART_Printf("---------- 初始内存检查 ----------\r\n");
  UART_Printf("配置的总堆大小: %d 字节\r\n", configTOTAL_HEAP_SIZE);
  UART_Printf("初始可用堆内存: %d 字节\r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小可用堆: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("------------------------------------------\r\n");

  /* 所有事件组初始化开始 */
  // 在此处添加事件组创建代码
  // 例如：xEventGroupCreate();
  // 示例：EventGroupHandle_t xEventGroup = xEventGroupCreate();


  // 事件组创建后的内存检查
  UART_Printf("---------- 事件组创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("-------------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   
  /* 所有事件组初始化结束 */

  /* 所有信号量初始化开始 */
  // 在此处添加信号量创建代码
  // 例如：xSemaphoreCreateBinary(信号量名称);
  // 或者：xSemaphoreCreateMutex(信号量名称);
  // 或者：xSemaphoreCreateCounting(最大计数, 初始计数);

  // 信号量创建后的内存检查
  UART_Printf("---------- 信号量创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("-------------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   
  /* 所有信号量初始化结束 */

  /* 所有队列初始化开始 */
  // 在此处添加队列创建代码
  // 例如：xQueueCreate(队列名称, 队列大小, 项目大小);

  // 队列创建后的内存检查
  UART_Printf("---------- 队列创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("-------------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   
  /* 所有队列初始化结束 */

  /* 所有任务初始化开始 */
  // 1.系统任务
  // 在此处添加系统任务创建代码
  // 例如：xTaskCreate(系统任务, "系统任务", ...);
  
  // 系统任务创建后的内存检查
  UART_Printf("---------- 系统任务创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("----------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   

  // 3.处理任务
  // 在此处添加处理任务创建代码
  // 例如：xTaskCreate(处理任务, "处理任务", ...);
  
  // 处理任务创建后的内存检查
  UART_Printf("---------- 处理任务创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("----------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   
  
  // 4.辅助任务
  // 在此处添加辅助任务创建代码
  // 例如：xTaskCreate(辅助任务, "辅助任务", ...);
    if(xTaskCreate(RTOS_OLED_Task, "RTOS_OLED任务", RTOS_OLED_STACK_SIZE, NULL, RTOS_OLED_PRIORITY, &RTOS_OLED_Handle) != pdPASS)
      UART_Printf("RTOS_OLED任务创建失败\r\n");
  
  // 辅助任务创建后的内存检查
  UART_Printf("---------- 辅助任务创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("-------------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   

  // 5.输出任务
  // 在此处添加输出任务创建代码
  // 例如：xTaskCreate(输出任务, "输出任务", ...);
  if(xTaskCreate(RTOS_LED_P13_Task, "RTOS_LED_PC13任务", RTOS_LED_STACK_SIZE, NULL, RTOS_LED_PRIORITY, &RTOS_LED_P13_Handle) != pdPASS)
      UART_Printf("RTOS_LED_P13任务创建失败\r\n");
  if(xTaskCreate(RTOS_Monitoring_Task, "RTOS_监控任务", RTOS_Monitoring_STACK_SIZE, NULL, RTOS_Monitoring_PRIORITY, &RTOS_Monitoring_Handle) != pdPASS)
      UART_Printf("RTOS_监控任务创建失败\r\n");
  
  // 输出任务创建后的内存检查
  UART_Printf("---------- 输出任务创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("----------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   
  
  /* 所有软件定时器初始化开始 */
  // 在此处添加软件定时器创建代码
  // 例如：xTimerCreate(), xTimerStart()
  
  // 软件定时器创建后的内存检查
  UART_Printf("---------- 软件定时器创建后内存检查 ----------\r\n");
  UART_Printf("当前可用内存: %d 字节 \r\n", xPortGetFreeHeapSize());
  UART_Printf("历史最小内存: %d 字节\r\n", xPortGetMinimumEverFreeHeapSize());
  UART_Printf("----------------------------------------\r\n");
  vTaskDelay(pdMS_TO_TICKS(100));   
    
    /* 所有软件定时器初始化结束 */
	vTaskDelay(pdMS_TO_TICKS(100));
	UART_Printf("<<<Delete this task and Free the memory>>>\r\n");  
  vTaskDelete(NULL); //删除此任务
}


