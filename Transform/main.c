#include "BmpDefine.h"
#include "BmpOperation.h"

int main(void)
{
	//¶ÁÈëÔ­Ê¼Í¼Ïñ
	BmpImage Im = ReadBMP("TEST.bmp");

	//²âÊÔTranslation²Ù×÷
	BmpImage Im_result = Translation(100, 100, Im);
	WriteBMP(Im_result, "Translation1.bmp");
	Im_result = Translation(-100, 100, Im);
	WriteBMP(Im_result, "Translation2.bmp");
	Im_result = Translation(100, -100, Im);
	WriteBMP(Im_result, "Translation3.bmp");
	Im_result = Translation(-100, -100, Im);
	WriteBMP(Im_result, "Translation4.bmp");

	//²âÊÔScale²Ù×÷
	Im_result = Scale(0.3, 0.7 ,Im, Nearestneighbor);
	WriteBMP(Im_result, "Scale_Nearestneighbor_1.bmp");
	Im_result = Scale(1.6, 1.1, Im, Nearestneighbor);
	WriteBMP(Im_result, "Scale_Nearestneighbor_2.bmp");
	Im_result = Scale(0.3, 0.7, Im, Bilinear);
	WriteBMP(Im_result, "Scale_Bilinear_1.bmp");
	Im_result = Scale(1.6, 1.1, Im, Bilinear);
	WriteBMP(Im_result, "Scale_Bilinear_2.bmp");

	//²âÊÔRotation²Ù×÷
	Im_result = Rotation(30, Im, Nearestneighbor);
	WriteBMP(Im_result, "Rotation_Nearestneighbor_1.bmp");
	Im_result = Rotation(120, Im, Nearestneighbor);
	WriteBMP(Im_result, "Rotation_Nearestneighbor_2.bmp");
	Im_result = Rotation(210, Im, Nearestneighbor);
	WriteBMP(Im_result, "Rotation_Nearestneighbor_3.bmp");
	Im_result = Rotation(300, Im, Nearestneighbor);
	WriteBMP(Im_result, "Rotation_Nearestneighbor_4.bmp");
	Im_result = Rotation(30, Im, Bilinear);
	WriteBMP(Im_result, "Rotation_Bilinear_1.bmp");
	Im_result = Rotation(120, Im, Bilinear);
	WriteBMP(Im_result, "Rotation_Bilinear_2.bmp");
	Im_result = Rotation(210, Im, Bilinear);
	WriteBMP(Im_result, "Rotation_Bilinear_3.bmp");
	Im_result = Rotation(300, Im, Bilinear);
	WriteBMP(Im_result, "Rotation_Bilinear_4.bmp");

	//²âÊÔShear²Ù×÷
	Im_result = Shear('x', 0.5, Im, Nearestneighbor);
	WriteBMP(Im_result, "Shear_Nearestneighbor_1.bmp");
	Im_result = Shear('y', 0.5, Im, Nearestneighbor);
	WriteBMP(Im_result, "Shear_Nearestneighbor_2.bmp");
	Im_result = Shear('x', -0.5, Im, Nearestneighbor);
	WriteBMP(Im_result, "Shear_Nearestneighbor_3.bmp");
	Im_result = Shear('y', -0.5, Im, Nearestneighbor);
	WriteBMP(Im_result, "Shear_Nearestneighbor_4.bmp");
	Im_result = Shear('x', 0.5, Im, Bilinear);
	WriteBMP(Im_result, "Shear_Bilinear_1.bmp");
	Im_result = Shear('y', 0.5, Im, Bilinear);
	WriteBMP(Im_result, "Shear_Bilinear_2.bmp");
	Im_result = Shear('x', -0.5, Im, Bilinear);
	WriteBMP(Im_result, "Shear_Bilinear_3.bmp");
	Im_result = Shear('y', -0.5, Im, Bilinear);
	WriteBMP(Im_result, "Shear_Bilinear_4.bmp");

	//²âÊÔMirror²Ù×÷
	Im_result = Mirror('x', Im);
	WriteBMP(Im_result, "Shear_Mirror_1.bmp");
	Im_result = Mirror('y', Im);
	WriteBMP(Im_result, "Shear_Mirror_2.bmp");
}