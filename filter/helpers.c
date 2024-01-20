#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; height > i; i++)
    {
        for (int j = 0; width > j; j++)
        {
            float g = (float)(image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue)/3;
            image[i][j].rgbtRed = round(g);
            image[i][j].rgbtGreen = round(g);
            image[i][j].rgbtBlue  = round(g);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; height > i; i++)
    {
        for (int j = 0; width/2 > j; j++)
        {
            //save the value of left to tem
            int temR = image[i][j].rgbtRed;
            int temG = image[i][j].rgbtGreen;
            int temB = image[i][j].rgbtBlue;

            //Set the value of the left to the right
            image[i][j].rgbtRed = image[i][width-1-j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width-1-j].rgbtGreen;
            image[i][j].rgbtBlue  = image[i][width-1-j].rgbtBlue;

            //Set the value of original right to the left
            image[i][width-1-j].rgbtRed = temR;
            image[i][width-1-j].rgbtGreen = temG;
            image[i][width-1-j].rgbtBlue = temB;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int temR[height][width];
    int temG[height][width];
    int temB[height][width];

    // dupulicate RGB pixels to tem
    for (int i = 0; height > i; i++)
    {
        for (int j= 0; width > j; j++)
        {
            temR[i][j] = image[i][j].rgbtRed;
            temG[i][j] = image[i][j].rgbtGreen;
            temB[i][j] = image[i][j].rgbtBlue;
        }
    }

    for (int a = 0; height > a; a++)
    {
        for (int b = 0; width > b; b++)
        {
            //upper-left corner
            if (a == 0 && b == 0)
            {
                image[a][b].rgbtRed = round((float)(temR[a][b]+temR[a][b+1]+temR[a+1][b]+temR[a+1][b+1])/4);
                image[a][b].rgbtGreen = round((float)(temG[a][b]+temG[a][b+1]+temG[a+1][b]+temG[a+1][b+1])/4);
                image[a][b].rgbtBlue = round((float)(temB[a][b]+temB[a][b+1]+temB[a+1][b]+temB[a+1][b+1])/4);
            }

            //upper-right corner
            else if (a == 0 && b == width-1)
            {
                image[a][b].rgbtRed = round((float)(temR[a][b-1]+temR[a][b]+temR[a+1][b-1]+temR[a+1][b])/4);
                image[a][b].rgbtGreen = round((float)(temG[a][b-1]+temG[a][b]+temG[a+1][b-1]+temG[a+1][b])/4);
                image[a][b].rgbtBlue = round((float)(temB[a][b-1]+temB[a][b]+temB[a+1][b-1]+temB[a+1][b])/4);
            }

            //lower-left corner
            else if (a == height-1 && b == 0)
            {
                image[a][b].rgbtRed = round((float)(temR[a][b]+temR[a][b+1]+temR[a-1][b]+temR[a-1][b+1])/4);
                image[a][b].rgbtGreen = round((float)(temG[a][b]+temG[a][b+1]+temG[a-1][b]+temG[a-1][b+1])/4);
                image[a][b].rgbtBlue = round((float)(temB[a][b]+temB[a][b+1]+temB[a-1][b]+temB[a-1][b+1])/4);
            }

            //lower-right corner
            else if (a == height-1 && b == width-1)
            {
                image[a][b].rgbtRed = round((float)(temR[a][b-1]+temR[a][b]+temR[a-1][b-1]+temR[a-1][b])/4);
                image[a][b].rgbtGreen = round((float)(temG[a][b-1]+temG[a][b]+temG[a-1][b-1]+temG[a-1][b])/4);
                image[a][b].rgbtBlue = round((float)(temB[a][b-1]+temB[a][b]+temB[a-1][b-1]+temB[a-1][b])/4);
            }

            //upper-row
            else if (a == 0)
            {
                image[a][b].rgbtRed = round((float)(temR[a][b-1]+temR[a][b]+temR[a][b+1]+temR[a+1][b-1]+temR[a+1][b]+temR[a+1][b+1])/6);
                image[a][b].rgbtGreen = round((float)(temG[a][b-1]+temG[a][b]+temG[a][b+1]+temG[a+1][b-1]+temG[a+1][b]+temG[a+1][b+1])/6);
                image[a][b].rgbtBlue = round((float)(temB[a][b-1]+temB[a][b]+temB[a][b+1]+temB[a+1][b-1]+temB[a+1][b]+temB[a+1][b+1])/6);
            }

            //lower-row
            else if (a == height-1)
            {
                image[a][b].rgbtRed = round((float)(temR[a-1][b-1]+temR[a-1][b]+temR[a-1][b+1]+temR[a][b-1]+temR[a][b]+temR[a][b+1])/6);
                image[a][b].rgbtGreen = round((float)(temG[a-1][b-1]+temG[a-1][b]+temG[a-1][b+1]+temG[a][b-1]+temG[a][b]+temG[a][b+1])/6);
                image[a][b].rgbtBlue = round((float)(temB[a-1][b-1]+temB[a-1][b]+temB[a-1][b+1]+temB[a][b-1]+temB[a][b]+temB[a][b+1])/6);
            }

            //far left-column
            else if (b == 0)
            {
                image[a][b].rgbtRed = round((float)(temR[a-1][b]+temR[a-1][b+1]+temR[a][b]+temR[a][b+1]+temR[a+1][b]+temR[a+1][b+1])/6);
                image[a][b].rgbtGreen = round((float)(temG[a-1][b]+temG[a-1][b+1]+temG[a][b]+temG[a][b+1]+temG[a+1][b]+temG[a+1][b+1])/6);
                image[a][b].rgbtBlue = round((float)(temB[a-1][b]+temB[a-1][b+1]+temB[a][b]+temB[a][b+1]+temB[a+1][b]+temB[a+1][b+1])/6);
            }

            //far right-column
            else if (b == width-1)
            {
                image[a][b].rgbtRed = round((float)(temR[a-1][b-1]+temR[a-1][b]+temR[a][b-1]+temR[a][b]+temR[a+1][b-1]+temR[a+1][b])/6);
                image[a][b].rgbtGreen = round((float)(temG[a-1][b-1]+temG[a-1][b]+temG[a][b-1]+temG[a][b]+temG[a+1][b-1]+temG[a+1][b])/6);
                image[a][b].rgbtBlue = round((float)(temB[a-1][b-1]+temB[a-1][b]+temB[a][b-1]+temB[a][b]+temB[a+1][b-1]+temB[a+1][b])/6);
            }

            else
            {
                image[a][b].rgbtRed = round((float)(temR[a-1][b-1]+temR[a-1][b]+temR[a-1][b+1]+temR[a][b-1]+temR[a][b]+temR[a][b+1]+temR[a+1][b-1]+temR[a+1][b]+temR[a+1][b+1])/9);
                image[a][b].rgbtGreen = round((float)(temG[a-1][b-1]+temG[a-1][b]+temG[a-1][b+1]+temG[a][b-1]+temG[a][b]+temG[a][b+1]+temG[a+1][b-1]+temG[a+1][b]+temG[a+1][b+1])/9);
                image[a][b].rgbtBlue = round((float)(temB[a-1][b-1]+temB[a-1][b]+temB[a-1][b+1]+temB[a][b-1]+temB[a][b]+temB[a][b+1]+temB[a+1][b-1]+temB[a+1][b]+temB[a+1][b+1])/9);
            }
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int temRx[height][width];
    int temGx[height][width];
    int temBx[height][width];
    int temRy[height][width];
    int temGy[height][width];
    int temBy[height][width];
    int Gx[3][3];
    int Gy[3][3];

    Gx[0][0] = -1;
    Gx[0][1] = 0;
    Gx[0][2] = 1;
    Gx[1][0] = -2;
    Gx[1][1] = 0;
    Gx[1][2] = 2;
    Gx[2][0] = -1;
    Gx[2][1] = 0;
    Gx[2][2] = 1;

    Gy[0][0] = -1;
    Gy[0][1] = -2;
    Gy[0][2] = -1;
    Gy[1][0] = 0;
    Gy[1][1] = 0;
    Gy[1][2] = 0;
    Gy[2][0] = 1;
    Gy[2][1] = 2;
    Gy[2][2] = 1;


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temRx[i][j] = 0;
            temGx[i][j] = 0;
            temBx[i][j] = 0;
            temRy[i][j] = 0;
            temGy[i][j] = 0;
            temBy[i][j] = 0;

            for (int a = 0; a < 3; a++)
            {
                if (i == 0 && a == 0)
                {
                    continue;
                }

                if (i == height - 1 && a == 2)
                {
                    continue;
                }

                for (int b = 0; b < 3; b++)
                {
                    if (j == 0 && b == 0)
                    {
                        continue;
                    }

                    if (j == width - 1 && b == 2)
                    {
                        continue;
                    }

                    temRx[i][j] = temRx[i][j] + Gx[a][b] * image[i - 1 + a][j - 1 + b].rgbtRed;
                    temGx[i][j] = temGx[i][j] + Gx[a][b] * image[i - 1 + a][j - 1 + b].rgbtGreen;
                    temBx[i][j] = temBx[i][j] + Gx[a][b] * image[i - 1 + a][j - 1 + b].rgbtBlue;

                    temRy[i][j] = temRy[i][j] + Gy[a][b] * image[i - 1 + a][j - 1 + b].rgbtRed;
                    temGy[i][j] = temGy[i][j] + Gy[a][b] * image[i - 1 + a][j - 1 + b].rgbtGreen;
                    temBy[i][j] = temBy[i][j] + Gy[a][b] * image[i - 1 + a][j - 1 + b].rgbtBlue;
                }
            }
        }
    }

    for (int c = 0; c < height; c++)
    {
        for (int d = 0; d < width; d++)
        {
            if (round(sqrt(pow(temRx[c][d], 2) + pow(temRy[c][d], 2))) > 255)
            {
                image[c][d].rgbtRed = 255;
            }
            else
            {
                image[c][d].rgbtRed = round((float) sqrt(pow(temRx[c][d], 2) + pow(temRy[c][d], 2)));
            }

            if (round(sqrt(pow(temGx[c][d], 2) + pow(temGy[c][d], 2))) > 255)
            {
                image[c][d].rgbtGreen = 255;
            }
            else
            {
                image[c][d].rgbtGreen = round((float) sqrt(pow(temGx[c][d], 2) + pow(temGy[c][d], 2)));
            }

            if (round(sqrt(pow(temBx[c][d], 2) + pow(temBy[c][d], 2))) > 255)
            {
                image[c][d].rgbtBlue = 255;
            }
            else
            {
                image[c][d].rgbtBlue = round((float) sqrt(pow(temBx[c][d], 2) + pow(temBy[c][d], 2)));
            }
        }
    }

    return;
}
