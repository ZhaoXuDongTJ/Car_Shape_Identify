// CarShapeIdentifyDoc.h : CCarShapeIdentifyDoc ��Ľӿ�
//

#include "HeImg.h"
#pragma once

class CCarShapeIdentifyDoc : public CDocument
{
protected: // �������л�����
	CCarShapeIdentifyDoc();
	DECLARE_DYNCREATE(CCarShapeIdentifyDoc)

// ����
public:
	IplImage *bk_img;		//����ͼ��
	IplImage *FrImg;		//ǰ��ͼ��	
	IplImage *BkGray;		//�����Ҷ�ͼ��
	IplImage *FrGray;		//ǰ���Ҷ�ͼ��
	IplImage * SubImg;		//������ͼ��
	IplImage * BinaryImg;	//��ֵͼ��
	CvSeq * contour;       //�������������
	heimg m_Cimage; //��ʾͼ��
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CCarShapeIdentifyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadbk();
	afx_msg void OnLoadfr();
	afx_msg void OnImgsub();
	afx_msg void OnBinary();
	afx_msg void OnOpen();
	afx_msg void OnRemovenoise();
	afx_msg void OnImgfill();
	afx_msg void OnContour();
	afx_msg void OnIdentify();
};


