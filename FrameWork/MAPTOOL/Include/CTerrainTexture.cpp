// CTerrainTexture.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "CTerrainTexture.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MAPTOOLView.h"
#include "Form.h"
#include "TerrainObject.h"
#include "QuadObject.h"
#include "CubeObject.h"
#include "ToolGameObject.h"

// CTerrainTexture 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTexture, CDialogEx)

CTerrainTexture::CTerrainTexture(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CTerrainTexture, pParent) ,m_iTreeIndex(0)
	, m_iTreeParentIndex(0)
{

}

CTerrainTexture::~CTerrainTexture()
{
}

void CTerrainTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, Terrain_Texture_Tree, m_Tree_Terrain_Texture);
	DDX_Control(pDX, Terrain_Texture_Picture, m_picture_TerrainTexture);
	DDX_Control(pDX, IDC_CHECK1, m_Check_Terrain);
	DDX_Control(pDX, IDC_CHECK2, m_Check_Quad);
	DDX_Control(pDX, IDC_CHECK3, m_Check_Cube);
	DDX_Control(pDX, Radio_Zminus, m_Radio_Zminus);
	DDX_Control(pDX, Radio_Zplus, m_Radio_Zplus);
	DDX_Control(pDX, Radio_Xminus, m_Radio_Xminus);
	DDX_Control(pDX, Radio_Xplus, m_Radio_Xplus);
	DDX_Control(pDX, Radio_Yminus, m_Radio_Yminus);
	DDX_Control(pDX, Radio_Yplus, m_Radio_Yplus);
	DDX_Control(pDX, Radio_ALL, m_Radio_ALL);
}


BEGIN_MESSAGE_MAP(CTerrainTexture, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, Terrain_Texture_Tree, &CTerrainTexture::OnTvnSelchangedTextureTree)
	ON_BN_CLICKED(IDC_BUTTON1, &CTerrainTexture::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CTerrainTexture::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTerrainTexture::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CTerrainTexture::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CTerrainTexture 메시지 처리기


void CTerrainTexture::Init_TreeCtrl()
{
	HTREEITEM hResource = m_Tree_Terrain_Texture.InsertItem(L"Texture");

	//리소스 절대 경로
	CString strFolder = L"..\\..\\Client\\Bin\\Resource\\Texture\\TILES*.*";

	//재귀함수 :파일 이름찾는것
	Get_FileList(strFolder,&hResource);


	m_Tree_Terrain_Texture.EnsureVisible(hResource);
}

void CTerrainTexture::Get_FileList(const CString& strFolder, HTREEITEM* ppItem)
{

	CFileFind finder;
	CString strFolderItem, strFileExt;
	BOOL bWorking = finder.FindFile(strFolder);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		//상위 디렉토리 건너뛰기
		if (finder.IsDots()) continue;

		//폴더 속 폴더이면 재귀호출해주기
		if (finder.IsDirectory())
		{
			HTREEITEM hFolder = m_Tree_Terrain_Texture.InsertItem(finder.GetFileName(), *ppItem);
			strFolderItem = finder.GetFilePath() + L"\\*.*";
			m_Tree_Terrain_Texture.SetItemData(*ppItem, (DWORD)*ppItem);
			Get_FileList(strFolderItem,&hFolder);
		}
		//파일 일때
		if (finder.IsArchived())
		{
			TCHAR szTemp[MAX_PATH];
			lstrcpy(szTemp, finder.GetFileName());
			PathRemoveExtension(szTemp);
			strFileExt = szTemp;
			HTREEITEM hTexture = m_Tree_Terrain_Texture.InsertItem(strFileExt, *ppItem, TVI_LAST);
			m_Tree_Terrain_Texture.SetItemData(*ppItem, (DWORD)*ppItem);

			m_tFilePath = finder.GetFilePath();

		}
	}


}



void CTerrainTexture::Set_PictureCtrl(const CString& strCur)
{
	m_tCurTexturePath = strCur;

	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_picture_TerrainTexture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_picture_TerrainTexture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	CImage image;//불러오고 싶은 이미지를 로드할 CImage 
	if (FAILED(image.Load(strCur)))
	{
		ERR_MSG(L"Texture 이미지 로드에 실패했습니다.");//이미지 로드
	}

	image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);//DC 해제

}

