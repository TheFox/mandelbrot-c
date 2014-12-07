
#ifndef _MANDELBROT_FUNCTIONS_H
#define _MANDELBROT_FUNCTIONS_H 1

#include "config.h"

#include <cmath>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef void* voidp;
typedef void** voidpp;
typedef unsigned char* ucharp;
typedef double mbnum;

float sqr(const float);
void print_copyright();
void print_config(int, int, int, int, int, int, int, int, float, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum, mbnum);
int point_iteration(const mbnum, const mbnum, const int);

#endif
