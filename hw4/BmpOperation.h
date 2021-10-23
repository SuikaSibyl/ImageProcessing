//¶ÁÐ´BMP
BmpImage ReadBMP(char *s);
void WriteBMP(BmpImage Im, char* s);
BmpImage BmpCpy(BmpImage Im);


void FullColor2Grayscale(BmpImage Im);
BmpImage Translation(int x, int y, BmpImage Im_in);
BmpImage Scale(double c, double d, BmpImage Im, enum Interpolation Ip);
BmpImage Rotation(double angle, BmpImage Im, enum Interpolation Ip);
BmpImage Shear(char axis, double d, BmpImage Im, enum Interpolation Ip);
BmpImage Mirror(char axis, BmpImage Im);