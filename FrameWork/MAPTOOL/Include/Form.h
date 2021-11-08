#pragma once


#include "CTerrainTexture.h"
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
	afx_msg void OnBnClickedTexture();
	afx_msg void OnDeltaposDetailspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedTerrainSave();
	void Save_Terrain(CString strFilePath);
	void Load_Terrain(CString strFilePath);
	void ReSize_Detail();
	// 다이얼로그
	CTerrainTexture m_tTerrainTexture;


	//Terrain변수
	int m_dwTerrainX;
	int m_dwTerrainZ;
	int m_dwInterval;
	int m_iTerrain_Detail;

	//List변수
	int m_listIndex;


private:
	CMAPTOOLView* m_pMapToolView;

	std::list<
public:
	CButton m_bWireFrame;
	afx_msg void OnBnClickedTerrainLoad();
	CListBox m_List_Terrain;
};


