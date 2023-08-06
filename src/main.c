#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <time.h>


#define OUTPUT_FILE_IMG "output.ppm"
#define OUTPUT_FILE_TXT "output.txt"

typedef struct {
    unsigned char red, green, blue;
} Pixel;


typedef struct {
    int x, y;
    Pixel * data;
} Image;


int main(int argc, char * *argv)
{


    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start);


    // Original file
    FILE * fileImage;
    // Original image
    Image * img = (Image *)malloc (sizeof(Image));

    // open file
    fileImage = fopen ( argv[1], "rb");

    // read image format
    char imageFormat[16];
    fgets (imageFormat, sizeof (imageFormat), fileImage);

    // read image size info
    fscanf (fileImage, "%d %d", &img -> x, &img -> y);

    // read maxIntensity const
    int maxIntensity;
    fscanf( fileImage, "%d", &maxIntensity );

    // going to pixel info
    while (fgetc (fileImage) != '\n');

    img -> data = (Pixel *)malloc (img->x * img->y * sizeof(Pixel));
    fread(img->data, 3 * img->x, img->y, fileImage);


    //// output file preparing
    FILE *output;
    output = fopen ( OUTPUT_FILE_IMG, "wb");

    //write the header file
    //image format
    fprintf(output, "P6\n");

    //image size
    fprintf(output, "%d\n%d\n",img->x,img->y);

    // maxIntencity
    fprintf(output, "255\n");


    Image * outputIMG;
    outputIMG = (Image *)malloc (sizeof(Image));
    outputIMG ->data = (Pixel *)malloc (img->x * img->y * sizeof(Pixel));
    outputIMG -> x = img -> x;
    outputIMG -> y = img -> y;
    //// output file ending



    int upIndex = -img ->x;
    int downIndex = img-> x;


    int i = 0;
    // first line
    for (; i < img -> x; ++i)
    {
        outputIMG -> data [i] . red   = img -> data [i] . red;
        outputIMG -> data [i] . green = img -> data [i] . green;
        outputIMG -> data [i] . blue  = img -> data [i] . blue;
    }

    // middle lines
    for (; i < (img -> x * img -> y) - img -> x; ++i )
    {
        // first column
        outputIMG -> data [i] . red      = img -> data [i] . red;
        outputIMG -> data [i] . green    = img -> data [i] . green;
        outputIMG -> data [i] . blue     = img -> data [i] . blue;
        int dupI = i;
        i++;
        // middle columns
        for (; i < dupI + (img -> x - 1 ); ++i)
        {
            // red
            int newRed = img -> data [i]              . red * 5 +
                                   img -> data [i - 1] . red * (-1) +
                                   img -> data [i + 1]  . red * (-1) +
                                   img -> data [i + upIndex]    . red * (-1) +
                                   img -> data [i + downIndex ] . red * (-1);
            if ( newRed > 255 )
                outputIMG -> data [i] . red = 255;
            else if ( newRed < 0 )
                outputIMG -> data [i] . red = 0;
            else
                outputIMG -> data [i] . red = newRed;
            // green
            int newGreen = img -> data [i]             . green * 5 +
                                     img -> data [i + 1] . green * (-1) +
                                     img -> data [i - 1]  . green * (-1) +
                                     img -> data [i + upIndex]    . green * (-1) +
                                     img -> data [i + downIndex ] . green * (-1);
            if ( newGreen > 255 )
                outputIMG -> data [i] . green = 255;
            else if ( newGreen < 0 )
                outputIMG -> data [i] . green = 0;
            else
                outputIMG -> data [i] . green = newGreen;
            // blue
            int newBlue = img -> data [i]             . blue * 5 +
                                   img -> data [i + 1] . blue * (-1) +
                                   img -> data [i - 1]  . blue * (-1) +
                                   img -> data [i + upIndex]    . blue * (-1) +
                                   img -> data [i + downIndex ] . blue * (-1);
            if ( newBlue > 255 )
                outputIMG -> data [i] . blue = 255;
            else if ( newBlue < 0 )
                outputIMG -> data [i] . blue = 0;
            else
                outputIMG -> data [i] . blue = newBlue;
        }
        // last column
        outputIMG -> data [i] . red = img -> data [i] . red;
        outputIMG -> data [i] . green = img -> data [i] . green;
        outputIMG -> data [i] . blue = img -> data [i] . blue;
    }

    // last line
    for (; i < img -> x * img -> y; ++i)
    {
        outputIMG -> data [i] . red   = img -> data [i] . red;
        outputIMG -> data [i] . green = img -> data [i] . green;
        outputIMG -> data [i] . blue  = img -> data [i] . blue;
    }


    fwrite(outputIMG->data, 3 * img->x, img->y, output);



    // histogram
    // Y = round(0.2126*R + 0.7152*G + 0.0722*B)
    // Subinterval:	od 0 do 50	od 51 do 101	od 102 do 152	od 153 do 203	od 204 do 255
    int to50 = 0, to101 = 0, to152 = 0, to203 = 0, to255 = 0;
    int j = 0;
    for (; j < outputIMG -> x * outputIMG -> y; ++j )
    {
        int Y = round( outputIMG -> data [j] . red * 0.2126 + outputIMG -> data [j] . green * 0.7152 + outputIMG -> data [j] . blue * 0.0722);
        if ( Y >= 0 && Y <= 50 )
            ++to50;
        else if ( Y >= 51 && Y <= 101 )
            ++to101;
        else if ( Y >= 102 && Y <= 152 )
            ++to152;
        else if ( Y >= 153 && Y <= 203 )
            ++to203;
        else if ( Y >= 204 && Y <= 255 )
            ++to255;
    }

    FILE *outputTXT;
    outputTXT = fopen ( OUTPUT_FILE_TXT, "wb");
    fprintf(outputTXT, "%d %d %d %d %d", to50, to101, to152, to203, to255);


    fclose ( fileImage );
    fclose (outputTXT);
    fclose ( output );

    free (outputIMG->data);
    free ( outputIMG);

    free(img->data);
    free (img);

    clock_gettime( CLOCK_REALTIME, &stop);
    double accum = ( stop.tv_sec - start.tv_sec )*1000.0 + ( stop.tv_nsec - start.tv_nsec )/ 1000000.0;
    printf( "Time: %.6lf ms\n", accum );

    return 0;
}
