#include "BmpDefine.h"
#include<math.h>
#include<memory.h>
#include<string.h>
#define GETVALUE(a) ((a)>255?255:((a)<0?0:(a)))
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
void logarithmic_gray(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int maxgray = 0;
	for (int i = 0; i < width*height; i++)
		if (Im->RgbData[i].rgbBlue > maxgray) maxgray = Im->RgbData[i].rgbBlue;		//寻找最大的灰度值
	double* Ld = (double*)malloc(num * sizeof(double));
	double denominator = log(maxgray + 1);
	double Ldmin = 1;
	for (int i = 0; i < width*height; i++)	//分别得出每一个[0,1]值
	{
		Ld[i] = log(Im->RgbData[i].rgbBlue + 1) / denominator;
		if (Ld[i] < Ldmin)
			Ldmin = Ld[i];
	}
	for (int i = 0; i < width*height; i++)	//均匀拉伸到0-255上
	{
		Im->RgbData[i].rgbBlue = Im->RgbData[i].rgbGreen = Im->RgbData[i].rgbRed = (Ld[i]-Ldmin) * 255/ (1 - Ldmin);
	}
	free(Ld);
}
void logarithmic_rgb(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int maxR = 0, maxG = 0, maxB = 0;
	for (int i = 0; i < width*height; i++)
	{
		if (Im->RgbData[i].rgbBlue > maxB) maxB = Im->RgbData[i].rgbBlue;
		if (Im->RgbData[i].rgbGreen > maxG) maxG = Im->RgbData[i].rgbGreen;
		if (Im->RgbData[i].rgbRed > maxR) maxR = Im->RgbData[i].rgbRed;
	}
	double* LdR = (double*)malloc(num * sizeof(double));
	double* LdG = (double*)malloc(num * sizeof(double));
	double* LdB = (double*)malloc(num * sizeof(double));
	double denominatorR = log(maxR + 1);
	double denominatorG = log(maxG + 1);
	double denominatorB = log(maxB + 1);
	double LdminR = 1, LdminG = 1, LdminB = 1;
	for (int i = 0; i < width*height; i++)
	{
		LdR[i] = log(Im->RgbData[i].rgbRed + 1) / denominatorR;
		if (LdR[i] < LdminR)
			LdminR = LdR[i];
		LdG[i] = log(Im->RgbData[i].rgbGreen + 1) / denominatorG;
		if (LdG[i] < LdminG)
			LdminG = LdG[i];
		LdB[i] = log(Im->RgbData[i].rgbBlue +1) / denominatorB;
		if (LdB[i] < LdminB)
			LdminB = LdB[i];
	}
	for (int i = 0; i < width*height; i++)
	{
		Im->RgbData[i].rgbBlue  = (LdB[i] - LdminB) * 255 / (1 - LdminB);
		Im->RgbData[i].rgbGreen = (LdG[i] - LdminG) * 255 / (1 - LdminG);
		Im->RgbData[i].rgbRed = (LdR[i] - LdminR) * 255 / (1 - LdminR);
	}
	free(LdR);
	free(LdG);
	free(LdB);
}
void logarithmic(BmpImage Im)
{
	YUVQUAD yuv=RGB2YUV(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int maxgray = 0;
	for (int i = 0; i < width*height; i++)//寻找最大的灰度值
		if (yuv[i].yuvY > maxgray) maxgray = yuv[i].yuvY;
	double* Ld = (double*)malloc(num * sizeof(double));
	double denominator = log(maxgray + 1);
	double Ldmin = 1;
	for (int i = 0; i < width*height; i++)//分别得出每一个[0,1]值
	{
		Ld[i] = log(yuv[i].yuvY + 1) / denominator;
		if (Ld[i] < Ldmin)
			Ldmin = Ld[i];
	}
	for (int i = 0; i < width*height; i++)	//均匀拉伸到0-255上
	{
		yuv[i].yuvY = (Ld[i] - Ldmin) * 255 / (1 - Ldmin);
	}
	free(Ld);
	YUV2RGB(Im, yuv);//重新构建RGB图像
	free(yuv);
}
void Histogram_Equalization_Gray(BmpImage Im)
{
	double check = 0;
	YUVQUAD yuv = RGB2YUV(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int* Histogram = (int*)malloc(sizeof(int) * 256);	//记录直方图像素个数
	double* S = (double*)malloc(sizeof(double) * 256);		//记录累计直方图概率
	memset(Histogram, 0, sizeof(int) * 256);
	memset(S, 0, sizeof(double) * 256);
	for (int i = 0; i < num; i++)
		Histogram[yuv[i].yuvY]++;
	S[0] = Histogram[0];
	for (int i = 1; i < 256; i++)	//积分
		S[i] = S[i - 1] + Histogram[i];
	for (int i = 0; i < 256; i++)		//归一化后展开到255
		S[i] = S[i] * 255 / num;
	for (int i = 0; i < num; i++)
	{
		yuv[i].yuvY = (BYTE)S[yuv[i].yuvY];
		Im->RgbData[i].rgbBlue = Im->RgbData[i].rgbGreen = Im->RgbData[i].rgbRed = yuv[i].yuvY;
	}
	free(S);
	free(Histogram);
	free(yuv);
}
void Histogram_Equalization_Union(BmpImage Im)
{
	double check = 0;
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int* Histogram = (int*)malloc(sizeof(int)*256);	//记录直方图像素个数
	double* S = (double*)malloc(sizeof(double) * 256);	//记录累计直方图概率
	memset(Histogram, 0, sizeof(int) * 256);
	memset(S, 0, sizeof(double)*256);
	for (int i = 0; i < num; i++)	//将RGB三色共同记录在同一张直方图
	{
		Histogram[Im->RgbData[i].rgbBlue]++;
		Histogram[Im->RgbData[i].rgbGreen]++;
		Histogram[Im->RgbData[i].rgbRed]++;
	}
	S[0] = Histogram[0];
	for (int i = 1; i < 256; i++)
		S[i] = S[i - 1] + Histogram[i];
	for (int i = 0; i < 256; i++)
	{
		S[i] =( S[i] * 255 / (3 * num));
		printf("%f	", S[i]);
	}
	for (int i = 0; i < num; i++)		//将RGB三色进行同一种映射
	{
		Im->RgbData[i].rgbBlue = (BYTE)GETVALUE(S[Im->RgbData[i].rgbBlue]);
		Im->RgbData[i].rgbGreen = (BYTE)GETVALUE(S[Im->RgbData[i].rgbGreen]);
		Im->RgbData[i].rgbRed = (BYTE)GETVALUE(S[Im->RgbData[i].rgbRed]);
	}
	free(S);
	free(Histogram);
}
void Histogram_Equalization(BmpImage Im)
{
	double check = 0;
	YUVQUAD yuv = RGB2YUV(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int* Histogram = (int*)malloc(sizeof(int) * 256);//记录直方图像素个数
	double* S = (double*)malloc(sizeof(double) * 256);	//记录累计直方图概率
	memset(Histogram, 0, sizeof(int) * 256);
	memset(S, 0, sizeof(double) * 256);
	for (int i = 0; i < num; i++)
		Histogram[yuv[i].yuvY]++;
	for (int i = 0; i < 256; i++)
	{
		printf("%d	", Histogram[i]);
	}
	S[0] = Histogram[0];
	for (int i = 1; i < 256; i++)
		S[i] = S[i - 1] + Histogram[i];
	for (int i = 1; i < 256; i++)
		S[i] = S[i] * 255 / num;
	for (int i = 0; i < num; i++)		//仅仅改变Y的值
		yuv[i].yuvY = (BYTE)S[yuv[i].yuvY];
	YUV2RGB(Im, yuv);	//重新转换为RGB模式
	free(S);
	free(Histogram);
	free(yuv);
}
void Histogram_Fitting_Gray(BmpImage Im,BmpImage Im_stand)
{
	YUVQUAD yuv = RGB2YUV(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int* Histogram = (int*)malloc(sizeof(int) * 256);	//记录待处理图片直方图
	double* S = (double*)malloc(sizeof(double) * 256);	//记录待处理图片累积概率
	memset(Histogram, 0, sizeof(int) * 256);
	memset(S, 0, sizeof(double) * 256);
	for (int i = 0; i < num; i++)	//记录待处理直方图
		Histogram[yuv[i].yuvY]++;
	S[0] = Histogram[0];
	for (int i = 1; i < 256; i++)
		S[i] = S[i - 1] + Histogram[i];
	for (int i = 0; i < 256; i++)	//记录待处理累积概率
		S[i] = S[i] / num;

	YUVQUAD yuvs = RGB2YUV(Im_stand);
	int widths = Im_stand->InfoHeader->biWidth;
	int heights = Im_stand->InfoHeader->biHeight;
	int nums = widths * heights;
	int* Histograms = (int*)malloc(sizeof(int) * 256);	//记录目标图片直方图
	double* Ss = (double*)malloc(sizeof(double) * 256);	//记录目标图片累积概率
	memset(Histograms, 0, sizeof(int) * 256);
	memset(Ss, 0, sizeof(double) * 256);
	for (int i = 0; i < nums; i++)	//记录目标图片直方图
		Histograms[yuvs[i].yuvY]++;
	Ss[0] = Histograms[0];
	for (int i = 1; i < 256; i++)
		Ss[i] = Ss[i - 1] + Histograms[i];
	for (int i = 0; i < 256; i++)	//记录目标图片直方图
		Ss[i] = Ss[i] / nums;

	int* map = (int*)malloc(sizeof(int) * 256);	//寻找最小绝对值差距，构建函数映射
	for (int i = 0; i < 256; i++)
	{
		double absmin = 1; int scalemin = 0;
		for (int j = 0; j < 256; j++)
		{
			if (fabs(S[i] - Ss[j])< absmin)
			{
				absmin = fabs(S[i] - Ss[j]);
				scalemin = j;
			}
		}
		map[i] = scalemin;
	}
	for (int i = 0; i < num; i++)	//更改灰度数值
	{
		yuv[i].yuvY = (BYTE)GETVALUE(map[yuv[i].yuvY]);
		Im->RgbData[i].rgbBlue = Im->RgbData[i].rgbGreen = Im->RgbData[i].rgbRed = yuv[i].yuvY;
	}

	free(S);
	free(Ss);
	free(Histogram);
	free(Histograms);
	free(yuv);
	free(yuvs);
	free(map);
}
void Histogram_Fitting(BmpImage Im, BmpImage Im_stand)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	int* Histogram_R = (int*)malloc(sizeof(int) * 256);
	int* Histogram_G = (int*)malloc(sizeof(int) * 256);
	int* Histogram_B = (int*)malloc(sizeof(int) * 256);
	double* S_R = (double*)malloc(sizeof(double) * 256);
	double* S_G = (double*)malloc(sizeof(double) * 256);
	double* S_B = (double*)malloc(sizeof(double) * 256);
	memset(Histogram_R, 0, sizeof(int) * 256);
	memset(Histogram_G, 0, sizeof(int) * 256);
	memset(Histogram_B, 0, sizeof(int) * 256);
	memset(S_R, 0, sizeof(double) * 256);
	memset(S_G, 0, sizeof(double) * 256);
	memset(S_B, 0, sizeof(double) * 256);
	for (int i = 0; i < num; i++)
	{
		Histogram_R[Im->RgbData[i].rgbRed]++;
		Histogram_G[Im->RgbData[i].rgbGreen]++;
		Histogram_B[Im->RgbData[i].rgbBlue]++;
	}
	S_R[0] = Histogram_R[0];
	S_G[0] = Histogram_G[0];
	S_B[0] = Histogram_B[0];
	for (int i = 1; i < 256; i++)
	{
		S_R[i] = S_R[i - 1] + Histogram_R[i];
		S_G[i] = S_G[i - 1] + Histogram_G[i];
		S_B[i] = S_B[i - 1] + Histogram_B[i];
	}
	for (int i = 0; i < 256; i++)
	{
		S_R[i] = S_R[i] / num;
		S_G[i] = S_G[i] / num;
		S_B[i] = S_B[i] / num;
	}
	int widths = Im_stand->InfoHeader->biWidth;
	int heights = Im_stand->InfoHeader->biHeight;
	int nums = widths * heights;
	int* Histograms_R = (int*)malloc(sizeof(int) * 256);
	int* Histograms_G = (int*)malloc(sizeof(int) * 256);
	int* Histograms_B = (int*)malloc(sizeof(int) * 256);
	double* Ss_R = (double*)malloc(sizeof(double) * 256);
	double* Ss_G = (double*)malloc(sizeof(double) * 256);
	double* Ss_B = (double*)malloc(sizeof(double) * 256);
	memset(Histograms_R, 0, sizeof(int) * 256);
	memset(Histograms_G, 0, sizeof(int) * 256);
	memset(Histograms_B, 0, sizeof(int) * 256);
	memset(Ss_R, 0, sizeof(double) * 256);
	memset(Ss_G, 0, sizeof(double) * 256);
	memset(Ss_B, 0, sizeof(double) * 256);
	for (int i = 0; i < nums; i++)
	{
		Histograms_R[Im_stand->RgbData[i].rgbRed]++;
		Histograms_G[Im_stand->RgbData[i].rgbGreen]++;
		Histograms_B[Im_stand->RgbData[i].rgbBlue]++;
	}
	Ss_R[0] = Histograms_R[0];
	Ss_G[0] = Histograms_G[0];
	Ss_B[0] = Histograms_B[0];
	for (int i = 1; i < 256; i++)
	{
		Ss_R[i] = Ss_R[i - 1] + Histograms_R[i];
		Ss_G[i] = Ss_G[i - 1] + Histograms_G[i];
		Ss_B[i] = Ss_B[i - 1] + Histograms_B[i];
	}
	for (int i = 0; i < 256; i++)
	{
		Ss_R[i] = Ss_R[i] / nums;
		Ss_G[i] = Ss_G[i] / nums;
		Ss_B[i] = Ss_B[i] / nums;
	}
	int* map_R = (int*)malloc(sizeof(int) * 256);
	int* map_G = (int*)malloc(sizeof(int) * 256);
	int* map_B = (int*)malloc(sizeof(int) * 256);
	for (int i = 0; i < 256; i++)
	{
		double absmin = 1; int scalemin = 0;
		for (int j = 0; j < 256; j++)
		{
			if (fabs(S_R[i] - Ss_R[j]) < absmin)
			{
				absmin = fabs(S_R[i] - Ss_R[j]);
				scalemin = j;
			}
		}
		map_R[i] = scalemin;
	}
	for (int i = 0; i < 256; i++)
	{
		double absmin = 1; int scalemin = 0;
		for (int j = 0; j < 256; j++)
		{
			if (fabs(S_G[i] - Ss_G[j]) < absmin)
			{
				absmin = fabs(S_G[i] - Ss_G[j]);
				scalemin = j;
			}
		}
		map_G[i] = scalemin;
	}
	for (int i = 0; i < 256; i++)
	{
		double absmin = 1; int scalemin = 0;
		for (int j = 0; j < 256; j++)
		{
			if (fabs(S_B[i] - Ss_B[j]) < absmin)
			{
				absmin = fabs(S_B[i] - Ss_B[j]);
				scalemin = j;
			}
		}
		map_B[i] = scalemin;
	}


	for (int i = 0; i < num; i++)
	{
		Im->RgbData[i].rgbRed = (BYTE)GETVALUE(map_R[Im->RgbData[i].rgbRed]);
		Im->RgbData[i].rgbGreen = (BYTE)GETVALUE(map_G[Im->RgbData[i].rgbGreen]);
		Im->RgbData[i].rgbBlue = (BYTE)GETVALUE(map_B[Im->RgbData[i].rgbBlue]);
	}

	free(S_R); free(S_G); free(S_B);
	free(Ss_R); free(Ss_G); free(Ss_B);
	free(Histogram_R); free(Histogram_G); free(Histogram_B);
	free(Histograms_R); free(Histograms_G); free(Histograms_B);
	free(map_R); free(map_G); free(map_B);
}
//////////////////////////////////////////////////
/////////////////////二值操作/////////////////////
//////////////////////////////////////////////////
void FullColor2Binary(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	Im->InfoHeader->biBitCount = 1;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);	//计算位图的实际宽度并确保它为32的倍数
	int num = width * height;
	YUVQUAD yuv = RGB2YUV(Im);
	int threshold = OTSU(Im);	//用大津算法获取阈值
	for (int i = 0; i < width*height; i++)	//判断阈值关系
	{
		if (yuv[i].yuvY > threshold)
		{
			Im->RgbData[i].rgbBlue = 255;
			Im->RgbData[i].rgbGreen = 255;
			Im->RgbData[i].rgbRed = 255;
		}
		else
		{
			Im->RgbData[i].rgbBlue = 0;
			Im->RgbData[i].rgbGreen = 0;
			Im->RgbData[i].rgbRed = 0;
		}
	}
	//为转换为二值图像改变相应头信息
	Im->FileHeader->bfOffset = 62;
	Im->FileHeader->bfSize = 62 + l_width * height;
	Im->InfoHeader->biBitCount = 1;
	Im->InfoHeader->biSizeImage = l_width * height;
	Im->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*2);
	Im->Pallete[0].rgbBlue = Im->Pallete[0].rgbRed = Im->Pallete[0].rgbGreen = 0;
	Im->Pallete[1].rgbBlue = Im->Pallete[1].rgbRed = Im->Pallete[1].rgbGreen = 255;
	Im->Pallete[0].rgbReversed = Im->Pallete[1].rgbReversed = 0;
}
int Niblack_Threshold(BmpImage Im,int i,int j, YUVQUAD yuv)
{
	int r = 7;
	int k = -0.1;
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int threshold;

	int m = 0, s = 0, n = 0;
	for (int p = i - r; p <= i + r; p++)	//计算均值
	{
		for (int q = j - r; q <= j + r; q++)
		{
			int temp;
			if (p < 0 || q < 0||p>=width||q>=height)	temp = 0;
			else
			{
				temp = yuv[p + q * width].yuvY;
				n++;
			}
			m += temp;
		}
	}
	m = m / n;
	for (int p = i - r; p <= i + r; p++)	//计算标准差
	{
		for (int q = j - r; q <= j + r; q++)
		{
			int temp;
			if (p < 0 || q < 0 || p >= width || q >= height)	temp = 0;
			else
			{
				temp = yuv[p + q * width].yuvY;
				s += (temp - m)*(temp - m);
			}
		}
	}
	s = (int)sqrt(s / n);
	threshold = m + 1.0*k*s;
	return threshold;
}
void Niblack(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	Im->InfoHeader->biBitCount = 1;
	int l_width = WIDTHBYTES(width* Im->InfoHeader->biBitCount);//计算位图的实际宽度并确保它为32的倍数
	int num = width * height;
	YUVQUAD yuv = RGB2YUV(Im);
	int* result = (int*)malloc(sizeof(int)*width*height);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int threshold = Niblack_Threshold(Im,i,j,yuv);
			if (yuv[i + j * width].yuvY < threshold)
				result[i + j * width] = 1;
			else
				result[i + j * width] = 0;
			//printf("%d %d ||", threshold, result[i + j * width]);
		}
	}
	for (int i = 0; i < width*height; i++)
	{
		if (result[i]==0)
		{
			Im->RgbData[i].rgbBlue = 255;
			Im->RgbData[i].rgbGreen = 255;
			Im->RgbData[i].rgbRed = 255;
		}
		else
		{
			Im->RgbData[i].rgbBlue = 0;
			Im->RgbData[i].rgbGreen = 0;
			Im->RgbData[i].rgbRed = 0;
		}
	}
	Im->FileHeader->bfOffset = 62;
	Im->FileHeader->bfSize = 62 + l_width * height;
	Im->InfoHeader->biBitCount = 1;
	Im->InfoHeader->biSizeImage = l_width * height;
	Im->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE) * 2);
	Im->Pallete[0].rgbBlue = Im->Pallete[0].rgbRed = Im->Pallete[0].rgbGreen = 0;
	Im->Pallete[1].rgbBlue = Im->Pallete[1].rgbRed = Im->Pallete[1].rgbGreen = 255;
	Im->Pallete[0].rgbReversed = Im->Pallete[1].rgbReversed = 0;
}
int OTSU(BmpImage Im)
{
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;
	YUVQUAD yuv = RGB2YUV(Im);
	//直方图Histogram用于统计每个灰度像素数目
	int Histogram[256] = { 0 };
	for (int i = 0; i < num; i++)
	{
		Histogram[(int)yuv[i].yuvY * 256 / 256]++;
	}
	float pt[256], w[256], u[256], o[256], Ut;
	//pt统计灰度级为i的像素个数占总像素的比例
	pt[0] = (float)Histogram[0] / num;
	//w为灰度级小等于i的图像像素数所占比例的累加
	w[0] = pt[0];
	//u为灰度级小等于i的图像像素i*pt[i]的累加
	u[0] = w[0];
	for (int i = 1; i < 256; i++)
	{
		pt[i] = (float)Histogram[i] / num;
		w[i] = w[i - 1] + pt[i];
		u[i] = u[i - 1] + i * pt[i];
	};
	Ut = u[255];//整幅图像平均灰度
	for (int i = 0; i < 256; i++)
	{
		o[i] = w[i] * (1 - w[i])*(u[i] / w[i] - (u[255] - u[i]) / (1 - w[i]))*(u[i] / w[i] - (u[255] - u[i]) / (1 - w[i]));//方差
	};
	int maxi = 0;
	float maxo = 0;
	for (int i = 0; i < 256; i++)
	{
		if (o[i] != 0x7FC0000)
			if (o[i] > maxo) { maxo = o[i]; maxi = i; }//遍历0~255各灰阶，计算并寻找类间方差极大值，当找到时，i为对应类间方差的灰度值

	}
	return maxi * 256 / 256;
}
//////////////////////////////////////////////////
//////////////////////形态学//////////////////////
//////////////////////////////////////////////////
StrElem Cross()
{
	StrElem Cross;
	Cross.x = (int*)malloc(5 * sizeof(int));
	Cross.y = (int*)malloc(5 * sizeof(int));
	Cross.x[0] = -1; Cross.x[1] = 0; Cross.x[2] = 0; Cross.x[3] = 0; Cross.x[4] = 1;
	Cross.y[0] = 0; Cross.y[1] = -1; Cross.y[2] = 0; Cross.y[3] = 1; Cross.y[4] = 0;
	Cross.len = 5;
	Cross.r = 1;
	return Cross;
}
StrElem Square(int r)
{
	StrElem Square;
	Square.len = (2 * r + 1)*(2 * r + 1);
	Square.x = (int*)malloc(Square.len * sizeof(int));
	Square.y = (int*)malloc(Square.len * sizeof(int));
	int idx = 0;
	for (int i = -r; i <= r; i++) {
		for (int j = -r; j <= r; j++) {
			Square.x[idx] = i;
			Square.y[idx] = j;
			idx++;
		}
	}
	Square.r = r;
	return Square;
}
void Erosion(BmpImage Im)
{
	StrElem strelem=Cross();

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;

	int* out = (int*)malloc(sizeof(int)*num);
	memset(out, 0, sizeof(int)*num);
	
	for(int i= strelem.r;i<width- strelem.r;i++)	//遍历非边缘的每一个点
		for (int j = strelem.r; j < height - strelem.r; j++)
		{
			int flag = 1;
			for (int k = 0; k < strelem.len; k++)
			{
				if (Im->RgbData[(j+ strelem.y[k])*width + i+ strelem.x[k]].rgbBlue != 0)
				{
					flag = 0; break;	//判断B是否包含于A
				}
			}
			if (flag == 1)
				out[j*width + i] = 1;	//获取二值化结果
		}
	for (int i = 0; i < num; i++)
	{
		if (out[i] == 1)
		{
			Im->RgbData[i].rgbBlue = 0;
			Im->RgbData[i].rgbRed = 0;
			Im->RgbData[i].rgbGreen = 0;
		}
		else
		{
			Im->RgbData[i].rgbBlue = 255;
			Im->RgbData[i].rgbRed =255;
			Im->RgbData[i].rgbGreen = 255;
		}
	}
}
void Dilation(BmpImage Im)
{
	StrElem strelem = Cross();

	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	int num = width * height;

	int width_l = width + 2 * strelem.r;
	int height_l = height + 2 * strelem.r;
	int r = strelem.r;

	int* in = (int*)malloc(sizeof(int)*width_l*height_l);
	int* out = (int*)malloc(sizeof(int)*width_l*height_l);
	memset(in, 0, sizeof(int)*width_l*height_l);
	memset(out, 0, sizeof(int)*width_l*height_l);

	for(int i= 0;i< width;i++)//遍历每一个点，进行扩充操作
		for (int j =0; j < height; j++)
		{
			if (Im->RgbData[i + j * width].rgbBlue == 0)
				in[(i + strelem.r) + (j+r) * width_l] = 1;
		}


	for(int i=r;i<width_l-r;i++)
		for (int j = r; j < height_l-r; j++)
		{
			int flag = 0;
			for (int k = 0; k < strelem.len; k++)
			{
				if (in[(i + strelem.x[k]) + width_l * (j + strelem.y[k])] == 1)	//判断B与A的交是否
				{
					flag = 1;
					break;
				}
			}
			if(flag==1)
			out[i + width_l * j] = 1;
		}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			if (out[(i + r) + (j + r)*width_l] == 1)
			{
				Im->RgbData[i + j * width].rgbBlue = 0;
				Im->RgbData[i + j * width].rgbRed = 0;
				Im->RgbData[i + j * width].rgbGreen = 0;
			}
			else
			{
				Im->RgbData[i + j * width].rgbBlue = 255;
				Im->RgbData[i + j * width].rgbRed = 255;
				Im->RgbData[i + j * width].rgbGreen = 255;
			}
		}

	free(in);
	free(out);
}
void Opening(BmpImage Im)
{
	Erosion(Im);
	Dilation(Im);
}
void Closing(BmpImage Im)
{
	Dilation(Im);
	Erosion(Im);
}