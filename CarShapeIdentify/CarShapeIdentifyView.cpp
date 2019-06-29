// CarShapeIdentifyView.cpp : CCarShapeIdentifyView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCarShapeIdentifyView 构造/析构

CCarShapeIdentifyView::CCarShapeIdentifyView()
{
	// TODO: 在此处添加构造代码

}

CCarShapeIdentifyView::~CCarShapeIdentifyView()
{
}

BOOL CCarShapeIdentifyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCarShapeIdentifyView 绘制

void CCarShapeIdentifyView::OnDraw(CDC* pDC)
{
	CCarShapeIdentifyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	myimg & img=pDoc->m_Cimage;
    CRect m_rect;
    GetClientRect(&m_rect) ;
    //计算图像的宽度和高度，能够按照原始大小显示
    m_rect.right=img.Width();
    m_rect.bottom=img.Height();
    img.DrawToHDC(pDC->GetSafeHdc(),m_rect);
}


// CCarShapeIdentifyView 打印

BOOL CCarShapeIdentifyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCarShapeIdentifyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCarShapeIdentifyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCarShapeIdentifyView 诊断

#ifdef _DEBUG
void CCarShapeIdentifyView::AssertValid() const
{
	CView::AssertValid();
}

void CCarShapeIdentifyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCarShapeIdentifyDoc* CCarShapeIdentifyView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCarShapeIdentifyDoc)));
	return (CCarShapeIdentifyDoc*)m_pDocument;
}
#endif //_DEBUG


// CCarShapeIdentifyView 消息处理程序
