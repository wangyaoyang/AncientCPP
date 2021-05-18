#ifndef		__GRAPHIC_ELEMENTS_H__
#define		__GRAPHIC_ELEMENTS_H__

#define		FLAT_XOY		1
#define		FLAT_XOZ		2
#define		FLAT_YOZ		3

//	Definition of 3D point
class CPointBase
{
private:	//Value
	double		m_x;
	double		m_y;
	double		m_z;
public:
	CPointBase(double x=0,double y=0,double z=0);
	void		m_X(double x);
	void		m_Y(double y);
	void		m_Z(double z);
	double		m_X();
	double		m_Y();
	double		m_Z();
};

class C3DPoint : public CPointBase
{
private:	//Can not be accessed
	int			m_u;
	int			m_v;
private:	//Attribute
	COLORREF	m_color;
	BOOL		m_description;
public:
	C3DPoint(double x=0,double y=0,double z=0);
	void		m_Set(double x,double y,double z);
	int			m_U();
	int			m_V();
	void		m_Color(COLORREF color);
	COLORREF	m_Color();
public:
	void		m_Descrip(BOOL yn);
	void		m_Draw(CDC* pCDC);
};

//	Definition of 3D line
class C3DLine
{
private:
	C3DPoint	m_pointA;
	C3DPoint	m_pointB;
	COLORREF	m_color;
	int			m_style;
public:
	C3DLine();
	C3DPoint	m_A();
	C3DPoint	m_B();
	COLORREF	m_Color();
	int			m_Style();
	void		m_Style(int);
	void		m_A(C3DPoint);
	void		m_B(C3DPoint);
	void		m_Color(COLORREF);
	void		m_Set(C3DPoint,C3DPoint);
public:
	void		m_Draw(CDC* pCDC);
};

//	Definition of 3D Coordination
class C3DCoordination
{
private:
	int			m_type;
	C3DPoint	m_o;
	C3DLine		m_ox;
	C3DLine		m_oy;
	C3DLine		m_oz;
public:
	C3DCoordination(int type,int n0,int u0,int v0);
	void		m_Set( int u,int v );
	C3DPoint	m_GetO();
	C3DLine		m_GetOX();
	C3DLine		m_GetOY();
	C3DLine		m_GetOZ();
public:
	void		m_Draw(CDC* pCDC);
};

typedef	struct InvalideRange
{
	double	left;
	double	right;
	BOOL	leftOpen;
	BOOL	rightOpen;
} ValueRange;

class C3DGraphic
{
private:
	CString		m_Fxy;
	CString		m_Fxz;
	CString		m_Fyz;
	ValueRange	m_range[3];
private:
	void		m_DrawFlat(CDC* pCDC,int flatNo,CString f,ValueRange r);
	void		m_DrawFlat(CDC* pCDC,CString fxz,ValueRange rx,ValueRange rz );
public:
	C3DGraphic();
	void		m_Set( char* fxy,char* fxz,char* fyz,
						ValueRange x,ValueRange y,ValueRange z );
	void		m_Draw(CDC* pCDC);
};

#endif		//__GRAPHIC_ELEMENTS_H__