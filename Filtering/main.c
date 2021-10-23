#include "BmpDefine.h"
#include "BmpOperation.h"

int main(void)
{
	BmpImage Im = ReadBMP("Lenna.bmp");
	Filter fl = ConstructFilter(Laplacian);
	//用rgb手段做锐化
	BmpImage Im_l = Filtering(fl, Im);
	WriteBMP(Im_l, "Laplacian_rgb.bmp");
	//用yuv手段做锐化
	Im_l = Filtering_yuv(fl, Im);
	WriteBMP(Im_l, "Laplacian_yuv.bmp");
	//用simplemean做均值滤波
	fl = ConstructFilter(SimpleMean);
	Im_l = Filtering(fl, Im);
	WriteBMP(Im_l, "SimpleMean.bmp");
	//用weightedmean做均值滤波
	fl = ConstructFilter(WeightedMean);
	Im_l = Filtering(fl, Im);
	WriteBMP(Im_l, "WeightedMean.bmp");
}