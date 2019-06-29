// CarShapeIdentifyDoc.h : CCarShapeIdentifyDoc 类的接口
//

#include "HeImg.h"
#pragma once

class CCarShapeIdentifyDoc : public CDocument
{
protected: // 仅从序列化创建
	CCarShapeIdentifyDoc();
	DECLARE_DYNCREATE(CCarShapeIdentifyDoc)

// 属性
public:
	IplImage *bk_img;		//背景图像
	IplImage *FrImg;		//前景图像	
	IplImage *BkGray;		//背景灰度图像
	IplImage *FrGray;		//前景灰度图像
	IplImage * SubImg;		//做差后的图像
	IplImage * BinaryImg;	//二值图像
	CvSeq * contour;       //存放轮廓的序列
	heimg m_Cimage; //显示图像
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CCarShapeIdentifyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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


