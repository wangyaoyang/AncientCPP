#include "stdafx.h"
#include "MyTree.h"

#define		COPYRIGHT_VERSION_INFO	"【Tree View】Copyright〖R〗J.W. 1999.3 Version 1.1"

/*************************************************************************/
CMyTree::CMyTree(void)
{
	m_count = NULL;
	m_hTreeRoot = NULL;
	m_hCurrItem = NULL;
}

CMyTree::~CMyTree(void)
{
}

void CMyTree::InitImageList(CTreeCtrl& tree,UINT BITMAP_ID)
{
	m_ImageList.Create(BITMAP_ID,16,0,RGB(255,0,255));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
}

void CMyTree::InitImageList(CTreeCtrl& tree,LPCTSTR BITMAP_ID)
{
	m_ImageList.Create(BITMAP_ID,16,0,RGB(255,0,255));
	m_ImageList.SetBkColor(GetSysColor(COLOR_WINDOW));

	/// Attach image list to Tree
	tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
}

/*************************************************************************/

BOOL CMyTree::TraverseTree( CTreeCtrl& tree,UINT code )
{
	return TraverseChilden( tree,tree.GetRootItem(),code );
}

BOOL CMyTree::TraverseChilden( CTreeCtrl& tree,HTREEITEM hChilden,UINT code )
{
	for(	HTREEITEM	hTempItem = hChilden;
			hTempItem != NULL;
			hTempItem = tree.GetNextSiblingItem( hTempItem ) )
	{
		TREE_ITEM	tItem;
		switch( code )
		{
		case TRAVERSE_OPERATE_LEAF:
			tItem.strData = tree.GetItemText( hTempItem );
			tItem.dwdData = tree.GetItemData( hTempItem );
			m_hCurrItem = hTempItem;
			LeafOperate( tree,tItem );
			tree.SetItemText( hTempItem,tItem.strData );
			tree.SetItemData( hTempItem,tItem.dwdData );
			break;
		case TRAVERSE_EXPAND:		tree.Expand( hTempItem,TVE_EXPAND );	break;
		case TRAVERSE_COLLAPSE:		tree.Expand( hTempItem,TVE_COLLAPSE );	break;
		case TRAVERSE_TVE_TOGGLE:	tree.Expand( hTempItem,TVE_TOGGLE );	break;
		default:;
		}
		if( tree.ItemHasChildren( hTempItem ) )
			TraverseChilden( tree,tree.GetChildItem( hTempItem ),code );
	}
	return TRUE;
}

/*************************************************************************/

BOOL CMyTree::Serialize(CTreeCtrl& tree,CArchive& ar)
{
	CString		copyright_version;

	if( ar.IsStoring() )
	{
		copyright_version = COPYRIGHT_VERSION_INFO;
		m_count = tree.GetCount();
		ar << copyright_version;
		ar << m_count;
		Serialize_Search( tree,ar,tree.GetRootItem() );
	}
	else
	{
		ar >> copyright_version;
		if( copyright_version != COPYRIGHT_VERSION_INFO ) return FALSE;
		ar >> m_count;
		Serialize_Search( tree,ar,TVI_ROOT );
	}
	return TRUE;
}

void CMyTree::Serialize_Search( CTreeCtrl& tree,CArchive& ar,HTREEITEM hItem )
{
	HTREEITEM	childitem;
	HTREEITEM	siblingitem;

	if( ar.IsStoring() )
	{
		Serialize_Leaf( tree,ar,hItem );
		for(	siblingitem = tree.GetChildItem( hItem );
				siblingitem != NULL;
				siblingitem = tree.GetNextSiblingItem( siblingitem ) )
		{
			Serialize_Search( tree,ar,siblingitem );
		}
		Serialize_Leaf( tree,ar,NULL );
	}
	else
	{
		while( m_count > 0 )
		{
			if( (childitem = Serialize_Leaf( tree,ar,hItem ))==NULL ) return;
			Serialize_Search( tree,ar,childitem );
		}
	}
}

HTREEITEM CMyTree::Serialize_Leaf( CTreeCtrl& tree,CArchive& ar,HTREEITEM hItem )
{
	HTREEITEM	childitem = NULL;
	TREE_ITEM	Item;
	POSITION	pos = NULL;
	CString		TextIndex;
	char		buff[64];
	int			IconGroup = 0;

	if( ar.IsStoring() )
	{
		if( hItem == NULL )
		{
			Item.dwdData |= NO_MORE_CHILD;
			Item.Serialize( ar );
			return NULL;
		}
		else
		{
			TextIndex = tree.GetItemText( hItem );
			//Item.strData = TextIndex.Left( TextIndex.Find("【") );
			Item.strData = TextIndex;
			Item.dwdData = tree.GetItemData( hItem );
			Item.Serialize( ar );
			m_count ++;
		}
	}
	else
	{
		Item.Serialize( ar );
		if( Item.dwdData & NO_MORE_CHILD ) return NULL;
		else
		{
			int		icon = (Item.dwdData & 0x7f000000) >> 24;
			//TextIndex = Item.strData + "【" + itoa(Item.dwdData,buff,10) + "】";
			TextIndex = Item.strData;
			childitem = tree.InsertItem( TextIndex, icon, icon+1, hItem );
			tree.SetItemData( childitem, Item.dwdData );
			m_count --;
		}
	}
	return childitem;
}

