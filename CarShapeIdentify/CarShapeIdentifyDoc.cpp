// CarShapeIdentifyDoc.cpp : CCarShapeIdentifyDoc 类的实现
//

#include "stdafx.h"
#include "CarShapeIdentify.h"

#include "CarShapeIdentifyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarShapeIdentifyDoc

IMPLEMENT_DYNCREATE(CCarShapeIdentifyDoc, CDocument)

BEGIN_MESSAGE_MAP(CCarShapeIdentifyDoc, CDocument)
	ON_COMMAND(ID_LoadBk, &CCarShapeIdentifyDoc::OnLoadbk)
	ON_COMMAND(ID_LoadFr, &CCarShapeIdentifyDoc::OnLoadfr)
	ON_COMMAND(ID_ImgSub, &CCarShapeIdentifyDoc::OnImgsub)
	ON_COMMAND(ID_Binary, &CCarShapeIdentifyDoc::OnBinary)
	ON_COMMAND(ID_Open, &CCarShapeIdentifyDoc::OnOpen)
	ON_COMMAND(ID_RemoveNoise, &CCarShapeIdentifyDoc::OnRemovenoise)
	ON_COMMAND(ID_ImgFill, &CCarShapeIdentifyDoc::OnImgfill)
	ON_COMMAND(ID_Contour, &CCarShapeIdentifyDoc::OnContour)
	ON_COMMAND(ID_Identify, &CCarShapeIdentifyDoc::OnIdentify)
END_MESSAGE_MAP()


// CCarShapeIdentifyDoc 构造/析构

CCarShapeIdentifyDoc::CCarShapeIdentifyDoc()
{

}

CCarShapeIdentifyDoc::~CCarShapeIdentifyDoc()
{
}

BOOL CCarShapeIdentifyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}




// CCarShapeIdentifyDoc 序列化

void CCarShapeIdentifyDoc::Serialize(CArchive& ar)
{
}


// CCarShapeIdentifyDoc 诊断

#ifdef _DEBUG
void CCarShapeIdentifyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCarShapeIdentifyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCarShapeIdentifyDoc 命令

