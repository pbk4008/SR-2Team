#pragma once

//===================
// CString -> TCHAR
// (TCHAR*)(LPCTSTR)
//===================


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
	afx_msg void OnBnClickedTexture(); //텍스처 설정 함수
	afx_msg void OnDeltaposDetailspin(NMHDR* pNMHDR, LRESULT* pResult); // Deatil설정 함수
	afx_msg void OnBnClickedTerrainSave(); //Terrain Save 함수
	afx_msg void OnBnClickedTerrainLoad(); //Terrain Load 함수
	afx_msg void OnEnChangeDetail();
	afx_msg void OnLbnSelchangeTerrain();
	afx_msg void OnBnClickedModifybutton();
	afx_msg void OnLbnSetfocusTerrain();
	afx_msg void OnEnChangeMovepower();
	void Save_Terrain(CString strFilePath);
	void Load_Terrain(CString strFilePath);
	void ReSize_Detail();
	void ReSize_TerrainInfo();
	void LinkResourceAndVariable();
	void Set_SRP(const _vec3& vecScale,const _vec3& vecRot,const _vec3& vecPos);
private:
	CMAPTOOLView* m_pMapToolView;
public:
	// 다이얼로그
	CTerrainTexture m_tTerrainTexture;
	CButton m_bWireFrame;
	CListBox m_List_Terrain;

	CString m_strFolderName;
	CString m_strFileName;

	CButton m_Button_Scale;
	CButton m_Button_Roation;
	CButton m_Button_Position;
	CButton m_Button_Zbuffer;

	//Terrain변수
	TERRAININFO m_tTerrainInfo;
	//TerrainList개수
	int m_TerrainlistIndex;
	//TerrainList 마지막 작업 Sel;
	int m_TerrainLastIndex;
	// 현재 선택된 오브젝트
	CGameObject* m_pNowObject;

	//Player위치
	_float m_tPlayerPos[2];

	//Monster위치
	//몬스터 종류필요함 _float m_tMonsterPos[][2];


	//Scale,Rotation,Position 을 가지는 matrix
	_vec3 m_vecScale;
	_vec3 m_vecRotaion;
	_vec3 m_vecPosition;

	int m_iNewTerrainX;
	int m_iNewTerrainZ;
	int m_iNewTerrainInterval;
	float m_fMovePower;
};


