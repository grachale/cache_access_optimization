# Convolution mask for image sharpening

### Task

Implementation of convolution mask for image sharpening with optimisation of the memory accesses in whatever way is needed – focus on efficient use of cache when using this particular mask:

 0	-1	 0
 
-1   5	-1

 0	-1	 0

 How convolution works, see here: http://setosa.io/ev/image-kernels/
 
If a value lies outside the limits of the interval <0, 255>, use the appropriate limit. The pixels on the image’s edges shall be only copied from the original image (the situation where the convolution core overruns the original image).

Furthermore, compute and write into a file the histogram of the focused image in shades of grey. For the conversion from RGB to grayscale, use the luminance model:
Y = round(0.2126*R + 0.7152*G + 0.0722*B)

The histogram shall be computed for left-closed, right-open intervals (bins) < i*L/N; (i+1)*L/N ) except the last bin, which is also right-closed; for i=0…​N-1, where L=255 and N=5 is the number of bins. In other words, divide the interval <0; 255> into the following bins: 0 to 50,	51 to 101,	102 to 152,	153 to 203,	204 to 255.
				
### Input data

The input image will be in the binary encoded portable pixmap format (PPM). An image in this format always begins with P6, followed by the image width and height. Next, the constant 255 (maximum intensity of a given pixel component), followed by the main image data - individual RGB components for each pixel. Every pixel component (R, G, B) occupies one byte.

The input image file name will be passed on the command line – see the parameters of the main function: main(int argc, char * *argv).

### Output of the program

Output of program will be the focused image in the file named output.ppm, and the histogram (luminance counts separated by spaces) in the file output.txt.

