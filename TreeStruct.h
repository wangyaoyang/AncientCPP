#ifndef		__TREE_STRUCT_H__
#define		__TREE_STRUCT_H__

#include "Elf.h"

class CTreeStruct : public CElf
{
private:
	CElf*		m_parent;
	CObList		m_children;
};


#endif		//__TREE_STRUCT_H__