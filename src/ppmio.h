#ifndef __PNMIO_H__
#define __PNMIO_H__ 1

#include <stdint.h>
#include <stdio.h>
#include "graphics.h"

#define PPM_OK 0
#define PPM_FILEERROR 1
#define PPM_BADFILE 2
#define PPM_BADALLOC 3

#define ppm_data(_i,_x,_y,_j) (_i->data[_i->width*_y+_x].x[_j])


void ignorewhitespace(FILE *f);
void ignorecomment(FILE *f);
bitmap_t *ppm_read(bitmap_t *recycle, const char *fn, int *status);
bitmap_t *ppm_fread(bitmap_t *recycle, FILE *f, int *status);
int ppm_write(const char *fn, bitmap_t *img);
int ppm_fwrite(FILE *f, bitmap_t *img);

#endif
