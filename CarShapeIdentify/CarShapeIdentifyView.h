// CarShapeIdentifyView.h : CCarShapeIdentifyView ��Ľӿ�
//


#pragma once


class CCarShapeIdentifyView : public CView
{
protected: // �������л�����
	CCarShapeIdentifyView();
	DECLARE_DYNCREATE(CCarShapeIdentifyView)

// ����
public:
	CCarShapeIdentifyDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCarShapeIdentifyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CarShapeIdentifyView.cpp �еĵ��԰汾
inline CCarShapeIdentifyDoc* CCarShapeIdentifyView::GetDocument() const
   { return reinterpret_cast<CCarShapeIdentifyDoc*>(m_pDocument); }
#endif

