/****************************************************************************
    tiff.c - read and write tiff images using libtiff routines.
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
#include <stdlib.h>

#include <tiffio.h>

int
read_tiff(const char *filename, int *width, int *height, unsigned char *rgb)
{
    unsigned char *ptr = NULL;
    int i, j, istart;

    TIFF* tif = TIFFOpen(filename, "r");

    uint32 w, h;
    uint32* raster;
  
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);

    *width = (int) w;
    *height = (int) h;

    raster = _TIFFmalloc(w * h * sizeof(uint32));
    if (raster != NULL) 
    {
        ptr = rgb;

        if (TIFFReadRGBAImage(tif, w, h, raster, 0)) 
        {
            for (j = h - 1; j >= 0; j--)
            {
                istart = j * w;
                for (i = 0; i < w; i++)
                {
                    *ptr++ = (unsigned char) TIFFGetR(raster[istart + i]);
                    *ptr++ = (unsigned char) TIFFGetG(raster[istart + i]);
                    *ptr++ = (unsigned char) TIFFGetB(raster[istart + i]);
                }
            }
        }
        else
        {
            _TIFFfree(raster);
            return(0);
        }
        _TIFFfree(raster);
    }
    else 
        return(0);

    TIFFClose(tif);
    return(1);
}

int
write_tiff(const char *filename, int width, int height, unsigned char *rgb)
{
    unsigned char *raster;
    int j;
  
    TIFF* tif = TIFFOpen(filename, "w");

    if (tif == NULL) 
    {
        fprintf(stderr, "Can't create TIFF file\n");
        return(0);
    }

    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32) width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32) height);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, -1));
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    for (j = 0; j < height; j++)
    {
        raster = rgb + (j * width * 3);
        TIFFWriteScanline(tif, raster, j, 0);
    }

    TIFFClose(tif);
    return(0);
}
