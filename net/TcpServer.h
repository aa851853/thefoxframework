#ifndef _THEFOX_NET_TCPSERVER_H_
#define _THEFOX_NET_TCPSERVER_H_

#include <base/Types.h>
#include <base/noncopyable.h>
#include <base/scoped_ptr.h>
#include <net/winapi.h>
#include <net/Callbacks.h>
#include <net/InetAddress.h>



namespace thefox
{

class Buffer;
class Acceptor;
class TcpConnection;
class IoCompletionPort;

class TcpServer : noncopyable
{
public:
	TcpServer(const InetAddress &listenAddr, const String &nameArg);
	~TcpServer();
	
	void start();
	
	void stop();
	
    void setConnectionCallback(const ConnectionCallback &cb)
    { _connectionCallback = cb; }

    void setMessageCallback(const MessageCallback &cb)
    { _messageCallback = cb; }

    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    { _writeCompleteCallback = cb; }
private:
	void newConnection(SOCKET sock, const InetAddress &peerAddr);
	void removeConnection(const TcpConnection &conn);
	
	typedef std::map<String, TcpConnectionPtr> ConnectionMap;
	
	scoped_ptr<IoCompletionPort>_iocp;
	scoped_ptr<Acceptor> _acceptor;

	const String _hostport;
	const String _name;
	ConnectionCallback _connectionCallback;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writeCompleteCallback;
	bool _started;
	int _nextConnId;
	ConnectionMap _connections;
};

}

#endif // _THEFOX_NET_TCPSERVER_H_