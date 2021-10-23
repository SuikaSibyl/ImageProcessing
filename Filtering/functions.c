#include "BmpDefine.h"
#include<math.h>
#include<memory.h>
#include<string.h>

#define GETVALUE(a) ((a)>255?255:((a)<0?0:(a)))
#define Coordinate2Array(x,y) ((x)+(y)*Im->InfoHeader->biWidth)
#define Coordinate2Array_out(x,y) ((x)+(y)*Im_out->InfoHeader->biWidth)
#define Pi 3.1416
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
BmpImage ResizeCanva(int x, int y, BmpImage Im)
{
	//重新建宽为x高为y的画布，除尺寸与像素外其它信息与Im相同，像素全为(0,0,0)
	BmpImage Im_out = (BmpImage)malloc(sizeof(struct tagBMPIMAGE));
	Im_out->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im_out->FileHeader = (PBITMAPFILEHEADER)malloc(sizeof(BITMAPFILEHEADER));
	Im_out->FileHeader->bfOffset = Im->FileHeader->bfOffset;	//更改FileHeader
	Im_out->FileHeader->bfReserved1 = Im->FileHeader->bfReserved1;
	Im_out->FileHeader->bfReserved2 = Im->FileHeader->bfReserved2;
	Im_out->FileHeader->bfSize = Im->FileHeader->bfSize + (x*y - Im->InfoHeader->biWidth*Im->InfoHeader->biHeight)*Im->InfoHeader->biBitCount;
	Im_out->FileHeader->bfType = Im->FileHeader->bfType;	//bfSize会发生改变，其它不会
	Im_out->InfoHeader = (PBITMAPINFOHEADER)malloc(sizeof(BITMAPINFOHEADER));	//更改InfoHeader
	Im_out->InfoHeader->biBitCount = Im->InfoHeader->biBitCount;
	Im_out->InfoHeader->biClrImportant = Im->InfoHeader->biClrImportant;
	Im_out->InfoHeader->biClrUsed = Im->InfoHeader->biClrUsed;
	Im_out->InfoHeader->biCompression = Im->InfoHeader->biCompression;
	Im_out->InfoHeader->biHeight = y;	//更改高度信息
	Im_out->InfoHeader->biPlanes = Im->InfoHeader->biPlanes;
	Im_out->InfoHeader->biSize = Im->InfoHeader->biSize;
	Im_out->InfoHeader->biSizeImage = Im->InfoHeader->biSizeImage;
	Im_out->InfoHeader->biWidth = x;	//更改宽度信息
	Im_out->InfoHeader->biXPelsPerMeter = Im->InfoHeader->biXPelsPerMeter;
	Im_out->InfoHeader->biYPelsPerMeter = Im->InfoHeader->biYPelsPerMeter;
	if (Im->Pallete != NULL)	//如有调色板，拷贝
	{
		int num = (int)pow(2, Im->InfoHeader->biBitCount);
		Im_out->Pallete = (PALLETE)malloc(sizeof(struct tagPALLATE)*num);
		memcpy(Im_out->Pallete, Im->Pallete, sizeof(struct tagPALLATE)*num);
	}
	else Im_out->Pallete = NULL;	
	Im_out->RgbData = (RGBQUAD)malloc(x*y * sizeof(struct tagRGBQUAD));	//RGB信息全为(0,0,0)
	memset(Im_out->RgbData, 0, x*y * sizeof(struct tagRGBQUAD));
	return Im_out;
}

