#ifndef __HORIVERT_H__
#define __HORIVERT_H__

#include "cv.h"
#include "highgui.h"

void thresh_horiz_project(IplImage* src, IplImage* proj_image);
void thresh_vertical_project(IplImage* src, IplImage* proj_image);

#endif