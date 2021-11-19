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
private:
	CMAPTOOLView* m_pMapToolView;
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
	afx_msg void OnBnClickedQuadCreate();
	afx_msg void OnBnClickedCubeCreate();
	afx_msg void OnBnClickedMakefilter();
	afx_msg void OnBnClickedDeletefilter();
	afx_msg void OnBnClickedModifyfilter();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTvnSelchangedObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMSetfocusObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLbnKillfocusTerrain();
	afx_msg void OnBnClickedCloneObject();
	afx_msg void OnBnClickedObjectSave();
	afx_msg void OnBnClickedObjectLoad();
	afx_msg void OnBnClickedCubesave();
	afx_msg void OnBnClickedCubeLoad();
	afx_msg void OnBnClickedModifyParent();
	afx_msg void OnBnClickedItemCreate();
	void ReSize_Detail();
	void ReSize_ObjectInfo();
	void LinkResourceAndVariableTerrain();
	void LinkResourceAndVariableQuad();
	void Set_SRP(const _vec3& vecScale,const _vec3& vecRot,const _vec3& vecPos);
public:	template<typename ... Args> std::string string_format(const std::string& format, Args ... args);
public:	HTREEITEM FindTreeData(HTREEITEM& hItem, CString& text);

public:
	// 다이얼로그
	CTerrainTexture m_tTerrainTexture;
	CButton m_bWireFrame;
	CButton m_Button_Zbuffer;
	CListBox m_List_Terrain;

	CString m_strFolderName;
	CString m_strFileName;

	CButton m_Button_Scale;
	CButton m_Button_Roation;
	CButton m_Button_Position;

	//현재 선택된 Object변수
	VTXINFO m_tNowInfo;
	// 현재 선택된 오브젝트
	CGameObject* m_pNowObject;
	//TerrainList개수
	int m_TerrainlistIndex;

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

	CTreeCtrl m_Tree_Object;
	// Object Tree의 Root
	HTREEITEM m_TreeObjectRoot;
	HTREEITEM m_TreeItemRoot;
	HTREEITEM m_TreeNow;
	CString m_strTreeFilterName;
	CString m_strObjectName;

	INIManager* m_pIniManager;
	CButton m_bAlphaTest;
	CComboBox m_Combo_ItemList;
	CComboBox m_Combo_MonsterList;
	afx_msg void OnBnClickedItemSave();
	afx_msg void OnBnClickedItemLoad();
	float m_fItemRadius;
	CButton m_Button_LightOnOff;
};

template<typename ... Args>
std::string CForm::string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	auto buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

