/****************************************************************************
    pnm.c - read and write pnm images using libpnm/netpbm routines.
    Distributed with Xplanet.
    Copyright (C) 2002 Hari Nair <hari@alumni.caltech.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
****************************************************************************/

#include <stdio.h>
#include <string.h>

#include <pnm.h>

int
read_pnm(const char *filename, int *width, int *height, char *rgb)
{
    FILE *infile = fopen(filename, "rb");

    xelval maxvalP;
    int formatP;

    xel **imagearray;

    int argcP = 1;
    char **argv;

    int i, j, ipos = 0;

    argv = malloc(sizeof(char **));
    if (argv == NULL)
    {
        fprintf(stderr, "Can't allocate memory in read_pnm().\n");
        return(0);
    }

    argv[0] = malloc(10);
    if (argv[0] == NULL)
    {
        fprintf(stderr, "Can't allocate memory in read_pnm().\n");
        return(0);
    }

    strcpy(argv[0], "loadimage");

    pnm_init(&argcP, argv);

    imagearray = pnm_readpnm(infile, width, height, &maxvalP, &formatP);

    switch (PNM_FORMAT_TYPE(formatP))
    {
    case PPM_TYPE:
        for (j = 0; j < *height; j++)
            for (i = 0; i < *width; i++)
            {
                rgb[ipos++] = (unsigned char) (PPM_GETR(imagearray[j][i]) 
                                                  & 0xff); 
                rgb[ipos++] = (unsigned char) (PPM_GETG(imagearray[j][i]) 
                                                  & 0xff); 
                rgb[ipos++] = (unsigned char) (PPM_GETB(imagearray[j][i]) 
                                                  & 0xff); 
            }
        break;
    case PGM_TYPE:
    case PBM_TYPE:
        for (j = 0; j < *height; j++)
            for (i = 0; i < *width; i++)
            {
                memset(rgb + ipos, 
                       (unsigned char) (PNM_GET1(imagearray[j][i]) & 0xff), 
                       3);
                ipos +=3;
            }
        break;
    default:
        fprintf(stderr, "Unknown magic number for pnm file\n");
        return(0);
    }

    pnm_freearray(imagearray, *height);
    return(1);
}

int
write_pnm(FILE *outfile, int width, int height, unsigned char *rgb,
          int maxv, int format, int forceplain)
{
    xelval maxval = maxv;
    xel **imagearray = pnm_allocarray(width, height);
    int i, j, ipos;
    xelval avg;

    switch (format)
    {
    case PBM_TYPE:
        for (j = 0; j < height; j++)
        { 
            ipos = j * width;
            for (i = 0; i < width; i++)
            {
                avg = (rgb[3 * (ipos + i)] + rgb[3 * (ipos + i) + 1] 
                       + rgb[3 * (ipos + i) + 2]) / 3;
                PNM_ASSIGN1(imagearray[j][i], (avg > 127 ? 1 : 0));
            }
        }
        break;
    case PGM_TYPE:
        for (j = 0; j < height; j++)
        { 
            ipos = j * width;
            for (i = 0; i < width; i++)
            {
                avg = (rgb[3 * (ipos + i)] + rgb[3 * (ipos + i) + 1] 
                       + rgb[3 * (ipos + i) + 2]) / 3;
                PNM_ASSIGN1(imagearray[j][i], avg);
            }
        }
        break;
    case PPM_TYPE:
        for (j = 0; j < height; j++)
        { 
            ipos = j * width;
            for (i = 0; i < width; i++)
            {
                PPM_ASSIGN(imagearray[j][i],
                           (xelval) rgb[3 * (ipos + i)],
                           (xelval) rgb[3 * (ipos + i) + 1],
                           (xelval) rgb[3 * (ipos + i) + 2]);
            }
        }
        break;
    default:
        fprintf(stderr, "Unknown pnm format\n");
    }

    pnm_writepnm(outfile, imagearray, width, height, maxval, format, 
                 forceplain);

    pnm_freearray(imagearray, height);
    return(1);
}