void CCarShapeIdentifyDoc::OnLoadbk()
{
	// TODO: 在此添加命令处理程序代码
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|任何文件|*.*||";
	CFileDialog dlg1(TRUE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	 
	//打开文件对话框
	if(dlg1.DoModal()==IDOK)
	{
		if(0==(bk_img=cvLoadImage(dlg1.GetPathName(),1)))
		{  
			AfxMessageBox("无法打开文件！");
			return;
		}
		else
		{
			m_Cimage.mSetImg(bk_img);
			UpdateAllViews(NULL);
			return ;
		}
	}
}

void CCarShapeIdentifyDoc::OnLoadfr()
{
	// TODO: 在此添加命令处理程序代码
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|任何文件|*.*||";
	CFileDialog dlg1(TRUE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	 
	//打开文件对话框
	if(dlg1.DoModal()==IDOK)
	{
		if(0==(FrImg=cvLoadImage(dlg1.GetPathName(),1)))
		{  
			AfxMessageBox("无法打开文件！");
			return;
		}
		else
		{
			m_Cimage.mSetImg(FrImg);
			UpdateAllViews(NULL);
			return ;
		}
	}
}

void CCarShapeIdentifyDoc::OnImgsub()
{
	// TODO: 在此添加命令处理程序代码
	//将图像首先转化为灰度图像
	BkGray=cvCreateImage(cvGetSize(bk_img),8,1);
	cvCvtColor(bk_img,BkGray,CV_BGR2GRAY);
	FrGray=cvCreateImage(cvGetSize(bk_img),8,1);
	cvCvtColor(FrImg,FrGray,CV_BGR2GRAY);

	//对图像进行高斯平滑操作
	cvSmooth(BkGray,BkGray,CV_GAUSSIAN,5);
	cvSmooth(FrGray,FrGray,CV_GAUSSIAN,5);

	//对图像进行做差运算
	SubImg=cvCreateImage(cvGetSize(bk_img),8,1);
	cvAbsDiff(FrGray,BkGray,SubImg);

    //显示图像
	m_Cimage.mSetImg(SubImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnBinary()
{
	// TODO: 在此添加命令处理程序代码
	//图像二值化
	BinaryImg=cvCreateImage(cvGetSize(bk_img),8,1);
	cvThreshold(SubImg,BinaryImg,0,255,CV_THRESH_OTSU);	

	//显示图像
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnOpen()
{
	// TODO: 在此添加命令处理程序代码
	//定义腐蚀和膨胀用的核,
	IplConvKernel * kernel_5_5;
	kernel_5_5=cvCreateStructuringElementEx(5,5,2,2,CV_SHAPE_ELLIPSE,0);
	IplConvKernel * kernel_7_7;
	kernel_7_7=cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE,0);

	//对前景图像进行开闭合运算以除去杂点，分割出运动物体
	//对图像进行腐蚀运算
	cvErode(BinaryImg,BinaryImg,kernel_5_5,1);

	//对图像进行膨胀运算
	cvDilate(BinaryImg,BinaryImg,kernel_7_7,1);

	//显示图像
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnRemovenoise()
{
	// TODO: 在此添加命令处理程序代码
	//去除较小的离散连通区域

	//首先将图像复制一份
	IplImage * BinaryImg2;
	BinaryImg2=cvCloneImage(BinaryImg);

	// 定义内存空间
	CvMemStorage * storage=cvCreateMemStorage(0);

	//定义统计联通区域轮廓的序列
	CvSeq * area_contour=NULL;
	int Number_contours=0;   //轮廓个数
	double Area=0;           //轮廓面积
	CvScalar s_fill_new;
	s_fill_new.val[0]=0;
	CvScalar loDiff;
	loDiff.val[0]=10;
	CvScalar upDiff;
	upDiff.val[0]=10;
	//查找图像中的最外部轮廓
	Number_contours=cvFindContours(BinaryImg,storage,&area_contour,sizeof(CvContour),CV_RETR_EXTERNAL);	
	//统计轮廓面积，对于面积小于阈值的进行删除
	for(CvSeq * c=area_contour;c!=NULL;c=c->h_next)
	{
		//获得轮廓面积
		Area=fabs(cvContourPerimeter(c));
		if(Area<30)
		{
			for(int i=0;i<1;i++)
			{
				//取轮廓上的一个点作为图像填充种子点
				CvPoint * seedpoint=CV_GET_SEQ_ELEM(CvPoint,c,i);
				//用漫水填充算法将小的轮廓填充为背景色
				cvFloodFill(BinaryImg2,*seedpoint,s_fill_new,loDiff,upDiff,NULL,4,0);
			}
		}
	}
	BinaryImg=cvCloneImage(BinaryImg2);
	//图像显示
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnImgfill()
{
	// TODO: 在此添加命令处理程序代码
	//对汽车进行内部填充
	CvScalar s;
	CvScalar s_new;
	s_new.val[0]=255;
	//记录填充行的最左位置
	int left_X=0;
	//记录填充行的最右位置
    int right_X=0;
	//记录填充列的最上位置
	int top_Y=0;
	//记录填充列的最下位置
	int bottom_Y=0;

	//纵向填充
	for(int wt=0;wt<BinaryImg->width;wt++)
	{
		for(int ht=0;ht<BinaryImg->height;ht++)
		{	
			s=cvGet2D(BinaryImg,ht,wt);
			if(255==s.val[0])
			{
				top_Y=ht;//找到最上边的白色点
				for(int m=BinaryImg->height-1;m>=0;m--)
				{
					s=cvGet2D(BinaryImg,m,wt);
					if(255==s.val[0])
					{
						bottom_Y=m;
						break; //跳出寻找最下边点的循环
					}
				}
				//将top_Y与bottom_Y之间的点设置为白色
				for(int i=top_Y;i<=bottom_Y;i++)
					cvSet2D(BinaryImg,i,wt,s_new);
				ht=BinaryImg->height;//强制跳出此列循环
			}	
		}
	}

    //横向填充
	for(int ht=0;ht<BinaryImg->height;ht++)
	{
		for(int wt=0;wt<BinaryImg->width;wt++)
		{	
			s=cvGet2D(BinaryImg,ht,wt);
			if(255==s.val[0])
			{
				left_X=wt;//找到最左边的白色点
				for(int m=BinaryImg->width-1;m>=0;m--)
				{
					s=cvGet2D(BinaryImg,ht,m);
					if(255==s.val[0])
					{
						right_X=m;
						break; //跳出寻找最右边点的循环
					}
				}
				//将left_X与right_X之间的点设置为白色
				for(int i=left_X;i<=right_X;i++)
					cvSet2D(BinaryImg,ht,i,s_new);
				wt=BinaryImg->width;//强制跳出此行循环
			}
		}
	}
	//显示图像
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnContour()
{
	// TODO: 在此添加命令处理程序代码
	//轮廓提取
	// 创建内存空间
	CvMemStorage * storage2=cvCreateMemStorage(0);
    //提取轮廓
	cvFindContours(BinaryImg,storage2,&contour,sizeof(CvContour),CV_RETR_EXTERNAL);
	//将轮廓绘制到图像blackImg上
    IplImage* balckImg;
	balckImg=cvCreateImage(cvGetSize(BinaryImg),8,1);
	cvZero(balckImg);
	if(contour)
		cvDrawContours(balckImg,contour,cvScalarAll(255),cvScalarAll(255),100);
	//显示图像
	m_Cimage.mSetImg(balckImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnIdentify()
{
	// TODO: 在此添加命令处理程序代码
	//进行轮廓的匹配
	//载入模板
	IplImage *sample[4];
	sample[0]=cvLoadImage("contour\\0.bmp",0);
	sample[1]=cvLoadImage("contour\\1.bmp",0);
	sample[2]=cvLoadImage("contour\\2.bmp",0);
	sample[3]=cvLoadImage("contour\\3.bmp",0);
	
	//提取模板轮廓
	CvMemStorage *storage_sample=NULL;
	storage_sample=cvCreateMemStorage(0);
	cvClearMemStorage(storage_sample);
	CvSeq* contours_data[4];
	for(int i=0;i<4;i++)
		contours_data[i]=0;
	for(int i=0;i<4;i++)
		cvFindContours(sample[i],storage_sample,&contours_data[i],sizeof(CvContour),CV_RETR_EXTERNAL);
 	   //进行轮廓匹配
	double xiangshidu[4];//匹配系数数组
	memset(xiangshidu,0,sizeof(xiangshidu));
	xiangshidu[0]=cvMatchShapes(contour,contours_data[0],2);
	for(int i=0;i<4;i++)	
	{
		xiangshidu[i]=cvMatchShapes(contour,contours_data[i],2,0);
	   
	}
	//查找相似度最小的值，即为最匹配的车型
	int best_match=0;
	double min_xiangshidu=xiangshidu[0];//将第一个匹配值假设为最小值
	for(int i=0;i<4;i++)
	{	
		if(xiangshidu[i]<min_xiangshidu)
		{
			min_xiangshidu=xiangshidu[i];
			best_match=i;
		}
	}
	//输出分类结果
		switch (best_match)
		{
		case 0:
			AfxMessageBox("小汽车");
		    break;
		case 1:
			AfxMessageBox("SUV");
			break;
		case 2:
			AfxMessageBox("商务车");
			break;
		case 3:
			AfxMessageBox("面包车");
			break;
		default:
			AfxMessageBox("识别失败");
			break;
		}
}
