
// EcApp.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CEcAppApp:
// �аѾ\��@�����O�� EcApp.cpp
//

class CEcAppApp : public CWinApp
{
public:
	CEcAppApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CEcAppApp theApp;