/*************************************************************************/

HTREEITEM CMyTree::InsertTreeLeaf( CTreeCtrl& tree,TREE_ITEM& tItem,int IconGroup,HTREEITEM ParentItem )
{
	HTREEITEM	fItem = NULL;
	HTREEITEM	hItem = NULL;
	int			ImageNo = 0;
	
	if( ParentItem ) fItem = ParentItem; else fItem = TVI_ROOT;
	tItem.dwdData = (tItem.dwdData&0x80ffffff) | ( (IconGroup&0x0000007f) << 24 );
	hItem = tree.InsertItem( tItem.strData, IconGroup, IconGroup+1, fItem, TVI_LAST );
	tree.SetItemData( hItem, tItem.dwdData );

	return hItem;
}

HTREEITEM CMyTree::DeleteTreeLeaf(CTreeCtrl& tree,HTREEITEM ItemTobeDel)
{
	HTREEITEM	hItemDeleted = NULL;
	
	if( ItemTobeDel ) hItemDeleted = ItemTobeDel; else hItemDeleted = tree.GetSelectedItem();

	m_hCurrItem = tree.GetParentItem( hItemDeleted );
	tree.DeleteItem( hItemDeleted );
	if( m_hCurrItem == NULL ) {
		m_hTreeRoot = NULL;
		return NULL;
	}
	return m_hCurrItem;
}

HTREEITEM CMyTree::SortTree( CTreeCtrl& tree,int SortParam )
{
	HTREEITEM	hItemToSort = tree.GetSelectedItem();

	if( SortParam==0 ) tree.SortChildren( hItemToSort );
	else {
		TV_SORTCB	SortCB;
		SortCB.hParent = hItemToSort;
		SortCB.lpfnCompare = CompareFunc;
		SortCB.lParam = (LPARAM) SortParam;
		tree.SortChildrenCB( &SortCB );
	}
	return hItemToSort;
}

DWORD CMyTree::LeafOperate( CTreeCtrl& tree,TREE_ITEM& pItem )
{
	return 0;
}

DWORD CMyTree::GoThrow( CTreeCtrl& tree,DWORD param,HTREEITEM hItem )
{
	return 0;
}

void CMyTree::SetCurrentLeaf( HTREEITEM hItem )
{
	m_hCurrItem = hItem;
}

HTREEITEM CMyTree::GetCurrentLeaf( void )
{
	return m_hCurrItem;
}

BOOL CMyTree::SetLeafText( CTreeCtrl& tree,LPCTSTR text,HTREEITEM hItem )
{
	HTREEITEM	hTreeItem = NULL;

	if( hItem ) hTreeItem = hItem; else hTreeItem = m_hCurrItem;
	return tree.SetItemText( hTreeItem, text );
}

BOOL CMyTree::SetLeafData( CTreeCtrl& tree,DWORD data,HTREEITEM hItem )
{
	HTREEITEM	hTreeItem = NULL;

	if( hItem ) hTreeItem = hItem; else hTreeItem = m_hCurrItem;
	return tree.SetItemData( hTreeItem, 0x7FFFFFFF&data );	//去掉最高位
}

CString CMyTree::GetLeafText( CTreeCtrl& tree,HTREEITEM hItem )
{
	HTREEITEM	hTreeItem = NULL;

	if( hItem ) hTreeItem = hItem; else hTreeItem = m_hCurrItem;
	return tree.GetItemText( hTreeItem );
}

DWORD CMyTree::GetLeafData( CTreeCtrl& tree,HTREEITEM hItem )
{
	HTREEITEM	hTreeItem = NULL;

	if( hItem ) hTreeItem = hItem; else hTreeItem = m_hCurrItem;
	return (0x7FFFFFFF&tree.GetItemData( hTreeItem ));	//去掉最高位
}

/*************************************************************************/
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return 0;
}
/*************************************************************************/

TREE_ITEM::TREE_ITEM(void)
{
	dwdData = 0;
}

void TREE_ITEM::Serialize( CArchive& ar )
{
	if (ar.IsStoring()) {
		ar << strData;
		ar << dwdData;
	}
	else {
		ar >> strData;
		ar >> dwdData;
	}
}
