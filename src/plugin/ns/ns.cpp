/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2015 Synergy Si Ltd
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

#include "ns.h"

#include "SecureSocket.h"
#include "SecureListenSocket.h"
#include "arch/Arch.h"
#include "base/Log.h"

#include <iostream>

SecureSocket* g_secureSocket = NULL;
SecureListenSocket* g_secureListenSocket = NULL;
Arch* g_arch = NULL;
Log* g_log = NULL;

extern "C" {

void
init(void* log, void* arch)
{
	if (g_log == NULL) {
		g_log = new Log(reinterpret_cast<Log*>(log));
	}

	if (g_arch == NULL) {
		g_arch = new Arch(reinterpret_cast<Arch*>(arch));
	}
}

int
initEvent(void (*sendEvent)(const char*, void*))
{
	return 0;
}

void*
invoke(const char* command, void** args)
{
	IEventQueue* arg1 = NULL;
	SocketMultiplexer* arg2 = NULL;
	if (args != NULL) {
		arg1 = reinterpret_cast<IEventQueue*>(args[0]);
		arg2 = reinterpret_cast<SocketMultiplexer*>(args[1]);
	}

	if (strcmp(command, "getSecureSocket") == 0) {
		if (g_secureSocket != NULL) {
			delete g_secureSocket;
		}
		g_secureSocket = new SecureSocket(arg1, arg2);
		g_secureSocket->initSsl(false);
		return g_secureSocket;
	}
	else if (strcmp(command, "getSecureListenSocket") == 0) {
		if (g_secureListenSocket != NULL) {
			delete g_secureListenSocket;
		}
		g_secureListenSocket = new SecureListenSocket(arg1, arg2);
		return g_secureListenSocket;
	}
	else {
		return NULL;
	}
}

int
cleanup()
{
	if (g_secureSocket != NULL) {
		delete g_secureSocket;
	}

	if (g_secureListenSocket != NULL) {
		delete g_secureListenSocket;
	}

	return 0;
}

}