void CTerrainTexture::Set_XYZRadioEnable(BOOL bEnable)
{
	if (bEnable)
	{
		m_Radio_Xminus.EnableWindow(TRUE);
		m_Radio_Xplus .EnableWindow(TRUE);
		m_Radio_Yminus.EnableWindow(TRUE);
		m_Radio_Yplus .EnableWindow(TRUE);
		m_Radio_Zminus.EnableWindow(TRUE);
		m_Radio_Zplus .EnableWindow(TRUE);
	}
	else
	{
		m_Radio_Xminus.SetCheck(FALSE);
		m_Radio_Xplus.SetCheck(FALSE);
		m_Radio_Yminus.SetCheck(FALSE);
		m_Radio_Yplus.SetCheck(FALSE);
		m_Radio_Zminus.SetCheck(FALSE);
		m_Radio_Zplus.SetCheck(FALSE);
		m_Radio_Xminus.EnableWindow(FALSE);
		m_Radio_Xplus.EnableWindow(FALSE);
		m_Radio_Yminus.EnableWindow(FALSE);
		m_Radio_Yplus.EnableWindow(FALSE);
		m_Radio_Zminus.EnableWindow(FALSE);
		m_Radio_Zplus.EnableWindow(FALSE);
		m_Radio_ALL.EnableWindow(FALSE);
	}


}

BOOL CTerrainTexture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Init_TreeCtrl();

	m_pForm = dynamic_cast<CForm*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 0));


	CString ObjTypeName;
	static_cast<CToolGameObject*>(m_pForm->m_pNowObject)->Get_TypeName(ObjTypeName);

	if (ObjTypeName == L"Terrain")
	{
		m_Check_Terrain.SetCheck(TRUE);
	}
	else if (ObjTypeName == L"Quad")
	{
		m_Check_Quad.SetCheck(TRUE);

	}
	else if (ObjTypeName == L"Cube")
	{
		m_Check_Cube.SetCheck(TRUE);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainTexture::OnTvnSelchangedTextureTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItemRoot, hItemParent, hItemCur;

	CString strRoot, strParent, strCur,strTxt;

	hItemCur = pNMTreeView->itemNew.hItem;
	hItemParent = this->m_Tree_Terrain_Texture.GetParentItem(hItemCur);
	hItemRoot = this->m_Tree_Terrain_Texture.GetParentItem(hItemParent);

	strRoot = m_Tree_Terrain_Texture.GetItemText(hItemRoot);
	strParent = m_Tree_Terrain_Texture.GetItemText(hItemParent);
	strCur = m_Tree_Terrain_Texture.GetItemText(hItemCur);

	// 처음 루트만 선택되었을때
	if (strRoot.IsEmpty() && strParent.IsEmpty() )
		return;

	// 폴더가 선택되었을때
	if ( strRoot.IsEmpty() && strParent && strCur)
	{
		_int index = 0;
		// 현재 선택한 Index 트리핸들
		HTREEITEM hItem = m_Tree_Terrain_Texture.GetSelectedItem();

		// 현재 선택한 트리핸들의 부모핸들의 자식핸들의 첫번째 Index (TILE_000.png 파일검색)
		HTREEITEM hChild = m_Tree_Terrain_Texture.GetChildItem(this->m_Tree_Terrain_Texture.GetParentItem(hItem));


		while (hChild)
		{
			//둘이 비교해서 같으면 그 인덱스이다.
			if (hChild == hItem) break;

			// 아니면 다음 인덱스로 넘겨주고 인덱스 변수 증가시킴
			hChild = m_Tree_Terrain_Texture.GetNextItem(hChild, TVGN_NEXT);
			++index;
		}
		m_iTreeParentIndex = index;

		return;
	}

	strTxt.Append(L"../../Client/Bin/Resource/" + strRoot);
	strTxt.Append(L"/" + strParent);
	strTxt.Append(L"/" + strCur + L".png");
	//폴더이름
	m_tCurTexture = strParent;
	//파일이름
	m_tCurTextureState = strCur;
	Set_PictureCtrl(strTxt);



	*pResult = 0;
}