Filter ConstructFilter(enum FilterType ft)
{
	Filter fl = (Filter)malloc(sizeof(struct filter));
	fl->a = 1; fl->b = 1;
	if (ft == SimpleMean)//构建simplemean算子
	{
		fl->ft = SimpleMean;
		for (int i = 0; i < 9; i++) fl->seq[i] = 1;
	}
	else if (ft == WeightedMean)//构建weightedmean算子
	{
		fl->ft = WeightedMean;
		fl->seq[0] = 1; fl->seq[1] = 2; fl->seq[2] = 1; fl->seq[3] = 2; fl->seq[4] = 4; fl->seq[5] = 2; fl->seq[6] = 1; fl->seq[7] = 2; fl->seq[8] = 1;
	}
	else if (ft == Laplacian)//构建拉普拉斯锐化微分算子
	{
		fl->ft = Laplacian;
		//fl->seq[0] = 1; fl->seq[1] = 1; fl->seq[2] = 1; fl->seq[3] = 1; fl->seq[4] = -8; fl->seq[5] = 1; fl->seq[6] = 1; fl->seq[7] = 1; fl->seq[8] = 1;
		fl->seq[0] = 0; fl->seq[1] = 1; fl->seq[2] = 0; fl->seq[3] = 1; fl->seq[4] = -4; fl->seq[5] = 1; fl->seq[6] = 0; fl->seq[7] = 1; fl->seq[8] = 0;
	}

	fl->dx[0] = -1; fl->dx[1] = 0; fl->dx[2] = 1; fl->dx[3] = -1; fl->dx[4] = 0; fl->dx[5] = 1; fl->dx[6] = -1; fl->dx[7] = 0; fl->dx[8] = 1;
	fl->dy[0] = 1; fl->dy[1] = 1; fl->dy[2] = 1; fl->dy[3] = 0; fl->dy[4] = 0; fl->dy[5] = 0; fl->dy[6] = -1; fl->dy[7] = -1; fl->dy[8] = -1;
	return fl;
}
BmpImage Filtering(Filter fl, BmpImage Im)
{
	BmpImage Im_out = BmpCpy(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	//用rgb对图像做拉普拉斯滤波并且叠加到原图像
	if (fl->ft == Laplacian)
	{
		int* lap_r = (int*)malloc(sizeof(int)*width*height);
		int* lap_g = (int*)malloc(sizeof(int)*width*height);
		int* lap_b = (int*)malloc(sizeof(int)*width*height);
		for (int i = 0 + fl->a; i < width - fl->a; i++)	//对图像rgb做拉普拉斯滤波
		{
			for (int j = 0 + fl->b; j < height - fl->b; j++)
			{
				lap_b[Coordinate2Array(i, j)] = lap_r[Coordinate2Array(i, j)] = lap_g[Coordinate2Array(i, j)] = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					lap_b[Coordinate2Array(i, j)] += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbBlue*fl->seq[k];
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					lap_r[Coordinate2Array(i, j)] += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbRed*fl->seq[k];
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					lap_g[Coordinate2Array(i, j)] += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbGreen*fl->seq[k];
			}
		}
		for (int i = 0 + fl->a; i < width - fl->a; i++)	//叠加到原图像
		{
			for (int j = 0 + fl->b; j < height - fl->b; j++)
			{
				Im_out->RgbData[Coordinate2Array(i, j)].rgbBlue = GETVALUE(Im_out->RgbData[Coordinate2Array(i, j)].rgbBlue- GETVALUE(lap_b[Coordinate2Array(i, j)]));
				Im_out->RgbData[Coordinate2Array(i, j)].rgbRed = GETVALUE(Im_out->RgbData[Coordinate2Array(i, j)].rgbRed- GETVALUE(lap_r[Coordinate2Array(i, j)]));
				Im_out->RgbData[Coordinate2Array(i, j)].rgbGreen = GETVALUE(Im_out->RgbData[Coordinate2Array(i, j)].rgbGreen- GETVALUE(lap_g[Coordinate2Array(i, j)]));
			}
		}
	}
	//用simplemean滤波器做中值滤波
	else if (fl->ft == SimpleMean)
	{
		for (int i = 0 + fl->a; i < width - fl->a; i++)
		{
			for (int j = 0 + fl->b; j < height - fl->b; j++)
			{
				int rec = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					rec += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbBlue*fl->seq[k];
				Im_out->RgbData[Coordinate2Array_out(i, j)].rgbBlue = rec / 9;
				rec = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					rec += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbRed*fl->seq[k];
				Im_out->RgbData[Coordinate2Array_out(i, j)].rgbRed = rec / 9;
				rec = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					rec += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbGreen*fl->seq[k];
				Im_out->RgbData[Coordinate2Array_out(i, j)].rgbGreen = rec / 9;
			}
		}
	}
	//用weightedmean滤波器做中值滤波
	else if (fl->ft == WeightedMean)
	{
		for (int i = 0 + fl->a; i < width - fl->a; i++)
		{
			for (int j = 0 + fl->b; j < height - fl->b; j++)
			{
				int rec = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					rec += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbBlue*fl->seq[k];
				Im_out->RgbData[Coordinate2Array_out(i, j)].rgbBlue = rec / 16;
				rec = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					rec += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbRed*fl->seq[k];
				Im_out->RgbData[Coordinate2Array_out(i, j)].rgbRed = rec / 16;
				rec = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					rec += Im->RgbData[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].rgbGreen*fl->seq[k];
				Im_out->RgbData[Coordinate2Array_out(i, j)].rgbGreen = rec / 16;
			}
		}
	}
	return Im_out;
}

BmpImage Filtering_yuv(Filter fl, BmpImage Im)
{
	BmpImage Im_out = BmpCpy(Im);
	int width = Im->InfoHeader->biWidth;
	int height = Im->InfoHeader->biHeight;
	YUVQUAD yuv = RGB2YUV(Im);
	//用yuv手段做拉普拉斯滤波并且叠加
	if (fl->ft == Laplacian)
	{
		int* lap = (int*)malloc(sizeof(int)*width*height);
		for (int i = 0 + fl->a; i < width - fl->a; i++)		//对y分量做滤波处理
		{
			for (int j = 0 + fl->b; j < height - fl->b; j++)
			{
				lap[Coordinate2Array(i, j)] = 0;
				for (int k = 0; k < (2 * fl->a + 1)*(2 * fl->b + 1); k++)
					lap[Coordinate2Array(i, j)] += yuv[Coordinate2Array(i + fl->dx[k], j + fl->dy[k])].yuvY*fl->seq[k];
			}
		}
		for (int i = 0 + fl->a; i < width - fl->a; i++)	//把结果叠加
		{
			for (int j = 0 + fl->b; j < height - fl->b; j++)
			{
				yuv[Coordinate2Array(i, j)].yuvY = GETVALUE(yuv[Coordinate2Array(i, j)].yuvY - GETVALUE(lap[Coordinate2Array(i, j)]));
			}
		}
		YUV2RGB(Im_out,yuv);	//转换为rgb图像
	}
	return Im_out;
}