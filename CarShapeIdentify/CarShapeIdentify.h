// CarShapeIdentify.h : CarShapeIdentify Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCarShapeIdentifyApp:
// �йش����ʵ�֣������ CarShapeIdentify.cpp
//

class CCarShapeIdentifyApp : public CWinApp
{
public:
	CCarShapeIdentifyApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCarShapeIdentifyApp theApp;