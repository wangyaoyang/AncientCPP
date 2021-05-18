#ifndef		__SOCKET_MANAGER_H__
#define		__SOCKET_MANAGER_H__


#define		ERR_PORT_EMPTY			-1
#define		ERR_PORT_NOT_ENOUGH		-2
#define		ERR_PORT_TOO_MUCH		-3
#define		ERR_PORT_LISTEN			-4

class CConnection;
class CMessage;

class CServer
{
friend class CConnection;
private:
	CObList		m_portList;
	int			m_MaxPort;
private:
	void	EmptyList();
	int		AddConnection();
protected:
	virtual int	OnAccept();
public:
	int		GetPortNum();
	void	RemoveConnection(int nPort);
	int		Send(int nPort,CMessage& packet);
	int		Read(int nPort,CMessage& packet);
public:
	CServer(int maxport=1);
	~CServer();
};

class CClient
{
private:
	CConnection*	m_Connection;
public:
	CClient();
	~CClient();
	BOOL	Connect(LPCTSTR Address=NULL,UINT nPort=0xffffffff);
	void	RemoveConnection();
	void	Send(CMessage& packet);
	int		Read(CMessage& packet);
};

#endif	//__SOCKET_MANAGER_H__