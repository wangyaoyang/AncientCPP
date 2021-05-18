#include "stdafx.h"
#include "math.h"
#include "GraphicElements.h"
#include "\tmndev\tmncpps\Calculat.h"

static	int static_h0 = 100;
static	int	static_v0 = 100;
static	int	N0 = 10;

CPointBase::CPointBase(double x,double y,double z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void CPointBase::m_X(double x)			{	m_x = x;	}
void CPointBase::m_Y(double y)			{	m_y = y;	}
void CPointBase::m_Z(double z)			{	m_z = z;	}
double CPointBase::m_X()				{	return m_x;	}
double CPointBase::m_Y()				{	return m_y;	}
double CPointBase::m_Z()				{	return m_z;	}

C3DPoint::C3DPoint(double x,double y,double z)
		: CPointBase(x,y,z)
{
	m_Set(x,y,z);
}

void C3DPoint::m_Set(double x,double y,double z)
{
	m_X(x);
	m_Y(y);
	m_Z(z);

	double	k = sqrt(3)/3;
	double	h = x - k*z;
	double	v = y - k*z;
	m_u = static_h0 + (int)(N0*h);
	m_v = static_v0 - (int)(N0*v);
}

int C3DPoint::m_U()					{	return m_u;	}

int C3DPoint::m_V()					{	return m_v;	}

void C3DPoint::m_Color(COLORREF clr)	{	m_color = clr;	}

COLORREF C3DPoint::m_Color()			{	return m_color;		}

void C3DPoint::m_Draw(CDC* pCDC)
{
	//pCDC->Rect(m_u,m_v,m_u+1,m_v+1);
	pCDC->MoveTo( m_u,m_v );
	pCDC->LineTo( m_u+1,m_v+1 );
}

void C3DPoint::m_Descrip(BOOL yn)
{
}

C3DLine::C3DLine()
{
}

C3DPoint C3DLine::m_A()				{	return m_pointA;	}
C3DPoint C3DLine::m_B()				{	return m_pointB;	}
COLORREF C3DLine::m_Color()			{	return m_color;		}
int C3DLine::m_Style()				{	return m_style;		}
void C3DLine::m_Color(COLORREF c)	{	m_color = c;		}
void C3DLine::m_Style(int s)		{	m_style = s;		}
void C3DLine::m_A(C3DPoint p)		{	m_pointA = p;		}
void C3DLine::m_B(C3DPoint p)		{	m_pointB = p;		}
void C3DLine::m_Set(C3DPoint a,C3DPoint b)	{	m_A(a);	m_B(b);	}
void C3DLine::m_Draw(CDC* pCDC)
{
	pCDC->MoveTo( m_pointA.m_U(),m_pointA.m_V() );
	pCDC->LineTo( m_pointB.m_U(),m_pointB.m_V() );
}

C3DCoordination::C3DCoordination(int t,int n,int u,int v)
{
	m_type = t;
	N0 = n;
	m_Set( u,v );
}

C3DPoint C3DCoordination::m_GetO()		{	return m_o;		}
C3DLine C3DCoordination::m_GetOX()		{	return m_ox;	}
C3DLine C3DCoordination::m_GetOY()		{	return m_oy;	}
C3DLine C3DCoordination::m_GetOZ()		{	return m_oz;	}

void C3DCoordination::m_Set( int u,int v )
{
	static_h0 = u;
	static_v0 = v;
	m_o.m_Set( 0,0,0 );
	C3DPoint	x(m_o.m_X()+10,m_o.m_Y(),m_o.m_Z());
	C3DPoint	X(m_o.m_X()-10,m_o.m_Y(),m_o.m_Z());
	C3DPoint	y(m_o.m_X(),m_o.m_Y()+10,m_o.m_Z());
	C3DPoint	Y(m_o.m_X(),m_o.m_Y()-10,m_o.m_Z());
	C3DPoint	z(m_o.m_X(),m_o.m_Y(),m_o.m_Z()+10);
	C3DPoint	Z(m_o.m_X(),m_o.m_Y(),m_o.m_Z()-10);
	m_ox.m_Set( X,x );
	m_oy.m_Set( Y,y );
	m_oz.m_Set( Z,z );
}

void C3DCoordination::m_Draw(CDC* pCDC)
{
	int	x = 0;
	int	y = 0;
	x = m_o.m_U();			y = m_o.m_V();			pCDC->TextOut( x+1,y-5,"o" );
	x = m_ox.m_B().m_U();	y = m_ox.m_B().m_V();	pCDC->TextOut( x+1,y-5,"x" );
	x = m_oy.m_B().m_U();	y = m_oy.m_B().m_V();	pCDC->TextOut( x+1,y-5,"y" );
	x = m_oz.m_B().m_U();	y = m_oz.m_B().m_V();	pCDC->TextOut( x+1,y-5,"z" );
	m_o.m_Draw(pCDC);
	m_ox.m_Draw(pCDC);
	m_oy.m_Draw(pCDC);
	m_oz.m_Draw(pCDC);
}

C3DGraphic::C3DGraphic()
{
	for( int i=0;i<3;i++ )
	{
		m_range[i].left = -10000.0f;	m_range[i].leftOpen = TRUE;
		m_range[i].right = 10000.0f;	m_range[i].rightOpen = TRUE;
	}
}

void C3DGraphic::m_Set(char* fxy,char* fxz,char* fyz,
					   ValueRange x,ValueRange y,ValueRange z)
{
	m_Fxy = CString( fxy );
	m_Fxz = CString( fxz );
	m_Fyz = CString( fyz );
	memcpy( &m_range[0],&x,sizeof(ValueRange) );
	memcpy( &m_range[1],&y,sizeof(ValueRange) );
	memcpy( &m_range[2],&z,sizeof(ValueRange) );
}

void C3DGraphic::m_DrawFlat( CDC* pCDC,int flatNo,CString fxyz,ValueRange range )
{
	if( fxyz == "" || fxyz.GetLength() > 255 ) return;
	char			f[256];
	int				i = 0;
	int				hh = 0;
	int				vv = 0;
	double			x = 0.0f;
	double			y = 0.0f;
	double			z = 0.0f;

	memset( f,0x00,256 );	strcpy( f,fxyz );
	switch( flatNo )
	{
	case FLAT_XOY:	//	在XOY平面，方程为 y = f(x,z),其中 z 恒等于 0
		for( i=0;f[i];i++ ) if( f[i] == 'z' || f[i] == 'Z' ) f[i] = '0';
		break;
	case FLAT_XOZ:	//	在XOZ平面，方程为 z = f(x,y),其中 y 恒等于 0
		for( i=0;f[i];i++ ) if( f[i] == 'y' || f[i] == 'Y' ) f[i] = '0';
		break;
	case FLAT_YOZ:	//	在YOZ平面，方程为 z = f(y,x),其中 x 恒等于 0
		for( i=0;f[i];i++ ) if( f[i] == 'x' || f[i] == 'X' ) f[i] = '0';
		break;
	default:	return;
	}
	for( hh=0; hh<800; hh++ )
	{
		double	varx = hh-static_h0;
		varx /= N0;
		if( range.leftOpen )	{ if( varx <= range.left ) continue; }
			else				{ if( varx < range.left ) continue; }
		if( range.rightOpen )	{ if( varx >= range.right ) break; }
			else				{ if( varx > range.right ) break; }
		double	vary = 0.0f;
		double	zero = 0.0f;
		CCalculater		cal(2);
		cal.m_SourceString=f;
		BOOL	code = cal.Calculate(&vary,(float)varx);

		switch( flatNo )
		{
		case FLAT_XOY:	x = varx, y = vary, z = zero;		break;
		case FLAT_XOZ:	x = varx, y = zero, z = vary;		break;
		case FLAT_YOZ:	x = zero, y = varx, z = vary;		break;
		}
		C3DPoint	point( x,y,z );
		point.m_Draw( pCDC );
	}
}

#define		SCREEN_WIDTH	800
#define		SCREEN_DEPTH	600

void C3DGraphic::m_DrawFlat( CDC* pCDC,CString fxz,ValueRange rx,ValueRange rz )
{
	if( fxz == "" || fxz.GetLength() > 255 ) return;
	char			f[256];

	int				i = 0;
	int				width = 0;
	int				depth = 0;
	int				zz = 0;
	double			x = 0.0f;
	double			y = 0.0f;
	double			z = 0.0f;

	memset( f,0x00,256 );	strcpy( f,fxz );

	for( depth=-SCREEN_DEPTH/2; depth<SCREEN_DEPTH/2; depth++ )
	{
		double	varz = ((double)depth)/N0;
		if( rz.leftOpen )	{ if( varz <= rz.left ) continue; }
			else			{ if( varz < rz.left ) continue; }
		if( rz.rightOpen )	{ if( varz >= rz.right ) break; }
			else			{ if( varz > rz.right ) break; }
		for( width=0; width<SCREEN_WIDTH; width++ )
		{
			double	temp = width-static_h0;
			double	varx = temp / N0;
			double	vary = 0.0f;

			if( rx.leftOpen )	{ if( varx <= rx.left ) continue; }
				else			{ if( varx < rx.left ) continue; }
			if( rx.rightOpen )	{ if( varx >= rx.right ) break; }
				else			{ if( varx > rx.right ) break; }

			CCalculater		yy(2);
			yy.m_SourceString=f;
			BOOL	code = yy.Calculate(&vary,(float)varx,(float)varz);

			C3DPoint	point( varx,vary,varz );
			point.m_Draw( pCDC );
		}
	}
}

void C3DGraphic::m_Draw(CDC* pCDC)
{
//	m_DrawFlat( pCDC,FLAT_XOY,m_Fxz,m_range[0] );
//	m_DrawFlat( pCDC,FLAT_XOZ,m_Fxy,m_range[0] );
//	m_DrawFlat( pCDC,FLAT_YOZ,m_Fxy,m_range[1] );
	m_DrawFlat( pCDC,m_Fxz,m_range[0],m_range[2] );
}