#ifndef _BmpFormat
#define _BmpFormat

#include<stdio.h>
#include<malloc.h>

#define   WIDTHBYTES(bits) (((bits)+31)/32*4)   
#define GETVALUE(a) ((a)>255?255:((a)<0?0:(a)))
#pragma warning(disable:4996)
#pragma pack(1) 

//////////////////////////////////////////////////
/////////////////////基本信息/////////////////////
//////////////////////////////////////////////////

/*定义集中会使用的基本类型，2字节，4字节*/
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

/*位图文件头，主要是对位图文件的一些描述,固定是14个字节*/
typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;      // 文件标识符 ,会是'BM'以说明是一个bmp文件
	DWORD bfSize;     // 文件的大小，包括这14个字节
	WORD bfReserved1; // 保留值,必须设置为0  
	WORD bfReserved2; // 保留值,必须设置为0 
	DWORD bfOffset;   // 从文件头到实际位图数据的偏移字符数，即前三个部分长度之和
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
/*位图信息头，主要是对位图图像方面信息的描述，固定是40个字节*/
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;			  	 // 信息头的大小，40 
	DWORD biWidth;               // 图像的宽度，像素总数目
	DWORD biHeight;              // 图像的高度(另表示正反，正数导致负数正置)     
	WORD biPlanes;               // 图像的位面数，总是1
	WORD biBitCount;             // 每个像素的位数 1，4，5，16，24，32
	DWORD biCompression;         // 压缩类型 (未压缩，BI_RGB?)
	DWORD biSizeImage;           // 指定实际位图数据占用的字节数||可以算作biSizeImage=biWidth'*biHeight,必须是大于或等于biWidth的最接近4的整数倍||无压缩可能为0
	DWORD biXPelsPerMeter;       // 水平分辨率，每米像素数
	DWORD biYPelsPerMeter;       // 垂直分辨率，每米像素数
	DWORD biClrUsed;             // 使用的色彩数 
	DWORD biClrImportant;        // 重要的颜色数 ，为0时认为所有颜色重要
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
/* 彩色表:调色板 */
typedef struct tagPALLATE
{
	BYTE rgbBlue;     // 蓝色
	BYTE rgbGreen;    // 绿色
	BYTE rgbRed;      // 红色
	BYTE rgbReversed; // 保留值，总设置为0
}*PALLETE;
/*用于保存一个像素的RGB数据*/
typedef struct tagRGBQUAD
{
	BYTE rgbBlue;     // 蓝色
	BYTE rgbGreen;    // 绿色
	BYTE rgbRed;      // 红色
	BYTE rgbReversed; // 保留值，总设置为0
} *RGBQUAD;
/*用于保存一个像素的YUV数据*/
typedef struct tagYUVQUAD
{
	BYTE yuvY;
	char yuvU;
	char yuvV;
} *YUVQUAD;
/*用于保存一个BMP图片的所有数据*/
typedef struct tagBMPIMAGE
{
	PBITMAPFILEHEADER FileHeader;
	PBITMAPINFOHEADER InfoHeader;
	PALLETE Pallete;
	RGBQUAD RgbData;
}*BmpImage;


//////////////////////////////////////////////////
//////////////////////形态学//////////////////////
//////////////////////////////////////////////////

/*用于保存一个结构元*/
typedef struct
{
	int* x;
	int* y;
	int len;
	int r;
}StrElem;

enum Interpolation { Nearestneighbor, Bilinear};
#pragma pack() 
#endif