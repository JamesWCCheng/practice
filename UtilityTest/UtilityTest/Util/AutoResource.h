#pragma once
#include <Windows.h>
struct Auto_HKey
{
    Auto_HKey(HKEY hKey = NULL)
	{
        this->hKey = hKey;
    }

    ~Auto_HKey()
    {
        if (hKey != NULL)
		{
			::RegCloseKey(hKey);
            hKey = NULL;
        }
    }

	void Hold(HKEY hKey)
	{
		this->hKey = hKey;
	}

    operator HKEY() { return hKey; }

    HKEY hKey;
};

struct Auto_Handle
{
	Auto_Handle(HANDLE handle = NULL)
	{
        this->handle = handle;
    }

    ~Auto_Handle()
    {
        if (handle != NULL)
		{
			::CloseHandle(handle);
            handle = NULL;
        }
    }

	void Hold(HANDLE handle)
	{
		this->handle = handle;
	}

    operator HANDLE() { return handle; }

    HANDLE handle;
};

struct Auto_CriticalSection
{
	Auto_CriticalSection()
	{
		::InitializeCriticalSection(&cs);
	}

	~Auto_CriticalSection()
	{
		::DeleteCriticalSection(&cs);
	}

	operator CRITICAL_SECTION&() { return cs; }

	CRITICAL_SECTION cs;
};

template<typename T>
struct Auto_RawBuffer
{
	Auto_RawBuffer(T* buffer = NULL)
	{
		this->buffer = buffer;
	}

	~Auto_RawBuffer()
	{
		if (buffer != NULL)
		{
			delete[] buffer;
			buffer = NULL;
		}
	}

	void Hold(T* buffer)
	{
		this->buffer = buffer;
	}

	operator T*() { return buffer; }

	T* buffer;
};