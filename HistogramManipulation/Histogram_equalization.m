I = imread('try0.bmp');
imshow(I)
I2 = histeq(I);
imshow(I2)