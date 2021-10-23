//¶ÁÐ´BMP
BmpImage ReadBMP(char *s);
void WriteBMP(BmpImage Im, char* s);
BmpImage BmpCpy(BmpImage Im);


void FullColor2Grayscale(BmpImage Im);
void logarithmic_gray(BmpImage Im);
void logarithmic_rgb(BmpImage Im);
void logarithmic(BmpImage Im);
void Histogram_Equalization(BmpImage Im);
void Histogram_Equalization_Gray(BmpImage Im);
void Histogram_Equalization_Union(BmpImage Im);
void Histogram_Fitting_Gray(BmpImage Im, BmpImage Im_stand);
void Histogram_Fitting(BmpImage Im, BmpImage Im_stand);

//void FullColor2Binary(BmpImage Im);
//void Niblack(BmpImage Im);
//void Erosion(BmpImage Im);
//void Dilation(BmpImage Im);
//void Opening(BmpImage Im);
//void Closing(BmpImage Im);