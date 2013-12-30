/****************************************************************************
    gif.c - read and write gif images using libgif/libungif.
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

#include <gif_lib.h>

/*
  A lot of this is based on the gif2rgb and rgb2gif codes in the libungif 
  distribution. 
*/

int
read_gif(const char *filename, int *width, int *height, unsigned char *rgb)
{
    int interlace_offset[] = { 0, 4, 2, 1 };
    int interlace_jump[] = { 8, 8, 4, 2 };
    GifColorType *colormap;
    GifFileType *infile;
    GifRecordType record_type;
    GifRowType *buffer = NULL;

    int i, j;
    int color_index;
    unsigned char *ptr = NULL;

    infile = DGifOpenFileName(filename);

    if (infile == NULL)
    {
        PrintGifError();
        return(0);
    }

    do
    {
        if (DGifGetRecordType(infile, &record_type) == GIF_ERROR) 
        {
            PrintGifError();
            return(0);
        }

        switch (record_type)
        {
        case IMAGE_DESC_RECORD_TYPE:
            if (DGifGetImageDesc(infile) == GIF_ERROR)
            {
                PrintGifError();
                return(0);
            }

            *width = infile->Image.Width;
            *height = infile->Image.Height;

            buffer = malloc(*height * sizeof(GifRowType *));
            if (buffer == NULL)
            {
                fprintf(stderr, "Can't allocate memory for GIF file.\n");
                return(0);
            }
            
            for (i = 0; i < *height; i++) 
            {
                buffer[i] = malloc(*width * sizeof(GifPixelType));
                if (buffer[i] == NULL)
                {
                    fprintf(stderr, "Can't allocate memory for GIF line.\n");
                    return(0);
                }
            }
            
            if (infile->Image.Interlace)
            {
                for (i = 0; i < 4; i++)
                    for (j = interlace_offset[i]; j < *height; 
                         j += interlace_jump[i])
                        DGifGetLine(infile, buffer[j], *width);
            }
            else
            {
                for (i = 0; i < *height; i++)
                    DGifGetLine(infile, buffer[i], *width);
            }
            break;
        case EXTENSION_RECORD_TYPE:
        {
            /* Skip extension blocks */
            int ext_code;
            GifByteType *ext;
            if (DGifGetExtension(infile, &ext_code, &ext) == GIF_ERROR) 
            {
                PrintGifError();
                return(0);
            }
            while (ext != NULL) 
            {
                if (DGifGetExtensionNext(infile, &ext) == GIF_ERROR) 
                {
                    PrintGifError();
                    return(0);
                }
            }
        }
        break;
        case TERMINATE_RECORD_TYPE:
            break;
        default:
            fprintf(stderr, "unknown record type in GIF file\n");
            break;
        }
    } while (record_type != TERMINATE_RECORD_TYPE);

    colormap = (infile->Image.ColorMap ? infile->Image.ColorMap->Colors
                : infile->SColorMap->Colors);

    ptr = rgb;

    for (j = 0; j < *height; j++)
    {
        for (i = 0; i < *width; i++)
        {
            color_index = (int) buffer[j][i];
            *ptr++ = (unsigned char) colormap[color_index].Red;
            *ptr++ = (unsigned char) colormap[color_index].Green;
            *ptr++ = (unsigned char) colormap[color_index].Blue;
        }
        free(buffer[j]);
    }
    
    free(buffer);

    DGifCloseFile(infile);
    return(1);
}

int 
write_gif(const char *filename, int width, int height, char *rgb)
{
    int i;
    int colormap_size = 256;
    GifByteType *red, *green, *blue, *buffer, *ptr;
    GifFileType *outfile;
    ColorMapObject *colormap;

    red = malloc(width * height * sizeof(GifByteType));
    green = malloc(width * height * sizeof(GifByteType));
    blue = malloc(width * height * sizeof(GifByteType));
    buffer = malloc(width * height * sizeof(GifByteType));

    if (red == NULL || green == NULL || blue == NULL || buffer == NULL)
    {
        fprintf(stderr, "Can't allocate memory for GIF file.\n");
        return(0);
    }

    colormap = MakeMapObject(colormap_size, NULL);

    for (i = 0; i < width * height; i++)
    {
        red[i]   = (GifByteType) rgb[3*i  ];
        green[i] = (GifByteType) rgb[3*i+1];
        blue[i]  = (GifByteType) rgb[3*i+2];
    }
  
    if (QuantizeBuffer(width, height, &colormap_size, red, green, blue,   
                       buffer, colormap->Colors) == GIF_ERROR)
    {
        PrintGifError();
        return(0);
    }

    free(red);
    free(green);
    free(blue);

    outfile = EGifOpenFileName((char *) filename, FALSE);
    if (outfile == NULL)
    {
        PrintGifError();
        return(0);
    }

    if (EGifPutScreenDesc(outfile, width, height, colormap_size, 0, colormap)
        == GIF_ERROR)
    {
        PrintGifError();
        return(0);
    }

    if (EGifPutImageDesc(outfile, 0, 0, width, height, FALSE, NULL)
        == GIF_ERROR)
    {
        PrintGifError();
        return(0);
    }

    ptr = buffer;
    for (i = 0; i < height; i++)
    {
        if (EGifPutLine(outfile, ptr, width) == GIF_ERROR)
        {
            PrintGifError();
            return(0);
        }
        ptr += width;
    }

    EGifSpew(outfile);

    if (EGifCloseFile(outfile) == GIF_ERROR) 
        PrintGifError();

    free(buffer);

    return(1);
}
