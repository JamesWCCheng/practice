#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <Windows.h>
#include <functional>
template<typename Callable>
HRESULT exception_safe_wrapper(Callable && function)
{
	try
	{
		function();
		return S_OK;
	}
	catch (std::bad_alloc const&)
	{
		return E_OUTOFMEMORY;
	}
	catch (...)
	{
		return GetLastError();
	}
}
