#pragma once


// CTerrainTexture 대화 상자

class CTerrainTexture : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTexture)

public:
	CTerrainTexture(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTerrainTexture();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTerrainTexture };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void Init_TreeCtrl();
	void Get_FileList(CString strFolder,HTREEITEM* ppItem);
	void Set_PictureCtrl(CString strCur);
public:
	CTreeCtrl m_Tree_Terrain_Texture;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTextureTree(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_picture_TerrainTexture;
	CString m_tCurTexturePath; //총경로
	CString m_tCurTexture; // 파일이름
	CString m_tCurTextureState; //폴더이름
	afx_msg void OnBnClickedButton1();
	int m_iTreeIndex;
	int m_iTreeParentIndex;
	CString m_tFilePath;


};
