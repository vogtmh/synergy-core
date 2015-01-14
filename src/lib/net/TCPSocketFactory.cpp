/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2012 Synergy Si Ltd.
 * Copyright (C) 2002 Chris Schoeneman
 * 
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 * 
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "net/TCPSocketFactory.h"

#include "net/TCPSocket.h"
#include "net/TCPListenSocket.h"
#include "arch/Arch.h"
#include "base/Log.h"

//
// TCPSocketFactory
//

#if defined _WIN32
static const char s_networkSecurity[] = { "ns" };
#else
static const char s_networkSecurity[] = { "libns" };
#endif

TCPSocketFactory::TCPSocketFactory(IEventQueue* events, SocketMultiplexer* socketMultiplexer) :
	m_events(events),
	m_socketMultiplexer(socketMultiplexer)
{
	// do nothing
}

TCPSocketFactory::~TCPSocketFactory()
{
	// do nothing
}

IDataSocket*
TCPSocketFactory::create(bool secure) const
{
	IDataSocket* socket = NULL;
	if (secure) {
		void* args[4] = {
			m_events,
			m_socketMultiplexer,
			Log::getInstance(),
			Arch::getInstance()
		};
		socket = static_cast<IDataSocket*>(
			ARCH->plugin().invoke(s_networkSecurity, "getSecureSocket", args));
	}
	else {
		socket = new TCPSocket(m_events, m_socketMultiplexer);
	}

	return socket;
}

IListenSocket*
TCPSocketFactory::createListen(bool secure) const
{
	IListenSocket* socket = NULL;
	if (secure) {
		void* args[4] = {
			m_events,
			m_socketMultiplexer,
			Log::getInstance(),
			Arch::getInstance()
		};
		socket = static_cast<IListenSocket*>(
			ARCH->plugin().invoke(s_networkSecurity, "getSecureListenSocket", args));
	}
	else {
		socket = new TCPListenSocket(m_events, m_socketMultiplexer);
	}

	return socket;
}
