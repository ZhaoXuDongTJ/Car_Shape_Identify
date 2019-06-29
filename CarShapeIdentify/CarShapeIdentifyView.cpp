// CarShapeIdentifyView.cpp : CCarShapeIdentifyView ���ʵ��
//

#include "stdafx.h"
#include "CarShapeIdentify.h"

#include "CarShapeIdentifyDoc.h"
#include "CarShapeIdentifyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarShapeIdentifyView

IMPLEMENT_DYNCREATE(CCarShapeIdentifyView, CView)

BEGIN_MESSAGE_MAP(CCarShapeIdentifyView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCarShapeIdentifyView ����/����

CCarShapeIdentifyView::CCarShapeIdentifyView()
{
	// TODO: �ڴ˴���ӹ������

}

CCarShapeIdentifyView::~CCarShapeIdentifyView()
{
}

BOOL CCarShapeIdentifyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CCarShapeIdentifyView ����

void CCarShapeIdentifyView::OnDraw(CDC* pDC)
{
	CCarShapeIdentifyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	myimg & img=pDoc->m_Cimage;
    CRect m_rect;
    GetClientRect(&m_rect) ;
    //����ͼ��Ŀ�Ⱥ͸߶ȣ��ܹ�����ԭʼ��С��ʾ
    m_rect.right=img.Width();
    m_rect.bottom=img.Height();
    img.DrawToHDC(pDC->GetSafeHdc(),m_rect);
}


// CCarShapeIdentifyView ��ӡ

BOOL CCarShapeIdentifyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CCarShapeIdentifyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CCarShapeIdentifyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CCarShapeIdentifyView ���

#ifdef _DEBUG
void CCarShapeIdentifyView::AssertValid() const
{
	CView::AssertValid();
}

void CCarShapeIdentifyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCarShapeIdentifyDoc* CCarShapeIdentifyView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarShapeIdentifyDoc)));
	return (CCarShapeIdentifyDoc*)m_pDocument;
}
#endif //_DEBUG


// CCarShapeIdentifyView ��Ϣ�������
