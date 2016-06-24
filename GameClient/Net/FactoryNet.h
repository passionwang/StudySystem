#pragma once

#ifndef __INCLUDE_FACTORY_NET_H__
#define __INCLUDE_FACTORY_NET_H__
#include "TcpNet.h"
#include "UdpNet.h"

//产品类型
enum ENUM_NET_TYPE
{
	enum_net_tcp,
	enum_net_udp
};

class CFactoryNet
{
public:
	INet* CreateNet(enum ENUM_NET_TYPE eType)
	{
		switch (eType)
		{
		case enum_net_tcp:
			{
				return new CTcpNet;
			}
			break;
		case enum_net_udp:
			{
				return new CUdpNet;
			}
			break;
		}

		return NULL;
	}
};


#endif //__INCLUDE_FACTORY_NET_H__