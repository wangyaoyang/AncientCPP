///////////////////////////////////////////////////////////
#include "stdafx.h"
#ifndef		__TABLE_REPORT_HEADER__
#define		__TABLE_REPORT_HEADER__

//##ModelId=39C1EC0F017F
class CTableRpt
{
private:
	//##ModelId=39C1EC0F01BB
	COLORREF	m_color;
	//##ModelId=39C1EC0F018B
	CImageList		m_ImageList;
public:
	//##ModelId=39C1EC0F0256
	CTableRpt(COLORREF = RGB(255,255,255));
	//##ModelId=39C1EC0F0254
	~CTableRpt(void);
public:
	//##ModelId=39C1EC0F023E
	BOOL		AddColumn(	CListCtrl&,
							LPCTSTR,
							int,
							int = -1,
							int = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
							int = LVCFMT_LEFT);
	//##ModelId=39C1EC0F024B
	BOOL		AddColumn( CListCtrl&,int,int, LPCTSTR );
	//##ModelId=39C1EC0F0210
	BOOL		AddItem( CListCtrl&,int,int,int=0,LPCTSTR="" );
	//##ModelId=39C1EC0F0207
	BOOL		EditItem( CListCtrl&,int,int,LPCTSTR );
	//##ModelId=39C1EC0F0203
	CString		GetRowText( CListCtrl&,int,int );
	//##ModelId=39C1EC0F01F2
	BOOL		DeleteItem( CListCtrl&,int );
	//##ModelId=39C1EC0F01F0
	BOOL		DeleteAll( CListCtrl& );
	//##ModelId=39C1EC0F01ED
	BOOL		DeleteColumn( CListCtrl&,int=0 );
	//////////////////////////////////////////////////////////////
	//##ModelId=39C1EC0F01CF
	void		InitImageList(CListCtrl&,UINT);
	//##ModelId=39C1EC0F01E5
	void		InitImageList(CListCtrl&,LPCTSTR);
};

#endif//		__TABLE_REPORT_HEADER__
