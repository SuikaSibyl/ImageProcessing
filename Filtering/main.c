#include "BmpDefine.h"
#include "BmpOperation.h"

int main(void)
{
	BmpImage Im = ReadBMP("Lenna.bmp");
	Filter fl = ConstructFilter(Laplacian);
	//��rgb�ֶ�����
	BmpImage Im_l = Filtering(fl, Im);
	WriteBMP(Im_l, "Laplacian_rgb.bmp");
	//��yuv�ֶ�����
	Im_l = Filtering_yuv(fl, Im);
	WriteBMP(Im_l, "Laplacian_yuv.bmp");
	//��simplemean����ֵ�˲�
	fl = ConstructFilter(SimpleMean);
	Im_l = Filtering(fl, Im);
	WriteBMP(Im_l, "SimpleMean.bmp");
	//��weightedmean����ֵ�˲�
	fl = ConstructFilter(WeightedMean);
	Im_l = Filtering(fl, Im);
	WriteBMP(Im_l, "WeightedMean.bmp");
}