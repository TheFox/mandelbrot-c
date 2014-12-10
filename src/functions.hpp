
#ifndef _MANDELBROT_FUNCTIONS_H
#define _MANDELBROT_FUNCTIONS_H 1

#include "config.h"

#ifdef __cplusplus
#include <cmath>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef long double mbnum;
typedef mbnum* mbnum_p;

#define MBNUM_FORMAT "Lf"

void print_copyright();
void print_config(int, int, int, int, int, int, int, int, float, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum);
int point_iteration(const mbnum, const mbnum, const int);
void data_file_name(char *, int, int, int, int, mbnum, mbnum, mbnum, mbnum, int);

#endif
