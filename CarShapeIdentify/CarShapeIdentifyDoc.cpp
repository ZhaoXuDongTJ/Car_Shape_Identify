// CarShapeIdentifyDoc.cpp : CCarShapeIdentifyDoc ���ʵ��
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


// CCarShapeIdentifyDoc ����/����

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




// CCarShapeIdentifyDoc ���л�

void CCarShapeIdentifyDoc::Serialize(CArchive& ar)
{
}


// CCarShapeIdentifyDoc ���

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


// CCarShapeIdentifyDoc ����

void CCarShapeIdentifyDoc::OnLoadbk()
{
	// TODO: �ڴ���������������
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|�κ��ļ�|*.*||";
	CFileDialog dlg1(TRUE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	 
	//���ļ��Ի���
	if(dlg1.DoModal()==IDOK)
	{
		if(0==(bk_img=cvLoadImage(dlg1.GetPathName(),1)))
		{  
			AfxMessageBox("�޷����ļ���");
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
	// TODO: �ڴ���������������
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|�κ��ļ�|*.*||";
	CFileDialog dlg1(TRUE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	 
	//���ļ��Ի���
	if(dlg1.DoModal()==IDOK)
	{
		if(0==(FrImg=cvLoadImage(dlg1.GetPathName(),1)))
		{  
			AfxMessageBox("�޷����ļ���");
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
	// TODO: �ڴ���������������
	//��ͼ������ת��Ϊ�Ҷ�ͼ��
	BkGray=cvCreateImage(cvGetSize(bk_img),8,1);
	cvCvtColor(bk_img,BkGray,CV_BGR2GRAY);
	FrGray=cvCreateImage(cvGetSize(bk_img),8,1);
	cvCvtColor(FrImg,FrGray,CV_BGR2GRAY);

	//��ͼ����и�˹ƽ������
	cvSmooth(BkGray,BkGray,CV_GAUSSIAN,5);
	cvSmooth(FrGray,FrGray,CV_GAUSSIAN,5);

	//��ͼ�������������
	SubImg=cvCreateImage(cvGetSize(bk_img),8,1);
	cvAbsDiff(FrGray,BkGray,SubImg);

    //��ʾͼ��
	m_Cimage.mSetImg(SubImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnBinary()
{
	// TODO: �ڴ���������������
	//ͼ���ֵ��
	BinaryImg=cvCreateImage(cvGetSize(bk_img),8,1);
	cvThreshold(SubImg,BinaryImg,0,255,CV_THRESH_OTSU);	

	//��ʾͼ��
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnOpen()
{
	// TODO: �ڴ���������������
	//���帯ʴ�������õĺ�,
	IplConvKernel * kernel_5_5;
	kernel_5_5=cvCreateStructuringElementEx(5,5,2,2,CV_SHAPE_ELLIPSE,0);
	IplConvKernel * kernel_7_7;
	kernel_7_7=cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE,0);

	//��ǰ��ͼ����п��պ������Գ�ȥ�ӵ㣬�ָ���˶�����
	//��ͼ����и�ʴ����
	cvErode(BinaryImg,BinaryImg,kernel_5_5,1);

	//��ͼ�������������
	cvDilate(BinaryImg,BinaryImg,kernel_7_7,1);

	//��ʾͼ��
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnRemovenoise()
{
	// TODO: �ڴ���������������
	//ȥ����С����ɢ��ͨ����

	//���Ƚ�ͼ����һ��
	IplImage * BinaryImg2;
	BinaryImg2=cvCloneImage(BinaryImg);

	// �����ڴ�ռ�
	CvMemStorage * storage=cvCreateMemStorage(0);

	//����ͳ����ͨ��������������
	CvSeq * area_contour=NULL;
	int Number_contours=0;   //��������
	double Area=0;           //�������
	CvScalar s_fill_new;
	s_fill_new.val[0]=0;
	CvScalar loDiff;
	loDiff.val[0]=10;
	CvScalar upDiff;
	upDiff.val[0]=10;
	//����ͼ���е����ⲿ����
	Number_contours=cvFindContours(BinaryImg,storage,&area_contour,sizeof(CvContour),CV_RETR_EXTERNAL);	
	//ͳ������������������С����ֵ�Ľ���ɾ��
	for(CvSeq * c=area_contour;c!=NULL;c=c->h_next)
	{
		//����������
		Area=fabs(cvContourPerimeter(c));
		if(Area<30)
		{
			for(int i=0;i<1;i++)
			{
				//ȡ�����ϵ�һ������Ϊͼ��������ӵ�
				CvPoint * seedpoint=CV_GET_SEQ_ELEM(CvPoint,c,i);
				//����ˮ����㷨��С���������Ϊ����ɫ
				cvFloodFill(BinaryImg2,*seedpoint,s_fill_new,loDiff,upDiff,NULL,4,0);
			}
		}
	}
	BinaryImg=cvCloneImage(BinaryImg2);
	//ͼ����ʾ
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnImgfill()
{
	// TODO: �ڴ���������������
	//�����������ڲ����
	CvScalar s;
	CvScalar s_new;
	s_new.val[0]=255;
	//��¼����е�����λ��
	int left_X=0;
	//��¼����е�����λ��
    int right_X=0;
	//��¼����е�����λ��
	int top_Y=0;
	//��¼����е�����λ��
	int bottom_Y=0;

	//�������
	for(int wt=0;wt<BinaryImg->width;wt++)
	{
		for(int ht=0;ht<BinaryImg->height;ht++)
		{	
			s=cvGet2D(BinaryImg,ht,wt);
			if(255==s.val[0])
			{
				top_Y=ht;//�ҵ����ϱߵİ�ɫ��
				for(int m=BinaryImg->height-1;m>=0;m--)
				{
					s=cvGet2D(BinaryImg,m,wt);
					if(255==s.val[0])
					{
						bottom_Y=m;
						break; //����Ѱ�����±ߵ��ѭ��
					}
				}
				//��top_Y��bottom_Y֮��ĵ�����Ϊ��ɫ
				for(int i=top_Y;i<=bottom_Y;i++)
					cvSet2D(BinaryImg,i,wt,s_new);
				ht=BinaryImg->height;//ǿ����������ѭ��
			}	
		}
	}

    //�������
	for(int ht=0;ht<BinaryImg->height;ht++)
	{
		for(int wt=0;wt<BinaryImg->width;wt++)
		{	
			s=cvGet2D(BinaryImg,ht,wt);
			if(255==s.val[0])
			{
				left_X=wt;//�ҵ�����ߵİ�ɫ��
				for(int m=BinaryImg->width-1;m>=0;m--)
				{
					s=cvGet2D(BinaryImg,ht,m);
					if(255==s.val[0])
					{
						right_X=m;
						break; //����Ѱ�����ұߵ��ѭ��
					}
				}
				//��left_X��right_X֮��ĵ�����Ϊ��ɫ
				for(int i=left_X;i<=right_X;i++)
					cvSet2D(BinaryImg,ht,i,s_new);
				wt=BinaryImg->width;//ǿ����������ѭ��
			}
		}
	}
	//��ʾͼ��
	m_Cimage.mSetImg(BinaryImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnContour()
{
	// TODO: �ڴ���������������
	//������ȡ
	// �����ڴ�ռ�
	CvMemStorage * storage2=cvCreateMemStorage(0);
    //��ȡ����
	cvFindContours(BinaryImg,storage2,&contour,sizeof(CvContour),CV_RETR_EXTERNAL);
	//���������Ƶ�ͼ��blackImg��
    IplImage* balckImg;
	balckImg=cvCreateImage(cvGetSize(BinaryImg),8,1);
	cvZero(balckImg);
	if(contour)
		cvDrawContours(balckImg,contour,cvScalarAll(255),cvScalarAll(255),100);
	//��ʾͼ��
	m_Cimage.mSetImg(balckImg);
	UpdateAllViews(NULL);
}

void CCarShapeIdentifyDoc::OnIdentify()
{
	// TODO: �ڴ���������������
	//����������ƥ��
	//����ģ��
	IplImage *sample[4];
	sample[0]=cvLoadImage("contour\\0.bmp",0);
	sample[1]=cvLoadImage("contour\\1.bmp",0);
	sample[2]=cvLoadImage("contour\\2.bmp",0);
	sample[3]=cvLoadImage("contour\\3.bmp",0);
	
	//��ȡģ������
	CvMemStorage *storage_sample=NULL;
	storage_sample=cvCreateMemStorage(0);
	cvClearMemStorage(storage_sample);
	CvSeq* contours_data[4];
	for(int i=0;i<4;i++)
		contours_data[i]=0;
	for(int i=0;i<4;i++)
		cvFindContours(sample[i],storage_sample,&contours_data[i],sizeof(CvContour),CV_RETR_EXTERNAL);
 	   //��������ƥ��
	double xiangshidu[4];//ƥ��ϵ������
	memset(xiangshidu,0,sizeof(xiangshidu));
	xiangshidu[0]=cvMatchShapes(contour,contours_data[0],2);
	for(int i=0;i<4;i++)	
	{
		xiangshidu[i]=cvMatchShapes(contour,contours_data[i],2,0);
	   
	}
	//�������ƶ���С��ֵ����Ϊ��ƥ��ĳ���
	int best_match=0;
	double min_xiangshidu=xiangshidu[0];//����һ��ƥ��ֵ����Ϊ��Сֵ
	for(int i=0;i<4;i++)
	{	
		if(xiangshidu[i]<min_xiangshidu)
		{
			min_xiangshidu=xiangshidu[i];
			best_match=i;
		}
	}
	//���������
		switch (best_match)
		{
		case 0:
			AfxMessageBox("С����");
		    break;
		case 1:
			AfxMessageBox("SUV");
			break;
		case 2:
			AfxMessageBox("����");
			break;
		case 3:
			AfxMessageBox("�����");
			break;
		default:
			AfxMessageBox("ʶ��ʧ��");
			break;
		}
}
