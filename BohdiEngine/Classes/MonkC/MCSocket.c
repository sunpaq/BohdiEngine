#ifndef WIN32

#include "MCSocket.h"
#include "MCLog.h"

fun(dumpInfo, void)) as(MCSocketClientInfo)
	printf("accept a client: %s\n", it->address.sa_data);
}

constructor(MCSocketClientInfo)) {
	as(MCSocketClientInfo)
		it->returnSfd = 0;
		it->address_len = 0;
	}
	return any;
}

static void create_and_bind_socket(struct MCSocket* this, MCSocketType socket_type, char* ip, char* port)
{
	//int sfd = ;
	//BOOL isServer;
	struct addrinfo hint, *result;
	runtime_log("begin prepare hint.\n");

	//prepare hint
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	//hint.ai_protocol = IPPROTO_IP;
	switch(socket_type){
		case MCSocket_Server_TCP:
			hint.ai_socktype = SOCK_STREAM;
			hint.ai_flags = AI_PASSIVE;
			this->isServer = 1;
		break;
		case MCSocket_Server_UDP:
			hint.ai_socktype = SOCK_DGRAM;
			hint.ai_flags = AI_PASSIVE;
			this->isServer = 1;
		break;
		case MCSocket_Client_TCP:
			hint.ai_socktype = SOCK_STREAM;
			this->isServer = 0;
		break;
		case MCSocket_Client_UDP:
			hint.ai_socktype = SOCK_DGRAM;
			this->isServer = 0;
		break;
	}
	//get addrinfo linklist
	if(getaddrinfo(ip, port, &hint, &result)){
		error_log("can not getaddrinfo, return null\n");
		return;
	}
	runtime_log("get the addrinfo linklist.\n");

	//try every addrinfo in list to create and bind until success
	struct addrinfo* iterator;
	for(iterator = result; iterator!=null; iterator = iterator->ai_next)
	{		
		this->sfd = socket(iterator->ai_family, iterator->ai_socktype, iterator->ai_protocol);
		if (this->isServer)
		{
			if (this->sfd==-1)
				continue;
			if (!bind(this->sfd, iterator->ai_addr, iterator->ai_addrlen))
			{
				//memcpy(&this->peeraddrinfo, iterator, sizeof(struct addrinfo));
				break;//bind success
			}			
				
		}else{
			if (this->sfd==-1)
				continue;
			if (!connect(this->sfd, iterator->ai_addr, iterator->ai_addrlen))
				break;//connect success
		}
		close(this->sfd);
	}
	//free linklist
	freeaddrinfo(result);
	//return socket fd
	//return sfd;
}

//EADDRINUSE
//EBADF
//ENOTSOCK
//EOPNOTSUPP
fun(listeningStart, int)) as(MCSocket)
	if(it->isServer!=1)return -1;
	return listen(it->sfd, MCSocket_Queue_Length);
}

fun(acceptARequest, struct MCSocketClientInfo*)) as(MCSocket)
	if (it->isServer!=1)return null;
	struct MCSocketClientInfo* clientinfo = MCSocketClientInfo(alloc(MCSocketClientInfo));
	clientinfo->returnSfd = accept(it->sfd, &clientinfo->address, &clientinfo->address_len);
	return clientinfo;
}

fun(receive, void)) {
    //recv(int, void *, size_t, int)
}

fun(receiveFrom, void)) {
    //recvfrom(int, void *, size_t, int, struct sockaddr *restrict, socklen_t *restrict)
}

fun(receiveMsg, void)) {
    //recvmsg(int, struct msghdr *, int)
}

fun(sendInfo, void)) {
    //send(int, const void *, size_t, int)
}

fun(sendTo, void)) {
    //sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t)
}

fun(sendMsg, void)) {
    //sendmsg(int, const struct msghdr *, int)
}

fun(release, void)) as(MCSocket)
    close(it->sfd);
}

constructor(MCSocket), MCSocketType socket_type, char* ip, char* port) {
	MCObject(any);
	as(MCSocket)
		create_and_bind_socket(it, socket_type, ip, port);
	}
	dynamic(MCSocket)
		funbind(listeningStart);
		funbind(acceptARequest);
		funbind(receive);
		funbind(receiveFrom);
		funbind(receiveMsg);
		funbind(sendInfo);
		funbind(sendTo);
		funbind(sendMsg);
		funbind(release);
	}
	return any;
}


#endif