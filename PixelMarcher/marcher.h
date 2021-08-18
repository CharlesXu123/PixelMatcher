/**
 *                    Pixel Marcher
 * 
 * This is the file where all your implementation for the A3 section of 
 * the assignment goes. You will be submitting this file to MarkUs. The
 * documentation for the functions here is a bit long (sorry!) but I hope it's 
 * precise enough to not leave room for ambiguity in any of the edge cases.
 *
 * Driver.c gives you a way to run this code on a single image,
 * and TestMarcher.c runs a few test cases to verify your solution.
 * Look at the relevant files for more information regarding how to
 * use them.
 * 
 * You shouldn't change any of the code here. You will
 * NOT be submitting this file as part of your assignment.
 * You don't need to declare any helper functions here since
 * they are not going to be exposed to any of the other files.
 *
 * CSCB63 Winter 2020 - Assignment 2
 * (c) Mustafa Quraish
 */

#ifndef __MARCHER_H__
#define __MARCHER_H__

#include "imgutils.h"
#include "minheap.h"

// You don't need to understand this syntax, but it essentially 
// defines `WeightFunc` to be a function which expects an Image
// pointer and two pixel coordinates, and returns a double.
typedef double (*WeightFunc)(Image *im, int a, int b);

double findPath(Image *im, WeightFunc weight, int path[]);
double allColourWeight(Image *im, int a, int b);

#endif
