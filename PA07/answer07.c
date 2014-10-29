#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>
#include "answer07.h"

#define BMP_MAGIC_NUMBER 0x4d42
#define DEFAULT_DPI_X 3780
#define DEFAULT_DPI_Y 3780

#define FALSE 0
#define TRUE 1

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t header_size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t  xresolution;
    int32_t  yresolution;
    uint32_t ncolors;
    uint32_t importantcolors; 
}BMP_Header;
#pragma pack(pop)

static void Image_printHeader(ImageHeader * header);
static int Image_checkValid(ImageHeader * header);

void linearNormalization(int width, int height, uint8_t * intensity)
{
    int min = 255, max = 0;
    int w, h;
    uint8_t * original = intensity;
    for(h = 0; h < height; h++)
    {
	for(w = 0; w < width; w++)
	{
	    if(*intensity > max) max = *intensity;
	    if(*intensity < min) min = *intensity;
	    intensity++;
	}
    }
    int range = max - min;
    for(h = 0; h < height; h++)
    {
	for(w = 0; w < width; w++)
	{
	    *original = (*original - min) * 255 / range;
	    original++;
	}
    }
}

void Image_free(Image * image)
{
    if(image != NULL)
    {
	//free(image->width);
	//free(image->height);
	free(image->comment);
	free(image->data);
	free(image);
    }
}

int Image_save(const char * filename, Image * image)
{
    int err = FALSE;
    FILE * fp = NULL;
    uint8_t * buffer = NULL;
    size_t written = 0;

    fp = fopen(filename, "wb");
    if(fp == NULL)
    {
	fprintf(stderr, "Failed to open '%s' for writting\n", filename);
	return FALSE;
    }

    //size_t bytes_per_row = (((8 * image->width + 31)/32) * 4) - 1;
    size_t bytes_per_row = image->width;

    ImageHeader header;
    header.magic_number = ECE264_IMAGE_MAGIC_NUMBER;
    header.width = image->width;
    header.height = image->height;
    header.comment_len = strlen(image->comment);

    if(!err)
    {
	written = fwrite(&header, sizeof(ImageHeader), 1, fp);
	if(written != 1)
	{
	    fprintf(stderr, "Error: only wrote %zd of %zd of file header to '%s'\n", written, sizeof(ImageHeader), filename);
	    err = TRUE;
	}
    }
 
    if(!err)
    {
	size_t bytes_in_str = sizeof(char) * (header.comment_len + 1);
	written = fwrite(image->comment, sizeof(char), header.comment_len + 1, fp); 
	if(written !=  bytes_in_str)
	{
	    fprintf(stderr, "Failed to write comment data to file\n");
	    err = TRUE;
	}
    }

    if(!err)
    {
	buffer = malloc(bytes_per_row);
	if(buffer == NULL)
	{
	    fprintf(stderr, "Error: failed to allocate write buffer\n");
	    err = TRUE;
	}
	else
	{
	    memset(buffer, 0, bytes_per_row);
	}
    }

    if(!err)
    {
	uint8_t * read_ptr = image->data;
	int row, col;
	for(row = 0; row < header.height && !err; ++row)
	{
	    uint8_t * write_ptr = buffer;
	    for(col = 0; col < header.width; ++col)
	    {
		*write_ptr++ = *read_ptr++;
	    }
	    written = fwrite(buffer, sizeof(uint8_t), bytes_per_row, fp);
	    if(written != bytes_per_row)
	    {
		fprintf(stderr, "Failed to write pixel data to file\n");
		err = TRUE;
	    }
	}
    }

    free(buffer);
    if(fp)
    {
	fclose(fp);
    }

    return !err;
}

