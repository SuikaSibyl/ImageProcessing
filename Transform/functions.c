#include "BmpDefine.h"
#include<math.h>
#include<memory.h>
#include<string.h>

#define GETVALUE(a) ((a)>255?255:((a)<0?0:(a)))
#define Coordinate2Array(x,y) ((x)+(y)*Im->InfoHeader->biWidth)
#define Coordinate2Array_out(x,y) ((x)+(y)*Im_out->InfoHeader->biWidth)
#define Pi 3.1416

typedef struct {
	int x;
	int y;
}Point;
typedef struct {
	double x;
	double y;
}dPoint;
//////////////////////////////////////////////////
/////////////////////��������/////////////////////
//////////////////////////////////////////////////
BmpImage ReadBMP(char *s)
{
	FILE *fp;
	BmpImage Im = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));
	Im->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	if ((fp = fopen(s, "rb")) == NULL)		//��HelloWorld.bmp��ʧ�ܷ��ش�����Ϣ
	{
		printf("Opening occurs Error!\n");
	}
	fseek(fp, 0, SEEK_SET);									//����fread�����ļ�ͷ����Ϣͷ
	fread(Im->FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(Im->InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if (Im->InfoHeader->biBitCount < 24)
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		Im->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*num);
		for (int i = 0; i < num; i++)
		{
			fread(&(Im->Pallete[i].rgbBlue), sizeof(BYTE), 1, fp);
			fread(&(Im->Pallete[i].rgbGreen), sizeof(BYTE), 1, fp);
			fread(&(Im->Pallete[i].rgbRed), sizeof(BYTE), 1, fp);
			fread(&(Im->Pallete[i].rgbReversed), sizeof(BYTE), 1, fp);
		}
	}
	else Im->Pallete = NULL;

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	BYTE *pColorData = (BYTE *)malloc(height*l_width);
	long nData = height * l_width;
	fread(pColorData, 1, nData, fp);

	Im->RgbData = (RGBQUAD)malloc(width*height * sizeof(struct tagRGBQUAD));
	if (Im->InfoHeader->biBitCount < 24)//�е�ɫ�壬��λͼΪ�����ɫ
	{
		int k;
		int index = 0;
		if (Im->InfoHeader->biBitCount == 1)
		{
			int k;
			int index = 0;
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 8;//k:ȡ�ø�������ɫ������ʵ�����������е����
					mixIndex = pColorData[k];//mixIndex:��ȡ��ǰ���ص���ɫ�ľ���ֵ
					if (k == 2755)
					{
						printf("Hello!");
					}
					switch (j % 8)
					{
					case 7:
						mixIndex = mixIndex << 7;
						mixIndex = mixIndex >> 7;
						break;
					case 6:
						mixIndex = mixIndex << 6;
						mixIndex = mixIndex >> 7;
						break;
					case 5:
						mixIndex = mixIndex << 5;
						mixIndex = mixIndex >> 7;
						break;

					case 4:
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 7;
						break;
					case 3:
						mixIndex = mixIndex << 3;
						mixIndex = mixIndex >> 7;
						break;

					case 2:
						mixIndex = mixIndex << 2;
						mixIndex = mixIndex >> 7;
						break;
					case 1:
						mixIndex = mixIndex << 1;
						mixIndex = mixIndex >> 7;
						break;

					case 0:
						mixIndex = mixIndex >> 7;
						break;
					}
					//���������ݱ��浽�����ж�Ӧ��λ��
					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;

				}
		}
		if (Im->InfoHeader->biBitCount == 2)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 4;//k:ȡ�ø�������ɫ������ʵ�����������е����
					//j:��ȡ��ǰ���ص���ɫ�ľ���ֵ
					mixIndex = pColorData[k];
					switch (j % 4)
					{
					case 0:
						mixIndex = mixIndex << 6;
						mixIndex = mixIndex >> 6;
						break;
					case 1:
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 6;
						break;
					case 2:
						mixIndex = mixIndex << 2;
						mixIndex = mixIndex >> 6;
						break;
					case 3:
						mixIndex = mixIndex >> 6;
						break;
					}

					//���������ݱ��浽�����ж�Ӧ��λ��
					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;


				}
		}
		if (Im->InfoHeader->biBitCount == 4)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 2;
					mixIndex = pColorData[k];
					if (j % 2 == 0)
					{//��
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 4;
					}
					else
					{//��
						mixIndex = mixIndex >> 4;
					}

					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;

				}

		}
		if (Im->InfoHeader->biBitCount == 8)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j;
					mixIndex = pColorData[k];
					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;
				}
		}
		if (Im->InfoHeader->biBitCount == 16)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					WORD mixIndex = 0;

					k = i * l_width + j * 2;
					WORD shortTemp;
					shortTemp = pColorData[k + 1];
					shortTemp = shortTemp << 8;

					mixIndex = pColorData[k] + shortTemp;

					Im->RgbData[index].rgbRed = Im->Pallete[mixIndex].rgbRed;
					Im->RgbData[index].rgbGreen = Im->Pallete[mixIndex].rgbGreen;
					Im->RgbData[index].rgbBlue = Im->Pallete[mixIndex].rgbBlue;
					Im->RgbData[index].rgbReversed = Im->Pallete[mixIndex].rgbReversed;
					index++;
				}
		}
	}
	else//λͼΪ24λ���ɫ
	 {
		 int k;
		 int index = 0;
		 for (int i = 0; i < height; i++)
			 for (int j = 0; j < width; j++)
			 {
				 k = i * l_width + j * 3;
				 Im->RgbData[index].rgbRed = pColorData[k + 2];
				 Im->RgbData[index].rgbGreen = pColorData[k + 1];
				 Im->RgbData[index].rgbBlue = pColorData[k];
				 Im->RgbData[index].rgbReversed = 0;
				 index++;
			 }
	 }
	 fclose(fp);
	 return Im;
}
YUVQUAD RGB2YUV(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	YUVQUAD YUV = (YUVQUAD)malloc(width*height * sizeof(struct tagYUVQUAD));
	for (int i = 0; i < width*height; i++)
	{
		YUV[i].yuvY = (BYTE)(0.114*Im->RgbData[i].rgbBlue + 0.587*Im->RgbData[i].rgbGreen + 0.299*Im->RgbData[i].rgbRed);		//Ϊÿ����Y��ֵ
		YUV[i].yuvU = (BYTE)(0.437*Im->RgbData[i].rgbBlue - 0.289*Im->RgbData[i].rgbGreen - 0.148*Im->RgbData[i].rgbRed);		//Ϊÿ����U��ֵ
		YUV[i].yuvV = (BYTE)(-0.100*Im->RgbData[i].rgbBlue - 0.515*Im->RgbData[i].rgbGreen + 0.615*Im->RgbData[i].rgbRed);
	}
	return YUV;
}
void YUV2RGB(BmpImage Im, YUVQUAD yuvquad)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	for (int i = 0; i < width*height; i++)
	{
		Im->RgbData[i].rgbRed = (BYTE)(GETVALUE(yuvquad[i].yuvY + 1.140*yuvquad[i].yuvV));
		Im->RgbData[i].rgbGreen = (BYTE)(GETVALUE(yuvquad[i].yuvY - 0.395*yuvquad[i].yuvU - 0.581*yuvquad[i].yuvV));
		Im->RgbData[i].rgbBlue = (BYTE)(GETVALUE(yuvquad[i].yuvY + 2.032*yuvquad[i].yuvU));
	}
}
void WriteBMP(BmpImage Im, char* s)
{
	FILE* fp = fopen(s, "wb");
	fwrite(Im->FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(Im->InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//����λͼ��ʵ�ʿ�Ȳ�ȷ����Ϊ32�ı���
	BYTE *pColorData = (BYTE *)malloc(height*l_width);
	memset(pColorData, 0, height*l_width);
	long nData = height * l_width;

	if (Im->Pallete != NULL)
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		for (int i = 0; i < num; i++)
		{
			fwrite(&(Im->Pallete[i].rgbBlue), sizeof(BYTE), 1, fp);
			fwrite(&(Im->Pallete[i].rgbGreen), sizeof(BYTE), 1, fp);
			fwrite(&(Im->Pallete[i].rgbRed), sizeof(BYTE), 1, fp);
			fwrite(&(Im->Pallete[i].rgbReversed), sizeof(BYTE), 1, fp);
		}
	}

	if (Im->InfoHeader->biBitCount < 24)
	{
		int k;
		int index = 0;
		if (Im->InfoHeader->biBitCount == 1)
		{
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					BYTE mixIndex = 0;
					k = i * l_width + j / 8;//k:ȡ�ø�������ɫ������ʵ�����������е����
					if (Im->RgbData[index].rgbRed == 255)
						mixIndex = 1;
					switch (j % 8)
					{
					case 0:
						mixIndex = mixIndex << 7;
						break;
					case 1:
						mixIndex = mixIndex << 6;
						break;
					case 2:
						mixIndex = mixIndex << 5;
						break;

					case 3:
						mixIndex = mixIndex << 4;
						break;
					case 4:
						mixIndex = mixIndex << 3;
						break;

					case 5:
						mixIndex = mixIndex << 2;
						break;
					case 6:
						mixIndex = mixIndex << 1;
						break;

					case 7:
						mixIndex = mixIndex;
						break;
					}
					pColorData[k] += mixIndex;
					index++;
				}
		}

	}
	else//λͼΪ24λ���ɫ
	{
		int k;
		int index = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				k = i * l_width + j * 3;
				pColorData[k + 2] = Im->RgbData[index].rgbRed;
				pColorData[k + 1] = Im->RgbData[index].rgbGreen;
				pColorData[k] = Im->RgbData[index].rgbBlue;
				index++;
			}
			/*for (int j= i * l_width + width * 3; j < i * l_width+l_width; j++)
			{
				pColorData[j] = 0;
			}*/
		}
	}
	fwrite(pColorData, 1, nData, fp);
}
BmpImage BmpCpy(BmpImage Im)
{
	BmpImage Im_out = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));

	Im_out->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im_out->FileHeader->bfOffset = Im->FileHeader->bfOffset;
	Im_out->FileHeader->bfReserved1 = Im->FileHeader->bfReserved1;
	Im_out->FileHeader->bfReserved2 = Im->FileHeader->bfReserved2;
	Im_out->FileHeader->bfSize = Im->FileHeader->bfSize;
	Im_out->FileHeader->bfType = Im->FileHeader->bfType;

	Im_out->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	Im_out->InfoHeader->biBitCount = Im->InfoHeader->biBitCount;
	Im_out->InfoHeader->biClrImportant = Im->InfoHeader->biClrImportant;
	Im_out->InfoHeader->biClrUsed = Im->InfoHeader->biClrUsed;
	Im_out->InfoHeader->biCompression = Im->InfoHeader->biCompression;
	Im_out->InfoHeader->biHeight = Im->InfoHeader->biHeight;
	Im_out->InfoHeader->biPlanes = Im->InfoHeader->biPlanes;
	Im_out->InfoHeader->biSize = Im->InfoHeader->biSize;
	Im_out->InfoHeader->biSizeImage = Im->InfoHeader->biSizeImage;
	Im_out->InfoHeader->biWidth = Im->InfoHeader->biWidth;
	Im_out->InfoHeader->biXPelsPerMeter = Im->InfoHeader->biXPelsPerMeter;
	Im_out->InfoHeader->biYPelsPerMeter = Im->InfoHeader->biYPelsPerMeter;

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;

	if (Im->Pallete != NULL)
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		Im_out->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*num);
		memcpy(Im_out->Pallete, Im->Pallete, sizeof(struct tagPALLATE)*num);
	}
	else Im_out->Pallete = NULL;

	Im_out->RgbData = (RGBQUAD)malloc(width*height * sizeof(struct tagRGBQUAD));
	memcpy(Im_out->RgbData, Im->RgbData, width*height * sizeof(struct tagRGBQUAD));

	return Im_out;
}
//////////////////////////////////////////////////
/////////////////////�ҶȲ���/////////////////////
//////////////////////////////////////////////////
void FullColor2Grayscale(BmpImage Im)
{
	YUVQUAD yuv = RGB2YUV(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	for (int i = 0; i < width*height; i++)
	{
		Im->RgbData[i].rgbBlue = Im->RgbData[i].rgbGreen = Im->RgbData[i].rgbRed = yuv[i].yuvY;
	}
}
//////////////////////////////////////////////////
/////////////////////��������/////////////////////
//////////////////////////////////////////////////
void RgbCopy(RGBQUAD X, RGBQUAD Y, int xvalue,int yvalue)
{
	//����ͼYһ������(yvalue)��RGB��Ϣ��ͼX��xvalue����
	X[xvalue].rgbBlue = Y[yvalue].rgbBlue;
	X[xvalue].rgbRed = Y[yvalue].rgbRed;
	X[xvalue].rgbGreen = Y[yvalue].rgbGreen;
}
Point Get_Nearestneighbor(double x,double y)
{
	//Ѱ��double��(x,y)�������
	Point P;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	P.x = (x - (int)x > 0.5) ? (int)x + 1 : (int)x;
	P.y = (y - (int)y > 0.5) ? (int)y + 1 : (int)y;
	return P;
}
void Get_Bilinear(RGBQUAD X, RGBQUAD Y,double x,double y, int P,int P11,int P12,int P21,int P22)
{
	//Ѱ��Y��double��(x,y)��˫����ֵ��������X�е�P��ע�⣬P11,P12,P21,P22Ϊdouble���ڵ�
	struct tagRGBQUAD R1, R2;
	X[P].rgbBlue = (1 - x)*(1 - y)*Y[P11].rgbBlue + x * (1 - y)*Y[P21].rgbBlue +\
		(1 - x)*y*Y[P22].rgbBlue+x*y*Y[P22].rgbBlue;
	X[P].rgbRed = (1 - x)*(1 - y)*Y[P11].rgbRed + x * (1 - y)*Y[P21].rgbRed + \
		(1 - x)*y*Y[P22].rgbRed + x * y*Y[P22].rgbRed;
	X[P].rgbGreen = (1 - x)*(1 - y)*Y[P11].rgbGreen + x * (1 - y)*Y[P21].rgbGreen + \
		(1 - x)*y*Y[P22].rgbGreen + x * y*Y[P22].rgbGreen;
}
Point SizeAfterRotation(double angle, BmpImage Im)
{
	//�����תangle�Ⱥ�����Ҫ�Ļ�����С
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	dPoint P1; P1.x = width / 2; P1.y = height / 2;
	dPoint P2; 
	P2.x = P1.x*cos(angle / 180 * 3.1416) - P1.y*sin(angle / 180 * 3.1416);	//��һ������Զ����ת������
	P2.y = P1.x*sin(angle / 180 * 3.1416) + P1.y*cos(angle / 180 * 3.1416);
	int width_1 = abs(P2.x >= 0 ? (int)(P2.x + 0.5) : (int)(P2.x - 0.5));
	int height_1 = abs(P2.y >= 0 ? (int)(P2.y + 0.5) : (int)(P2.y - 0.5));
	P2.x = -P1.x*cos(angle / 180 * 3.1416) - P1.y*sin(angle / 180 * 3.1416);//�ڶ�������Զ����ת������
	P2.y = -P1.x*sin(angle / 180 * 3.1416) + P1.y*cos(angle / 180 * 3.1416);
	int width_2 = abs(P2.x >= 0 ? (int)(P2.x + 0.5) : (int)(P2.x - 0.5));
	int height_2 = abs(P2.y >= 0 ? (int)(P2.y + 0.5) : (int)(P2.y - 0.5));
	Point P;
	P.x = 2*(width_1 >= width_2 ? width_1 : width_2);	//��ȱ�Ϊ���xֵ������
	P.y = 2*(height_1 > height_2 ? height_1 : height_2);	//�߶ȱ�Ϊ���yֵ������
	return P;
}
//////////////////////////////////////////////////
/////////////////////ͼ�β���/////////////////////
//////////////////////////////////////////////////
BmpImage ResizeCanva(int x, int y, BmpImage Im)
{
	//���½���Ϊx��Ϊy�Ļ��������ߴ���������������Ϣ��Im��ͬ������ȫΪ(0,0,0)
	BmpImage Im_out = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));
	Im_out->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im_out->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im_out->FileHeader->bfOffset = Im->FileHeader->bfOffset;	//����FileHeader
	Im_out->FileHeader->bfReserved1 = Im->FileHeader->bfReserved1;
	Im_out->FileHeader->bfReserved2 = Im->FileHeader->bfReserved2;
	Im_out->FileHeader->bfSize = Im->FileHeader->bfSize + (x*y - Im->InfoHeader->biWidth*Im->InfoHeader->biHeight)*Im->InfoHeader->biBitCount;
	Im_out->FileHeader->bfType = Im->FileHeader->bfType;	//bfSize�ᷢ���ı䣬��������
	Im_out->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));	//����InfoHeader
	Im_out->InfoHeader->biBitCount = Im->InfoHeader->biBitCount;
	Im_out->InfoHeader->biClrImportant = Im->InfoHeader->biClrImportant;
	Im_out->InfoHeader->biClrUsed = Im->InfoHeader->biClrUsed;
	Im_out->InfoHeader->biCompression = Im->InfoHeader->biCompression;
	Im_out->InfoHeader->biHeight = y;	//���ĸ߶���Ϣ
	Im_out->InfoHeader->biPlanes = Im->InfoHeader->biPlanes;
	Im_out->InfoHeader->biSize = Im->InfoHeader->biSize;
	Im_out->InfoHeader->biSizeImage = Im->InfoHeader->biSizeImage;
	Im_out->InfoHeader->biWidth = x;	//���Ŀ����Ϣ
	Im_out->InfoHeader->biXPelsPerMeter = Im->InfoHeader->biXPelsPerMeter;
	Im_out->InfoHeader->biYPelsPerMeter = Im->InfoHeader->biYPelsPerMeter;
	if (Im->Pallete != NULL)	//���е�ɫ�壬����
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		Im_out->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*num);
		memcpy(Im_out->Pallete, Im->Pallete, sizeof(struct tagPALLATE)*num);
	}
	else Im_out->Pallete = NULL;	
	Im_out->RgbData = (RGBQUAD)malloc(x*y * sizeof(struct tagRGBQUAD));	//RGB��ϢȫΪ(0,0,0)
	memset(Im_out->RgbData, 0, x*y * sizeof(struct tagRGBQUAD));
	return Im_out;
}
BmpImage Translation(int x, int y, BmpImage Im)
{
	//ƽ�Ʋ���������xΪ���ң�yΪ���Ϸ���
	BmpImage Im_out = ResizeCanva(abs(x) + Im->InfoHeader->biWidth, abs(y) + Im->InfoHeader->biHeight, Im);	//�½�����
	int zerox = (x >= 0 ? x : 0);	//����x<0ʱ����Ϊ�����ƶ������ԭ�����ڻ�������
	int zeroy = (y >= 0 ? y : 0);	//����y<0ʱ����Ϊ�����ƶ������ԭ�����ڻ�������
	for (int i = zerox; i < zerox + Im->InfoHeader->biWidth; i++)
		for (int j = zeroy; j < zeroy + Im->InfoHeader->biHeight; j++)
			RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(i - zerox, j - zeroy));	//�Ӷ�Ӧ���ؿ���
	return Im_out;
}
BmpImage Scale(double c, double d, BmpImage Im, enum Interpolation Ip)
{
	//���Ų�����cΪx�������űȣ�dΪy�������űȣ�ImΪ����ͼƬ��IpΪ��ֵ��ʽ
	Point P = Get_Nearestneighbor(c*(Im->InfoHeader->biWidth), d*(Im->InfoHeader->biHeight));	//Ѱ�ұ߽�
	BmpImage Im_out = ResizeCanva(P.x, P.y, Im);	//�½�����
	int width = Im_out->InfoHeader->biWidth; int oriwidth = Im->InfoHeader->biWidth;
	int height = Im_out->InfoHeader->biHeight;	int oriheight = Im->InfoHeader->biHeight;
	if (Ip == Nearestneighbor)	//����������ڷ�
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				P = Get_Nearestneighbor((double)i / c, (double)j / d);	//Ѱ�������
				RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(P.x, P.y));	//����
			}
		return Im_out;
	}
	else if (Ip = Bilinear)	//������˫���Է�
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				double orix = (double)i / c;	//Ѱ��doubleֵx
				double oriy = (double)j / d;	//Ѱ��doubleֵy
				if (floor(orix) >= 0 && ceil(orix) < oriwidth && floor(oriy) >= 0 && ceil(oriy) < oriheight)
				{
					//����˫���Դ�
					Get_Bilinear(Im_out->RgbData, Im->RgbData, orix - floor(orix), oriy - floor(oriy), Coordinate2Array_out(i, j), Coordinate2Array(floor(orix), floor(oriy)), \
						Coordinate2Array(floor(orix), ceil(oriy)), Coordinate2Array(ceil(orix), floor(oriy)), Coordinate2Array(ceil(orix), ceil(oriy)));
				}
			}
		return Im_out;
	}
}
BmpImage Rotation(double angle, BmpImage Im, enum Interpolation Ip)
{
	//��ת��������ʱ����תangle�ȣ�Im����ͼƬ��IpΪ��ֵ��ʽ
	Point P = SizeAfterRotation(angle, Im);	//�õ���ת�󻭲���С
	BmpImage Im_out = ResizeCanva(P.x, P.y, Im);	//�½�����
	int width = Im_out->InfoHeader->biWidth; int oriwidth = Im->InfoHeader->biWidth;
	int height = Im_out->InfoHeader->biHeight;	int oriheight = Im->InfoHeader->biHeight;
	if (Ip == Nearestneighbor)	//�����������ڲ�ֵ
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				double newx = i - (double)width / 2;
				double newy = j - (double)height / 2;
				double orix = cos(angle / 180 * Pi)*newx + sin(angle / 180 * Pi)*newy;	//���任�õ�ԭͼ����
				double oriy = cos(angle / 180 * Pi)*newy - sin(angle / 180 * Pi)*newx;
				orix = (double)(oriwidth) / 2 + orix;	//��������ϵ������������ϵ����ʵ����ϵ
				oriy = (double)(oriheight) / 2 + oriy;
				if (orix >= 0 && orix < oriwidth && oriy >= 0 && oriy < oriheight)
				{
					Point temp = Get_Nearestneighbor(orix, oriy);	//Ѱ�������
					if (temp.x >= 0 && temp.x < oriwidth && temp.y >= 0 && temp.y < oriheight)
					{
						RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(temp.x, temp.y));
					}
				}
			}
	}
	else if (Ip == Bilinear)	//�������˫���Բ�ֵ
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				double newx = i - (double)width / 2;
				double newy = j - (double)height / 2;
				double orix = cos(angle / 180 * Pi)*newx + sin(angle / 180 * Pi)*newy;//���任�õ�ԭͼ����
				double oriy = cos(angle / 180 * Pi)*newy - sin(angle / 180 * Pi)*newx;
				orix = (double)(oriwidth) / 2 + orix;	//��������ϵ������������ϵ����ʵ����ϵ
				oriy = (double)(oriheight) / 2 + oriy;
				if (floor(orix) >= 0 && ceil(orix) < oriwidth && floor(oriy) >= 0 && ceil(oriy) < oriheight)
				{
					Get_Bilinear(Im_out->RgbData, Im->RgbData, orix - floor(orix), oriy - floor(oriy), Coordinate2Array_out(i, j), Coordinate2Array(floor(orix), floor(oriy)), \
						Coordinate2Array(floor(orix), ceil(oriy)), Coordinate2Array(ceil(orix), floor(oriy)), Coordinate2Array(ceil(orix), ceil(oriy)));
				}
			}
	}
	return Im_out;
}
BmpImage Shear(char axis, double d, BmpImage Im, enum Interpolation Ip)
{
	//���в�����axis�涨����������ᣬd�涨���б�����ImΪ����ͼƬ��IpΪ��ֵ����
	BmpImage Im_out = NULL;
	int oriwidth = Im->InfoHeader->biWidth;
	int oriheight = Im->InfoHeader->biHeight;
	if (axis == 'x')
	{
		int width=(oriwidth + fabs(d) * oriheight +0.5);	//�õ��µĿ��
		int height = oriheight;
		Im_out = ResizeCanva(width, height, Im);	//�½�����
		if (Ip == Nearestneighbor)		//������������
		{
			for (int i = 0; i < width; i++)
				for (int j = 0; j < height; j++)
				{
					double orix = (i - (d > 0 ? d : 0)* oriheight + d * j);	//���任�õ�ԭ����
					double oriy = j;
					if (orix >= 0 && orix < oriwidth && oriy >= 0 && oriy < oriheight)
					{
						//�ж�������Ƿ��ڷ�Χ��
						Point temp = Get_Nearestneighbor(orix, oriy);
						if (temp.x >= 0 && temp.x < oriwidth && temp.y >= 0 && temp.y < oriheight)
						{
							RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(temp.x, temp.y));
						}
					}
				}
		}
		else if (Ip == Bilinear)	//���ȡ˫����
		{
			for (int i = 0; i < width; i++)
				for (int j = 0; j < height; j++)
				{
					double orix = (i - (d > 0 ? d : 0)* oriheight + d * j);	//���任�õ�ԭ����
					double oriy = j;
					if (floor(orix) >= 0 && ceil(orix) < oriwidth && floor(oriy) >= 0 && ceil(oriy) < oriheight)
					{
						Get_Bilinear(Im_out->RgbData, Im->RgbData, orix - floor(orix), oriy - floor(oriy), Coordinate2Array_out(i, j), Coordinate2Array(floor(orix), floor(oriy)), \
							Coordinate2Array(floor(orix), ceil(oriy)), Coordinate2Array(ceil(orix), floor(oriy)), Coordinate2Array(ceil(orix), ceil(oriy)));
					}
				}
		}
	}
	else	//���axis==y��
	{
		int width = oriwidth ;
		int height = oriheight + fabs(d) * oriwidth + 0.5;	//�õ��µĸ߶�
		Im_out = ResizeCanva(width, height, Im);	//�ؽ�����
		if (Ip == Nearestneighbor)	//������������
		{
			for (int i = 0; i < width; i++)
				for (int j = 0; j < height; j++)
				{
					double orix = i;
					double oriy = j- (d > 0 ? d : 0)* oriwidth+ d * i;	//���任��ԭ������Ϣ
					if (orix >= 0 && orix < oriwidth && oriy >= 0 && oriy < oriheight)
					{
						Point temp = Get_Nearestneighbor(orix, oriy);	//Ѱ�������
						if (temp.x >= 0 && temp.x < oriwidth && temp.y >= 0 && temp.y < oriheight)	//�ж�������Ƿ����
						{
							RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(temp.x, temp.y));
						}
					}
				}
		}
		else if (Ip == Bilinear)	//�������˫����
		{
			for (int i = 0; i < width; i++)
				for (int j = 0; j < height; j++)
				{
					double orix = i;	
					double oriy = j - (d > 0 ? d : 0)* oriwidth + d * i;	//���任��ԭ������Ϣ
					if (floor(orix) >= 0 && ceil(orix) < oriwidth && floor(oriy) >= 0 && ceil(oriy) < oriheight)
					{
						Get_Bilinear(Im_out->RgbData, Im->RgbData, orix - floor(orix), oriy - floor(oriy), Coordinate2Array_out(i, j), Coordinate2Array(floor(orix), floor(oriy)), \
							Coordinate2Array(floor(orix), ceil(oriy)), Coordinate2Array(ceil(orix), floor(oriy)), Coordinate2Array(ceil(orix), ceil(oriy)));
					}
				}
		}
	}
	return Im_out;
}
BmpImage Mirror(char axis, BmpImage Im)
{
	//���������axisѡȡ�Գ���
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	BmpImage Im_out = ResizeCanva(width, height, Im);
	if (axis == 'x')	//��xΪ�Գ���
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(width - 1 - i, j));
			}
		return Im_out;
	}
	else if (axis == 'y')	//��yΪ�Գ���
	{
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				RgbCopy(Im_out->RgbData, Im->RgbData, Coordinate2Array_out(i, j), Coordinate2Array(i ,height-j-1));
			}
		return Im_out;
	}
}