#pragma once
#include <iostream>
#include <filesystem>
#include <chrono>

#include <memory>
#include <functional>
#include <algorithm>

#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

// error C4996: 'WSAAsyncSelect': Use WSAEventSelect() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <winsock.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <tchar.h>