void CTerrainTexture::OnBnClickedButton1()
{
	CMAPTOOLView* pView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0,1));
	CForm* pForm = dynamic_cast<CForm*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0,0));

	int index = 0;

	// 현재 선택한 Index 트리핸들
	HTREEITEM hItem = m_Tree_Terrain_Texture.GetSelectedItem();

	// 현재 선택한 트리핸들의 부모핸들의 자식핸들의 첫번째 Index (TILE_000.png 파일검색)
	HTREEITEM hChild = m_Tree_Terrain_Texture.GetChildItem(this->m_Tree_Terrain_Texture.GetParentItem(hItem));


	while (hChild)
	{
		//둘이 비교해서 같으면 그 인덱스이다.
		if (hChild == hItem) break;

		// 아니면 다음 인덱스로 넘겨주고 인덱스 변수 증가시킴
		hChild = m_Tree_Terrain_Texture.GetNextItem(hChild, TVGN_NEXT);
		++index;
	}
	m_iTreeIndex = index;

	//파일이름
	pView->m_tTexturePath = pForm->m_strFileName = m_tCurTextureState;
	//폴더이름
	pView->m_tTextureFolder = pForm->m_strFolderName = m_tCurTexture;

	CGameObject* pObj = nullptr;
	pObj = m_pForm->m_pNowObject;

	if (m_Check_Terrain.GetCheck())
	{
		static_cast<CToolGameObject*>(pObj)->Set_Path((TCHAR*)(LPCTSTR)m_tCurTexture, (TCHAR*)(LPCTSTR)m_tCurTextureState);

		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		pForm->UpdateData(false);

		if (!GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(pView->m_pDevice, TEXTURETYPE::TEX_NORMAL, m_tCurTexturePath, m_tCurTextureState, 1);

		static_cast<CToolGameObject*>(pObj)->Set_Texture(CTexture::Create(pView->m_pDevice));
		CTexture* pTexture = static_cast<CToolGameObject*>(pObj)->Get_Texture();
		pTexture->setTexture(GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL));
	}
	else if (m_Check_Quad.GetCheck())
	{
		static_cast<CToolGameObject*>(pObj)->Set_Path((TCHAR*)(LPCTSTR)m_tCurTexture, (TCHAR*)(LPCTSTR)m_tCurTextureState);
		pForm->UpdateData(false);

		if (!GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(pView->m_pDevice, TEXTURETYPE::TEX_NORMAL, m_tCurTexturePath, m_tCurTextureState, 1);

		static_cast<CToolGameObject*>(pObj)->Set_Texture(CTexture::Create(pView->m_pDevice));
		CTexture* pTexture = static_cast<CToolGameObject*>(pObj)->Get_Texture();
		pTexture->setTexture(GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL));
	}
	else if (m_Check_Cube.GetCheck())
	{
		// 0번 : Z-
		// 1번 : Z+
		// 2번 : X-
		// 3번 : X+
		// 4번 : Y-
		// 5번 : Y+
		_int TextureIndex = -1;
		if (m_Radio_Zminus.GetCheck())
		{
			TextureIndex = 0;
		}
		else if (m_Radio_Zplus.GetCheck())
		{
			TextureIndex = 1;
		}
		else if (m_Radio_Xminus.GetCheck())
		{
			TextureIndex = 2;
		}
		else if (m_Radio_Xplus.GetCheck())
		{
			TextureIndex = 3;
		}
		else if (m_Radio_Yminus.GetCheck())
		{
			TextureIndex = 4;
		}
		else if (m_Radio_Yplus.GetCheck())
		{
			TextureIndex = 5;
		}
		

		if (m_Radio_ALL.GetCheck())
		{
			for(_uint i = 0 ; i < 6 ; ++i)
			{
				static_cast<CToolGameObject*>(pObj)->Set_Path((TCHAR*)(LPCTSTR)m_tCurTexture, (TCHAR*)(LPCTSTR)m_tCurTextureState, i);
				pForm->UpdateData(false);

			if (!GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL))
				Insert_Texture(pView->m_pDevice, TEXTURETYPE::TEX_NORMAL, m_tCurTexturePath, m_tCurTextureState, 1);
				// 깡통텍스처 만든다.
				static_cast<CToolGameObject*>(pObj)->Set_Texture(CTexture::Create(pView->m_pDevice), i);
				// 깡통텍스처 불러온다.
				CTexture* pTexture = static_cast<CToolGameObject*>(pObj)->Get_Texture(i);
				// 깡통에 그림을 넣어준다.
				pTexture->setTexture(GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL));
			}
		}
		else
		{
			static_cast<CToolGameObject*>(pObj)->Set_Path((TCHAR*)(LPCTSTR)m_tCurTexture, (TCHAR*)(LPCTSTR)m_tCurTextureState, TextureIndex);
			pForm->UpdateData(false);
			if (!GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL))
				Insert_Texture(pView->m_pDevice, TEXTURETYPE::TEX_NORMAL, m_tCurTexturePath, m_tCurTextureState, 1);
			// 깡통텍스처 만든다.
			static_cast<CToolGameObject*>(pObj)->Set_Texture(CTexture::Create(pView->m_pDevice), TextureIndex);
			// 깡통텍스처 불러온다.
			CTexture* pTexture = static_cast<CToolGameObject*>(pObj)->Get_Texture(TextureIndex);
			// 깡통에 그림을 넣어준다.
			pTexture->setTexture(GetTexture(m_tCurTextureState, TEXTURETYPE::TEX_NORMAL));
		}
	}

}

void CTerrainTexture::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	m_Check_Quad.SetCheck(FALSE);
	m_Check_Cube.SetCheck(FALSE);
	Set_XYZRadioEnable(FALSE);
}


void CTerrainTexture::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	m_Check_Terrain.SetCheck(FALSE);
	m_Check_Cube.SetCheck(FALSE);
	Set_XYZRadioEnable(FALSE);

}


void CTerrainTexture::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	m_Check_Terrain.SetCheck(FALSE);
	m_Check_Quad.SetCheck(FALSE);
	Set_XYZRadioEnable(TRUE);
}
