#pragma once
#include "highgui.h"
#include "cv.h"

class heimg :public CvvImage
{
public:
	heimg();
	void mSetImg(IplImage *pImg);
public:
	~heimg();
};