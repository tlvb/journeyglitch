#include "ppmio.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define D 16


void vertigo(bitmap_t *bi, bitmap_t *bo);
void glitch(bitmap_t *bi, bitmap_t *bo);
void sweep(bitmap_t *bi, bitmap_t *bo);

int rcmp(const char *s1, const char *s2) {
	int l1 = strlen(s1);
	int l2 = strlen(s2);
	for (int i=0; ; ++i) {
		if (i == l1 || i == l2) return 0;
		int d = s1[l1-1-i] != s2[l2-1-i];
		if (d != 0) return d;
	}
}

int main(int argc, const char* const* argv) { /*{{{*/
	assert(argc == 4); // type filei fileo

	fprintf(stderr, "type: '%c', file in: '%s', file out: '%s'\n", argv[1][0], argv[2], argv[3]);

	char *command = calloc(strlen(argv[2])+128, sizeof(char));
	if (rcmp(argv[2], "jpg") == 0 || rcmp(argv[2], "jpeg") == 0) {
		fprintf(stderr, "jpeg image assumed from extension\n");
		sprintf(command, "jpegtopnm <%s", argv[2]);
	}
	else if (rcmp(argv[2], "png") == 0) {
		fprintf(stderr, "png image assumed from extension\n");
		sprintf(command, "pngtopnm <%s", argv[2]);
	}
	else {
		fprintf(stderr, "unknown extension\n");
	}

	FILE *pfd = popen(command, "r");
	if (!pfd) {
		perror(NULL);
	}
	assert(pfd != NULL);
	int flag = PPM_OK;
	bitmap_t *bi = ppm_fread(NULL, pfd, &flag);
	pclose(pfd);
	free(command);
	assert(flag == PPM_OK);
	assert(bi != NULL);

	fprintf(stderr, "w: %u, h: %u\n", bi->width, bi->height);

	bitmap_t *bo = bitmap_new(NULL, bi->width, bi->height);

	switch(argv[1][0]) {
	case 's':
		// scholar
		vertigo(bi, bo);
		break;
	case 'c':
		// cyborg
		glitch(bi, bo);
		break;
	case 'd':
		// drifter
		sweep(bi, bo);
		break;
	default:
		fprintf(stderr, "bad parameter '%c'\n", argv[1][0]);
		return 1;
	}

	command = calloc(strlen(argv[3])+128, sizeof(char));
	sprintf(command, "pnmtojpeg -quality 100 > %s", argv[3]);
	pfd = popen(command, "w");
	if (!pfd) {
		perror(NULL);
	}
	assert(pfd);
	ppm_fwrite(pfd, bo);
	pclose(pfd);
	free(command);
	bitmap_free(bi);
	bitmap_free(bo);
	return 0;
} /*}}}*/

void vertigo(bitmap_t *bi, bitmap_t *bo) { /*{{{*/
	float w = (float) bi->width;
	float h = (float) bi->height;
	const float di = sqrtf(w/2*w/2 + h/2*h/2);
	for (unsigned int y=0; y<bi->height; ++y) {
		for (unsigned int x=0; x<bi->width; ++x) {
			float rx = fabs(w/2 - ((float) x));
			float ry = fabs(w/2 - ((float) y));
			float d = sqrtf(rx*rx + ry*ry) / di;

			float ac = (
					((float)ppm_data(bi,x,y,0)) +
					((float)ppm_data(bi,x,y,1)) +
					((float)ppm_data(bi,x,y,2))) / (255.0*3.0);

			float nl = (1-ac*d)*(1-ac*d);

			for (unsigned int i=0; i<3; ++i) {
				ppm_data(bo,x,y,i) = (1-d)*ppm_data(bi,x,y,i) + d*ac*255;
				ppm_data(bo,x,y,i) *= (1.2-d)>1?1:1.2-d;
			}
		}
	}
} /*}}}*/
void glitch(bitmap_t *bi, bitmap_t *bo) { /*{{{*/
	for (unsigned int y=0; y<bi->height; y=y+D) {
		for (unsigned int x=0; x<bi->width; x=x+D) {
			float acmax = -1;
			unsigned int ax = 0;
			unsigned int ay = 0;
			for (unsigned int yy=y; yy<y+D; ++yy) {
				if (yy>=bi->height) continue;
				for (unsigned int xx=x; xx<x+D; ++xx) {
					if (xx>=bi->width) continue;
					float ac = (
							((float)ppm_data(bi,xx,yy,0)) +
							((float)ppm_data(bi,xx,yy,1)) +
							((float)ppm_data(bi,xx,yy,2))) / (255.0*3.0);
					if ((ac > 0.9) && (ac > acmax)) {
						acmax = ac;
						ax = xx;
						ay = yy;
					}
				}
			}
			for (unsigned int yy=y; yy<y+D; ++yy) {
				if (yy>=bi->height) continue;
				for (unsigned int xx=x; xx<x+D; ++xx) {
					if (xx>=bi->width) continue;
					float r = ((float) ppm_data(bi,xx,yy,0))/255;
					float g = ((float) ppm_data(bi,xx,yy,1))/255;
					float b = ((float) ppm_data(bi,xx,yy,2))/255;
					if (acmax > -1) {
						r = 0.1+((float) ppm_data(bi,xx,yy,0))/255;
						g = 0.1+((float) ppm_data(bi,xx,yy,1))/255;
						b = 0.1+((float) ppm_data(bi,xx,yy,2))/255;
					}
					g = g*(14.0/16.0) + 1.0/16.0;
					r = r*(14.0/8.0) - 1.0/8.0;
					r = r<0?0:r; r = r>1?2-r:r;
					g = g<0?0:g; g = g>1?2-g:g;
					b = b<0?0:b; b = b>1?2-b:b;
					if (b < 0.2) {
						b = ((float) ppm_data(bi,x,y,2))/255;
					}
					ppm_data(bo,xx,yy,0) = r*255;
					ppm_data(bo,xx,yy,1) = g*255;
					ppm_data(bo,xx,yy,2) = b*255;
				}
			}
		}
	}
} /*}}}*/
void sweep(bitmap_t *bi, bitmap_t *bo) {
	for (unsigned int y=0; y<bi->height; ++y) {
		for (unsigned int x=0; x<bi->width; ++x) {
			if (x > 0) {
				float rd = ((float) ppm_data(bo,x-1,y,0))/255;
				float gd = ((float) ppm_data(bo,x-1,y,1))/255;
				float bd = ((float) ppm_data(bo,x-1,y,2))/255;
				float r = ((float) ppm_data(bi,x,y,0))/255;
				float g = ((float) ppm_data(bi,x,y,1))/255;
				float b = ((float) ppm_data(bi,x,y,2))/255;
				float acd = (rd+gd+bd)/3.0;
				float ac = (r+g+b)/3.0;
				if (fabs(acd-ac) < 0.2) {
					ppm_data(bo,x,y,0) = (r+rd*7)*31.125;
					ppm_data(bo,x,y,1) = (g+gd*7)*31.125;
					ppm_data(bo,x,y,2) = (b+bd*7)*31.125;
				}
				else {
					for (unsigned int i=0; i<3; ++i) {
						ppm_data(bo,x,y,i) = ppm_data(bi,x,y,i);
					}
				}
			}
			else {
				for (unsigned int i=0; i<3; ++i) {
					ppm_data(bo,x,y,i) = ppm_data(bi,x,y,i);
				}
			}
		}
	}
}
