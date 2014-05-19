#include <stdio.h>
#include <net/callbacks.h>
#include <net/inet_address.h>
#include <net/buffer.h>
#include <net/tcp_server.h>
#include <net/tcp_connection.h>
#include <net/event_loop.h>

using namespace thefox;

TcpServer *svr = NULL;

void onConnection(const TcpConnectionPtr &conn)
{
	printf("Connection success connID=%d\r\n", conn->connId());
}

void onClose(const TcpConnectionPtr &conn)
{
	printf("Connection close connID=%d\r\n", conn->connId());
}

void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
{
	printf("收到数据 总共读:%u 总共写:%u\r\n", conn->readBytes(), conn->writeBytes());
	svr->send(conn, buf->peek(), buf->readableBytes());
	buf->retrieveAll();
}

int main(int argc, char *argv[])
{
    WSADATA wsd;
    WSAStartup(MAKEWORD(2, 2), &wsd);

	EventLoop loop;
	svr = new TcpServer(&loop, "MyIocpDemo");
	svr->setConnectionCallback(onConnection);
	svr->setCloseCallback(onClose);
	svr->setMessageCallback(onMessage);
	svr->start(InetAddress(7901));
	loop.start();
	loop.join();

	WSACleanup();
	delete svr;
	return 0;
}