#pragma once


#include "CTerrainTexture.h"
#include "CTerrainPicture.h"
// CForm 폼 보기


class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedCreatebutton(); //터레인생성 함수
	// 다이얼로그
	CTerrainTexture m_tTerrainTexture;
	CTerrainPicture m_tTerrainPickture;


	//변수
	int m_dwTerrainX;
	int m_dwTerrainZ;
	int m_dwInterval;
	int m_iTerrain_Detail;


private:
	CMAPTOOLView* m_pMapToolView;
public:
	CButton m_bWireFrame;
	afx_msg void OnBnClickedTexture();
	afx_msg void OnDeltaposDetailspin(NMHDR* pNMHDR, LRESULT* pResult);
};


