#pragma once


// CTerrainPicture 대화 상자

class CTerrainPicture : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainPicture)

public:
	CTerrainPicture(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTerrainPicture();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTerrainPicture };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void Get_FileList(CString strFolder);
	void Init_TerrainPicture();
	void Init_ScrollBar();
public:
	CStatic m_Picture_Terrain;
	virtual BOOL OnInitDialog();
	CString m_tFilePath;
	_bool m_bFirst = false;
	_int m_iPictureX = 1;
	_int m_iPictureY = 1;
	afx_msg void OnBnClickedButton1();
	CScrollBar m_Scroll_Picture;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	POINT	c, origin; //Scrollbar 위치에 따른 Point 위치 변수
	// Original Point 위치
};
