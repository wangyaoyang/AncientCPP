#ifndef		__TRANSACTION_TREE_H__
#define		__TRANSACTION_TREE_H__

#include "MyTree.h"

class CTransactionTree : public CMyTree
{
private:
	HTREEITEM	SelectInheritor( CTreeCtrl&,DWORD,HTREEITEM );
public:
	CTransactionTree();
	~CTransactionTree();
	DWORD		LeafOperate( CTreeCtrl&,TREE_ITEM& );
	DWORD		GoThrow( CTreeCtrl&,DWORD,HTREEITEM=NULL );
};

#endif		//__TRANSACTION_TREE_H__