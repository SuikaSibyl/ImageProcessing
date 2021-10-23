#ifndef _BmpFormat
#define _BmpFormat

#include<stdio.h>
#include<malloc.h>

#define   WIDTHBYTES(bits) (((bits)+31)/32*4)   
#define GETVALUE(a) ((a)>255?255:((a)<0?0:(a)))
#pragma warning(disable:4996)
#pragma pack(1) 

//////////////////////////////////////////////////
/////////////////////������Ϣ/////////////////////
//////////////////////////////////////////////////

/*���弯�л�ʹ�õĻ������ͣ�2�ֽڣ�4�ֽ�*/
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;

/*λͼ�ļ�ͷ����Ҫ�Ƕ�λͼ�ļ���һЩ����,�̶���14���ֽ�*/
typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;      // �ļ���ʶ�� ,����'BM'��˵����һ��bmp�ļ�
	DWORD bfSize;     // �ļ��Ĵ�С��������14���ֽ�
	WORD bfReserved1; // ����ֵ,��������Ϊ0  
	WORD bfReserved2; // ����ֵ,��������Ϊ0 
	DWORD bfOffset;   // ���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ַ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
/*λͼ��Ϣͷ����Ҫ�Ƕ�λͼͼ������Ϣ���������̶���40���ֽ�*/
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;			  	 // ��Ϣͷ�Ĵ�С��40 
	DWORD biWidth;               // ͼ��Ŀ�ȣ���������Ŀ
	DWORD biHeight;              // ͼ��ĸ߶�(���ʾ�������������¸�������)     
	WORD biPlanes;               // ͼ���λ����������1
	WORD biBitCount;             // ÿ�����ص�λ�� 1��4��5��16��24��32
	DWORD biCompression;         // ѹ������ (δѹ����BI_RGB?)
	DWORD biSizeImage;           // ָ��ʵ��λͼ����ռ�õ��ֽ���||��������biSizeImage=biWidth'*biHeight,�����Ǵ��ڻ����biWidth����ӽ�4��������||��ѹ������Ϊ0
	DWORD biXPelsPerMeter;       // ˮƽ�ֱ��ʣ�ÿ��������
	DWORD biYPelsPerMeter;       // ��ֱ�ֱ��ʣ�ÿ��������
	DWORD biClrUsed;             // ʹ�õ�ɫ���� 
	DWORD biClrImportant;        // ��Ҫ����ɫ�� ��Ϊ0ʱ��Ϊ������ɫ��Ҫ
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
/* ��ɫ��:��ɫ�� */
typedef struct tagPALLATE
{
	BYTE rgbBlue;     // ��ɫ
	BYTE rgbGreen;    // ��ɫ
	BYTE rgbRed;      // ��ɫ
	BYTE rgbReversed; // ����ֵ��������Ϊ0
}*PALLETE;
/*���ڱ���һ�����ص�RGB����*/
typedef struct tagRGBQUAD
{
	BYTE rgbBlue;     // ��ɫ
	BYTE rgbGreen;    // ��ɫ
	BYTE rgbRed;      // ��ɫ
	BYTE rgbReversed; // ����ֵ��������Ϊ0
} *RGBQUAD;
/*���ڱ���һ�����ص�YUV����*/
typedef struct tagYUVQUAD
{
	BYTE yuvY;
	char yuvU;
	char yuvV;
} *YUVQUAD;
/*���ڱ���һ��BMPͼƬ����������*/
typedef struct tagBMPIMAGE
{
	PBITMAPFILEHEADER FileHeader;
	PBITMAPINFOHEADER InfoHeader;
	PALLETE Pallete;
	RGBQUAD RgbData;
}*BmpImage;


//////////////////////////////////////////////////
//////////////////////��̬ѧ//////////////////////
//////////////////////////////////////////////////

/*���ڱ���һ���ṹԪ*/
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