#include "stdafx.h"
#include "TableRpt.h"

//////////////////////////////////////////////////////////////////////////////////////////
CTableRpt::CTableRpt(COLORREF color)
{
	m_color = color;
}

CTableRpt::~CTableRpt()
{
}

BOOL CTableRpt::AddColumn(CListCtrl& tab,int nCol,int nWidth,LPCTSTR lpszColumnHeading)
{
	if( nCol == 0 ) {
		tab.SetBkColor( m_color );
		tab.SetTextBkColor( m_color );
		tab.SetTextColor( ~m_color );
	}
	tab.DeleteColumn( nCol );
	return tab.InsertColumn(nCol,lpszColumnHeading,LVCFMT_LEFT,nWidth);
}

BOOL CTableRpt::AddColumn(CListCtrl& tab,LPCTSTR strItem,int nItem,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = tab.GetStringWidth(lvc.pszText) + 15;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	if( nItem == 0 ) {
		tab.SetBkColor( m_color );
		tab.SetTextBkColor( m_color );
		tab.SetTextColor( ~m_color );
	}
	tab.DeleteColumn( nItem );
	return tab.InsertColumn( nItem,&lvc );
}

BOOL CTableRpt::AddItem(CListCtrl& tab,int nItem,int nSubItem,int iImage,LPCTSTR strItem)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	lvItem.iImage = iImage;
	if(nSubItem == 0) return tab.InsertItem(&lvItem);
	return tab.SetItem(&lvItem);
}

BOOL CTableRpt::EditItem(CListCtrl& tab,int nItem,int nSubItem,LPCTSTR lpszText)
{
	return tab.SetItemText( nItem, nSubItem, lpszText );
}

CString CTableRpt::GetRowText(CListCtrl& tab,int Row,int Col)
{
	CString		text;

	text = tab.GetItemText( Row,Col );
	return text;
}

BOOL CTableRpt::DeleteItem(CListCtrl& tab,int nItem)
{
	return tab.DeleteItem( nItem );
}
	
BOOL CTableRpt::DeleteAll(CListCtrl& tab)
{
	return tab.DeleteAllItems();
}

BOOL CTableRpt::DeleteColumn( CListCtrl& tab,int col )
{
	return tab.DeleteColumn(col);
}

void CTableRpt::InitImageList(CListCtrl& tree,UINT BITMAP_ID)
{
	m_ImageList.Create(BITMAP_ID,16,0,RGB(255,0,255));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
}

void CTableRpt::InitImageList(CListCtrl& tree,LPCTSTR BITMAP_ID)
{
	m_ImageList.Create(BITMAP_ID,16,0,RGB(255,0,255));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
}
