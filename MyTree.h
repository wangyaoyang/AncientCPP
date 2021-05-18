#ifndef	__DIR_TREE__
#define	__DIR_TREE__

#include <afx.h>
#include <afxcoll.h>

#define	NO_MORE_CHILD		0x80000000
#define	NO_MORE_CHILD_N		0x7FFFFFFF
#define	TREE_LEAF_TYPE0		0x00000000
#define	TREE_LEAF_TYPE1		0x00000001
#define	TREE_LEAF_TYPE2		0x00000000
#define	TREE_LEAF_TYPE3		0x00000001
#define	TREE_LEAF_TYPE4		0x00000000
#define	TREE_LEAF_TYPE5		0x00000001
#define	TREE_LEAF_TYPE6		0x00000001
#define	TREE_LEAF_TYPE7		0x00000001
#define	TREE_LEAF_TYPE8		0x00000001
#define	TREE_LEAF_TYPE9		0x00000001
#define	LIST_HEAD			0x00000000
#define	LIST_TAIL			0x00000001

#define	KEY_INS_GRAY		0x002d
#define	KEY_DEL_GRAY		0x002e
#define	KEY_INS_WHITE		0x0060
#define	KEY_DEL_WHITE		0x006d

#define	TRAVERSE_OPERATE_LEAF	0x0001
#define	TRAVERSE_EXPAND			0x0002
#define	TRAVERSE_COLLAPSE		0x0003
#define	TRAVERSE_TVE_TOGGLE		0x0004

//#define	MAX_LABEL_LENTH		256

//##ModelId=39C1EC2B00DF
class TREE_ITEM : public CObject
{
public:
	//##ModelId=39C1EC2B00FE
	TREE_ITEM(void);
	//##ModelId=39C1EC2B00F6
	virtual void	Serialize( CArchive& ar );
public:
	//##ModelId=39C1EC2B00F5
	CString	strData;
	//##ModelId=39C1EC2B00F4
	DWORD	dwdData;
};

//##ModelId=39C1EC11038B
class CMyTree
{
private:
	//##ModelId=39C1EC1103BD
	int				m_count;
protected:
	//##ModelId=39C1EC1103B3
	HTREEITEM		m_hTreeRoot;
private:
	//##ModelId=39C1EC1200EF
	BOOL			TraverseChilden( CTreeCtrl&,HTREEITEM,UINT );
	//##ModelId=39C1EC1200DA
	HTREEITEM		Serialize_Leaf( CTreeCtrl& tree,CArchive& ar,HTREEITEM hItem );
	//##ModelId=39C1EC1200CF
	void			Serialize_Search(CTreeCtrl& tree,CArchive& ar,HTREEITEM hItem);
protected:
	//##ModelId=39C1EC1103AB
	CImageList		m_ImageList;
	//##ModelId=39C1EC1200C8
	virtual DWORD	LeafOperate( CTreeCtrl&,TREE_ITEM& );
	//##ModelId=39C1EC1200BE
	virtual	DWORD	GoThrow( CTreeCtrl&,DWORD,HTREEITEM=NULL );
public:
	//##ModelId=39C1EC110395
	HTREEITEM		m_hCurrItem;
public:
	//##ModelId=39C1EC1200BC
	CMyTree(void);
	//##ModelId=39C1EC12009F
	~CMyTree(void);
	///////////////////////////////////////////////////////
	//##ModelId=39C1EC12008C
	BOOL			Serialize(CTreeCtrl& tree,CArchive& ar);
	//##ModelId=39C1EC120089
	BOOL			TraverseTree( CTreeCtrl&,UINT );
	///////////////////////////////////////////////////////
	//##ModelId=39C1EC12004E
	HTREEITEM		InsertTreeLeaf( CTreeCtrl&,TREE_ITEM&,int=0,HTREEITEM=NULL );
	//##ModelId=39C1EC120044
	HTREEITEM		DeleteTreeLeaf( CTreeCtrl&,HTREEITEM=NULL );
	//##ModelId=39C1EC120031
	HTREEITEM		SortTree( CTreeCtrl&,int );
	//##ModelId=39C1EC12002F
	HTREEITEM		GetCurrentLeaf( void );
	//##ModelId=39C1EC120011
	void			SetCurrentLeaf( HTREEITEM );
	//##ModelId=39C1EC1103EA
	BOOL			SetLeafText( CTreeCtrl&,LPCTSTR,HTREEITEM = NULL );
	//##ModelId=39C1EC1103E7
	CString			GetLeafText( CTreeCtrl&,HTREEITEM = NULL );
	//##ModelId=39C1EC1103CC
	BOOL			SetLeafData( CTreeCtrl&,DWORD,HTREEITEM = NULL );
	//##ModelId=39C1EC1103C9
	DWORD			GetLeafData( CTreeCtrl&,HTREEITEM = NULL );
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	//##ModelId=39C1EC1103BE
	void			InitImageList(CTreeCtrl&,UINT);
	//##ModelId=39C1EC1103C1
	void			InitImageList(CTreeCtrl&,LPCTSTR);
};

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#endif	//__DIR_TREE__
