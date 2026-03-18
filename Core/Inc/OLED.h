#ifndef __OLED_H
#define __OLED_H

#include "OLED_Data.h"



/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

////范围设置
//#define zuoshang 0,0
//#define miny 0
//#define minx 0
//#define youxia 63,127
//#define maxy 63
//#define maxx 127
////申明显存
//extern u8 bufdis[(maxy+1)*(maxx+1)/8];

//void Clearbuffer(void);
//void OLED_Display(void);
//void Clearbuffer_bc(u8 y);
//void PlacePixel(uint8_t y, uint8_t x);
//void PlacePixel_back(uint8_t y, uint8_t x);
//void Placecharbit(uint8_t x, uint8_t y,uint8_t cha);
//void Placechar(uint8_t x, uint8_t y,uint8_t cha);
//void PlaceString(uint8_t Line, uint8_t Column, char *String);
//void Placechar_F8x16(uint8_t x, uint8_t y,uint8_t chr);
//void PlaceString_F8x16(uint8_t Line, uint8_t Column, char *String);
//void printf_Place(char a,char b,char *stringg, ...);
//void printf_Place_F8x16(char a,char b,char *stringg, ...);
//void printf_Place_F8x16_back(char a,char b,char *stringg, ...);
//void PlaceChinese_Fc32(uint8_t x, uint8_t y,uint8_t chr);
//void PlaceChinese_Fc32_back(uint8_t x, uint8_t y,uint8_t chr);
//void rectangle(uint8_t y1, uint8_t x1,uint8_t y2, uint8_t  x2);
//void rectangle_back(uint8_t y1, uint8_t x1,uint8_t y2, uint8_t  x2);
//void Placeline(uint8_t x1, uint8_t y1,uint8_t x2, uint8_t y2);
//void PlaceCyuan(short y1, short x1,float R);
//void FillCircle(short x1, short y1, short R);
//void Placedashed_line(char y);
//void PlaceVerticalline(char x);
//void PlotWave(char plot,char tongdao,unsigned short pianyi,short chufa);






/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
