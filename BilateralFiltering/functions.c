#include "BmpDefine.h"
#include<math.h>
#include<memory.h>
#include<string.h>

#define GETVALUE(a) ((a)>255?255:((a)<0?0:(a)))
#define Coordinate2Array(x,y) ((x)+(y)*Im->InfoHeader->biWidth)	//获取坐标ij在线性数组中的下标
#define Coordinate2Array_out(x,y) ((x)+(y)*Im_out->InfoHeader->biWidth)
#define Pi 3.1415926
#define Gs(x) (exp(-((x)*(x)/(2*Sigmas*Sigmas)))/(Sigmas*sqrt(2*Pi)))//计算S的高斯方程结果
#define Gr(x) (exp(-((x)*(x)/(2*Sigmar*Sigmar)))/(Sigmar*sqrt(2*Pi)))//计算R的高斯方程结果
#define SQUARE(x) ((x)*(x))//计算平方
//////////////////////////////////////////////////
/////////////////////基本操作/////////////////////
//////////////////////////////////////////////////
BmpImage ReadBMP(char *s)
{
	FILE *fp;
	BmpImage Im = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));
	Im->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));
	if ((fp = fopen(s, "rb")) == NULL)		//打开HelloWorld.bmp，失败返回错误信息
	{
		printf("Opening occurs Error!\n");
	}
	fseek(fp, 0, SEEK_SET);									//利用fread读入文件头与信息头
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
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//计算位图的实际宽度并确保它为32的倍数
	BYTE *pColorData = (BYTE *)malloc(height*l_width);
	long nData = height * l_width;
	fread(pColorData, 1, nData, fp);

	Im->RgbData = (RGBQUAD)malloc(width*height * sizeof(struct tagRGBQUAD));
	if (Im->InfoHeader->biBitCount < 24)//有调色板，即位图为非真彩色
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
					k = i * l_width + j / 8;//k:取得该像素颜色数据在实际数据数组中的序号
					mixIndex = pColorData[k];//mixIndex:提取当前像素的颜色的具体值
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
					//将像素数据保存到数组中对应的位置
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
					k = i * l_width + j / 4;//k:取得该像素颜色数据在实际数据数组中的序号
					//j:提取当前像素的颜色的具体值
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

					//将像素数据保存到数组中对应的位置
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
					{//低
						mixIndex = mixIndex << 4;
						mixIndex = mixIndex >> 4;
					}
					else
					{//高
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
	else//位图为24位真彩色
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
		YUV[i].yuvY = (BYTE)(0.114*Im->RgbData[i].rgbBlue + 0.587*Im->RgbData[i].rgbGreen + 0.299*Im->RgbData[i].rgbRed);		//为每像素Y赋值
		YUV[i].yuvU = (BYTE)(0.437*Im->RgbData[i].rgbBlue - 0.289*Im->RgbData[i].rgbGreen - 0.148*Im->RgbData[i].rgbRed);		//为每像素U赋值
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
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//计算位图的实际宽度并确保它为32的倍数
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
					k = i * l_width + j / 8;//k:取得该像素颜色数据在实际数据数组中的序号
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
	else//位图为24位真彩色
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
/////////////////////灰度操作/////////////////////
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
void RgbCopy(RGBQUAD X, RGBQUAD Y, int xvalue,int yvalue)
{
	//拷贝图Y一个像素(yvalue)的RGB信息到图X的xvalue像素
	X[xvalue].rgbBlue = Y[yvalue].rgbBlue;
	X[xvalue].rgbRed = Y[yvalue].rgbRed;
	X[xvalue].rgbGreen = Y[yvalue].rgbGreen;
}
BmpImage BilateralFilting(BmpImage Im,int S,double Sigmas,double Sigmar)
{
	//用于对图像进行双倍滤波
	//Im读入的图片，S滤波器的宽度（应为奇数），Sigmas空间系数，Sigmar密度系数
	BmpImage Im_out = BmpCpy(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int r =(S-1)/2;
	for(int i=0 ; i<width;i++)
		for (int j = 0; j < height; j++)	//对每一个像素进行滤波
		{
			double Wp = 0;
			double sum=0;
			int minm = i - r >= 0 ? i - r: 0;
			int minn = j - r >= 0 ? j - r : 0;
			int maxm = i + r <= width ? i + r: width;			
			int maxn = j + r <= height ? j + r : height;	//计算滤波器范围，处理边界情况
			for(int m=minm;m<maxm;m++)
				for (int n = minn; n < maxn; n++)	//对滤波器范围内的每一个像素进行计算
				{
					double Gsigmas = Gs(sqrt(SQUARE(m - i) + SQUARE(n - j)));
					int temp = abs((int)Im->RgbData[Coordinate2Array(m, n)].rgbBlue - (int)Im->RgbData[Coordinate2Array(i, j)].rgbBlue);
					double Gsigmar = Gr(temp);
					sum += Gsigmas * Gsigmar*Im->RgbData[Coordinate2Array(m, n)].rgbBlue;
					Wp += Gsigmas * Gsigmar;
				}
			Im_out->RgbData[Coordinate2Array(i, j)].rgbBlue = sum / Wp;
			Wp = 0; sum = 0;
			for (int m = minm; m < maxm; m++)
				for (int n = minn; n < maxn; n++)	//同上，对Red进行处理
				{
					double Gsigmas = Gs(sqrt(SQUARE(m - i) + SQUARE(n - j)));
					int temp = abs((int)Im->RgbData[Coordinate2Array(m, n)].rgbRed - (int)Im->RgbData[Coordinate2Array(i, j)].rgbRed);
					double Gsigmar = Gr(temp);
					sum += Gsigmas * Gsigmar*Im->RgbData[Coordinate2Array(m, n)].rgbRed;
					Wp += Gsigmas * Gsigmar;
				}
			Im_out->RgbData[Coordinate2Array(i, j)].rgbRed = sum / Wp;
			Wp = 0; sum = 0;
			for (int m = minm; m < maxm; m++)
				for (int n = minn; n < maxn; n++)	//同上，对Green进行处理
				{
					double Gsigmas = Gs(sqrt(SQUARE(m - i) + SQUARE(n - j)));
					int temp = abs((int)Im->RgbData[Coordinate2Array(m, n)].rgbGreen - (int)Im->RgbData[Coordinate2Array(i, j)].rgbGreen);
					double Gsigmar = Gr(temp);
					sum += Gsigmas * Gsigmar*Im->RgbData[Coordinate2Array(m, n)].rgbGreen;
					Wp += Gsigmas * Gsigmar;
				}
			Im_out->RgbData[Coordinate2Array(i, j)].rgbGreen = sum / Wp;
		}
	return Im_out;
}
void SelfAdaptingParameters(BmpImage Im,double* Sigmas, double* Sigmar)
{
	YUVQUAD yuv = RGB2YUV(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	double diagonal = sqrt(SQUARE(width) + SQUARE(height));
	*Sigmas = 0.1*diagonal;//以对角线长度的倍数作为sigmas
	int dx[9] = { -1,0,1,-1,0,1,-1,0,1 };
	int dy[9] = { 1,1,1,0,0,0,-1,-1,-1 };
	int sx[9] = { 1,0,-1,2,0,-2,1,0,-1 };
	int sy[9] = { 1,2,1,0,0,0,-1,-2,-1 };
	int count = 0; double sum = 0;
	for (int i = 1; i < width-1; i++)
		for (int j = 1; j < height-1; j++)	//对每一个像素进行滤波
		{
			int sxf=0, syf=0;
			count++;
			for (int k = 0; k < 9; k++)
			{
				sxf += sx[k] * yuv[Coordinate2Array(i + dx[k], j + dx[k])].yuvY;//Sobel算子X分量
				syf += sy[k] * yuv[Coordinate2Array(i + dx[k], j + dx[k])].yuvY;//Sobel算子Y分量
			}
			double temp = sqrt(SQUARE(sxf) + SQUARE(syf));
			sum += temp;
		}
	*Sigmar = 0.1*sum/count;//以平均数作为sigmar
}