Image * Image_load(const char *filename)
{
    FILE * fp = NULL;
    int err = FALSE;
    size_t read;
    ImageHeader header;
    Image * tmp_im = NULL, * im = NULL;
    size_t n_bytes;

    fp = fopen(filename, "rb");
    if(!fp)
    {
	fprintf(stderr, "Failed to open file '%s'\n", filename);
	err = TRUE;
    }

    if(!err)
    {
	read = fread(&header, sizeof(ImageHeader), 1, fp);
	if(read != 1)
 	{
	    fprintf(stderr, "Failed to read header from '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	Image_printHeader(&header);
    }

    if(!err)
    {
	if(!Image_checkValid(&header))
	{
	    fprintf(stderr, "Invalid header in '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	tmp_im = malloc(sizeof(Image));
	if(tmp_im == NULL)
	{
	    fprintf(stderr, "Failed to allocate im structure\n");
	    err = TRUE;
	}
    }

    if(!err)
    {
	tmp_im->width = header.width;
	tmp_im->height = header.height;
	n_bytes = sizeof(char) * header.comment_len + 1;
        tmp_im->comment = malloc(n_bytes);
	if(tmp_im->comment == NULL)
        {
            fprintf(stderr, "Failed to allocate %zd bytes for comment\n", n_bytes);
            err = TRUE;
        }
    }

    if(!err)
    {
        char * comment = malloc(n_bytes);
	if(comment == NULL)
      	{	
	    fprintf(stderr, "Failed to allocate %zd bytes for buffer comment\n", n_bytes);
	    err = TRUE; 
	}
	else
	{
    	    read = fread(comment, n_bytes - 1, 1, fp);
            if(read != 1)
	    {
	     	fprintf(stderr, "Invalid comment size in '%s'\n", filename);
	    	err = TRUE;
		tmp_im->data = NULL;
	    }
	    else if(comment[header.comment_len - 1] == '\0')
	    {
		sprintf(tmp_im->comment, "%s", comment);
	    }
	    else
	    {
	    	fprintf(stderr, "Invalid comment in '%s'\n", filename);
	    	err = TRUE;
		tmp_im->data = NULL;
	    }
	    free(comment);
	}
    }

    if(!err)
    {
	n_bytes = sizeof(uint8_t) * header.width * header.height;
	tmp_im->data = malloc(n_bytes);
	if(tmp_im->data == NULL)
	{
	    fprintf(stderr, "Failed to allocate %zd bytes for image data\n", n_bytes);
	    err = TRUE;
	}
    }

    if(!err)
    {
	size_t bytes_per_row = header.width;
	n_bytes = bytes_per_row * header.height;
	uint8_t * rawimg = malloc(n_bytes);
	if(rawimg == NULL)
	{
	    fprintf(stderr, "Could not allocate %zd bytes of image data\n", n_bytes);
	    err = TRUE;
	}
	else
	{
	    read = fread(rawimg, sizeof(uint8_t), n_bytes, fp);
	    if(n_bytes != read)
	    {
		fprintf(stderr, "Only read %zd of %zd bytes of image data \n", read, n_bytes);
		err = TRUE;
	    }
	    else
	    {
		uint8_t * write_ptr = tmp_im->data;
		uint8_t * read_ptr;
		int row, col;
		for(row = 0; row < header.height; ++row)
		{
		    read_ptr = &rawimg[row * bytes_per_row];
		    for(col = 0; col < header.width; ++col)
		    {
			*write_ptr++ = *read_ptr++;
		    }
		}
	    }
	}
	free(rawimg);
    }
    
    if(!err)
    {
	uint8_t byte;
	read = fread(&byte, sizeof(uint8_t), 1, fp);
	if(read != 0)
	{
	    fprintf(stderr, "Stray bytes at the end of ECE264 file '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	im = tmp_im;
	tmp_im = NULL;
    }

    if(tmp_im != NULL)
    {
	free(tmp_im->comment);
  	free(tmp_im->data);
	free(tmp_im);
    }

    if(fp)
    {
	fclose(fp);
    }

    return im;
}

/*Image * Image_load(const char *filename)
{
    FILE * fp = NULL;
    size_t read;
    ImageHeader header;
    Image * tmp_im = NULL, * im = NULL;
    size_t n_bytes = 0;
    int err = FALSE;

    if(!err)
    {
	fp = fopen(filename, "rb");
	if(!fp)
	{
	    fprintf(stderr, "Failed to open file '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	read = fread(&header, sizeof(ImageHeader), 1, fp);
	if(read != 1)
	{
	    fprintf(stderr, "Failed to read header from '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	Image_printHeader(&header);
    }

    if(!err)
    {
	if(!Image_checkValid(&header))
	{
	    fprintf(stderr, "Invalid header in '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	tmp_im = malloc(sizeof(Image));
	if(tmp_im == NULL)
	{
	    fprintf(stderr, "Failed to allocate im structure\n");
	    err = TRUE;
	}
    }

    if(!err)
    {
	tmp_im->width = header.width;
	tmp_im->height = header.height;

	char * filename_cpy = strdup(filename);
	char * file_basename = basename(filename_cpy);

	const char * prefix = "Original ECE264 Image file: ";
	n_bytes = sizeof(char) * (strlen(prefix) + strlen(file_basename) + 1);
	tmp_im->comment = malloc(n_bytes);
	if(tmp_im->comment == NULL)
	{
	    fprintf(stderr, "Failed to allocate %zd bytes for comment\n", n_bytes);
	    err = TRUE;
	}
	else
	{
	    sprintf(tmp_im->comment, "%s%s", prefix, file_basename);
	}
	free(filename_cpy);

	n_bytes = sizeof(uint8_t) * header.width * header.height;
	tmp_im->data = malloc(n_bytes);
	if(tmp_im->data == NULL)
	{
	    fprintf(stderr, "Failed to allocate %zd bytes for image data\n", n_bytes);
	    err = TRUE;
	}
    }

    if(!err)
    {
	if(fseek(fp, header.magic_number, SEEK_SET) != 0)
	{
	    fprintf(stderr, "Failed to seek %d, the data of the image data\n", header.magic_number);
	    err = TRUE;
	}
    }

    if(!err)
    {
	size_t bytes_per_row = ((24 * header.width + 31) / 32) * 4;
	n_bytes = bytes_per_row * header.height;
	uint8_t * rawimg = malloc(n_bytes);
	if(rawimg == NULL)
	{
	    fprintf(stderr, "Could not allocate %zd bytes of image data\n", n_bytes);
	    err = TRUE;
	}
	else
	{
	    read = fread(rawimg, sizeof(uint8_t), n_bytes, fp);
	    if(n_bytes != read)
	    {
		fprintf(stderr, "Only read %zd of %zd bytes of image data\n", read, n_bytes);
		err = TRUE;
	    }
	    else
	    {
		uint8_t * write_ptr = tmp_im->data;
		uint8_t * read_ptr;
		int intensity;
		int row, col;
		for(row = 0; row < header.height; ++row)
		{
		    read_ptr = &rawimg[row * bytes_per_row];
		    for(col = 0; col < header.width; ++col)
		    {
			intensity  = *read_ptr++;
			intensity += *read_ptr++;
			intensity += *read_ptr++;
			*write_ptr++ = intensity / 3;
		    }
		}
	    }
	}
	free(rawimg);
    }

    if(!err)
    {
	uint8_t byte;
	read = fread(&byte, sizeof(uint8_t), 1, fp);
	if(read != 0)
	{
	    fprintf(stderr, "Stray bytes at the end of ECE264 file '%s'\n", filename);
	    err = TRUE;
	}
    }

    if(!err)
    {
	im = tmp_im;
	tmp_im = NULL;
    }

    if(tmp_im != NULL)
    {
	free(tmp_im->comment);
	free(tmp_im->data);
	free(tmp_im);
    }

    if(fp)
    {
	fclose(fp);
    }

    return im;
}*/

static void Image_printHeader(ImageHeader * header)
{
    printf("Printing Image header information:\n");
    printf("  file type (should be %x) %x\n", ECE264_IMAGE_MAGIC_NUMBER, header->magic_number);
    printf("  file width: %d\n", header->width);
    printf("  file height: %d\n", header->height);
    printf("  file comment length: %d\n", header->comment_len);
}

static int Image_checkValid(ImageHeader * header)
{
    if(header->magic_number != ECE264_IMAGE_MAGIC_NUMBER) return FALSE;

    if(header->width <= 0) return FALSE;

    if(header->height <= 0) return FALSE;

    if(header->comment_len < 1) return FALSE;

    return TRUE;
}
