#include "StdAfx.h"
#include "HeImg.h"

heimg::heimg()
{
}
void heimg::mSetImg(IplImage * pImg)
{
	m_img=cvCloneImage(pImg);
}

heimg::~heimg(void)
{
}