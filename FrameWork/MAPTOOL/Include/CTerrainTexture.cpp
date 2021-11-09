// CTerrainTexture.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "CTerrainTexture.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MAPTOOLView.h"

// CTerrainTexture 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTexture, CDialogEx)

CTerrainTexture::CTerrainTexture(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CTerrainTexture, pParent) ,m_iTreeIndex(0)
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
}


BEGIN_MESSAGE_MAP(CTerrainTexture, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, Terrain_Texture_Tree, &CTerrainTexture::OnTvnSelchangedTextureTree)
	ON_BN_CLICKED(IDC_BUTTON1, &CTerrainTexture::OnBnClickedButton1)
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

void CTerrainTexture::Get_FileList(CString strFolder, HTREEITEM* ppItem)
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

			
		/*	CMAPTOOLView* pView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 1));
			Insert_Texture(pView->m_pDevice, TEXTURETYPE::TEX_NORMAL, m_tFilePath, L"Terrain", 1);*/

		}
	}


}



void CTerrainTexture::Set_PictureCtrl(CString strCur)
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

BOOL CTerrainTexture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Init_TreeCtrl();



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
	pView->m_tTexturePath = m_tCurTextureState;
	//폴더이름
	pView->m_tTextureFolder = m_tCurTexture;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Insert_Texture(pView->m_pDevice, TEXTURETYPE::TEX_NORMAL, m_tCurTexturePath, m_tCurTextureState,1);

}