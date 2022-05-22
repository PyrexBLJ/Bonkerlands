#pragma once

#include <cell/cell_fs.h>
#include <cell/pad/libpad.h>
#include <fastmath.h>
#include <initializer_list>
#include <ppu_intrinsics.h>
#include <stdarg.h>
#include <string>
#include <sys/memory.h>
#include <sys/ppu_thread.h>
#include <sys/process.h>
#include <sys/prx.h>
#include <sys/syscall.h>
#include <sys/timer.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <csetjmp>
#include <np.h>
#include <cell/http.h>
#include <sys/synchronization.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <semaphore.h>
#include <pthread.h>
#include <sched.h>

#include <string.hpp>
#include <math.hpp>
#include <symbol.hpp>

#include <system/imports.hpp>

#include <system/syscalls.hpp>
#include <system/memory.hpp>
#include <system/filesystem.hpp>
#include <network/httpclient.hpp>
#include <sys/spu_thread_group.h>
#include <sys/dbg.h>
#include <cell/dbg.h>
#include <cell/sysmodule.h>

#include <stdint.h>
#include <unistd.h>
#include <sys/sys_time.h>
#include <sys/timer.h>

#include <stack>
#include <cell/pad.h>

#include <map>

#define BOOST_NO_RTTI
#define BOOST_NO_EXCEPTIONS
#define BOOST_NO_EXCEPTION_STD_NAMESPACE
#define BOOST_EXCEPTION_DISABLE
#define BOOST_NO_STD_LOCALE
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE
#define BOOST_NO_TYPEID
#define BOOST_NO_CXX11_SMART_PTR
#define BOOST_NO_CXX11_CONSTEXPR
#define BOOST_NO_CXX14_CONSTEXPR
#define BOOST_NO_IOSTREAM
#define BOOST_NO_STD_TYPEINFO

typedef libpsutil::math::vec2_t vec2_t;
typedef libpsutil::math::vec3_t vec3_t;
typedef libpsutil::math::vec4_t vec4_t;
typedef unsigned int ctx_id;
typedef libpsutil::memory::detour detour;
template<typename T>
using symbol = libpsutil::symbol<T>;

using namespace std;

class color {
public:
	unsigned char r, g, b, a;
	inline color() {};
	color(int r, int g, int b, int a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	explicit color(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	explicit color(float r, float g, float b, float a) {
		this->r = r * 255;
		this->g = g * 255;
		this->b = b * 255;
		this->a = a * 255;
	}

	explicit color(const int color) {
		this->r = ((color >> 16) & 0xff) / 255.f;
		this->g = ((color >> 8) & 0xff) / 255.f;
		this->b = ((color & 0xff)) / 255.f;
		this->a = ((color >> 24) & 0xff) / 255.f;
	}

	operator unsigned char* () {
		return (unsigned char*)&this[0];
	}

	void operator=(const vec4_t& vec) {
		this->r = vec.x;
		this->g = vec.y;
		this->b = vec.z;
		this->a = vec.w;
	}
};

static symbol<unsigned int()> Sys_Milliseconds{ 0x10DABCC };
static uint64_t get_time_now()
{
    return Sys_Milliseconds();
}

#define assert(x) __builtin_expect(x)

#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8  char

#define nullptr 0
#ifdef _DEBUG
#define printf _sys_printf
#else
#define printf(...)
#endif

/*
# ========================================================================================= #
# Structs
# ========================================================================================= #
*/

#include "src/defines.hpp"

template< class T >
struct TArray
{
public:
	T* Data;
	int Count;
	int Max;

public:
	TArray()
	{
		Data = NULL;
		Count = Max = 0;
	};

public:
	int Num()
	{
		return this->Count;
	};

	T& operator() (int i)
	{
		return this->Data[i];
	};

	const T& operator() (int i) const
	{
		return this->Data[i];
	};

	void Add(T InputData)
	{
		Data = (T*)_sys_malloc(sizeof(T) * (Count + 1));
		Data[Count++] = InputData;
		Max = Count;
	};

	void Clear()
	{
		if (Data != NULL) {
			_sys_free(Data);
			Data = NULL;
			Count = Max = 0;
		}
	};

	inline T GetIndex(int i) const
	{
		T Addr = (T)((unsigned long)Data[i] ^ i);
		return Addr;
	}
};
struct FNameEntryHash
{
	char name[64];
	const char* name_ptr;
	int bbbb;
	int size;
	void* magic;
};

struct FNameEntry
{
	char pad[8];
	char name[256];
};

struct FString;
struct FString : public TArray< wchar_t >
{
	FString() {};
	~FString() { /*Clear();*/ };
	/*~FString() {
		memset(this->Data, 0, sizeof(wchar_t) * this->Count);

		_sys_free(this->Data);
		this->Max = 0;
		this->Count = 0;
	};*/

	FString(wchar_t* Other)
	{
		this->Max = this->Count = *Other ? (wcslen(Other) + 1) : 0;

		if (this->Count)
			this->Data = Other;
	};

	FString(const wchar_t* Other)
	{
		this->Max = this->Count = *Other ? (wcslen(Other) + 1) : 0;
		if (this->Count)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	FString operator = (wchar_t* Other)
	{
		if (this->Data != Other)
		{
			this->Max = this->Count = *Other ? (wcslen(Other) + 1) : 0;

			if (this->Count)
				this->Data = Other;
		}

		return *this;
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}
};

struct FName
{
	int				Index;
	unsigned char	unknownData[0x4];

	FName() : Index(0) {};

	FName(int i) : Index(i) {};

	~FName() {};

	FName(char* FindName)
	{
		static TArray< int > NameCache;

		for (int i = 0; i < NameCache.Count; ++i)
		{
			if (!strcmp(this->Names()->Data[NameCache(i)]->name, FindName))
			{
				Index = NameCache(i);
				return;
			}
		}

		for (int i = 0; i < this->Names()->Count; ++i)
		{
			if (this->Names()->Data[i])
			{
				if (!strcmp(this->Names()->Data[i]->name, FindName))
				{
					NameCache.Add(i);
					Index = i;
				}
			}
		}
	};

	static char* GetName(int i)
	{
		auto* name = Names()->Data[i];

		if (name == nullptr)
			return "";

		FNameEntryHash entry;
		Unhash_name(&entry, (char*)name->name);
		if (entry.name[0] != '\0')
			return (char*)entry.name;
	}

	static TArray< FNameEntry* >* Names()
	{
		return (TArray< FNameEntry* >*) GNames;
	};

	char* GetName()
	{
		auto* name = this->Names()->Data[Index];

		if (name == nullptr)
			return "";

		FNameEntryHash entry;
		Unhash_name(&entry, (char*)name->name);
		if (entry.name[0] != '\0')
			return (char*)entry.name;

		/*auto widechar_to_multibyte = [](const wchar_t* value, size_t size, char* buffer) {
			for (int i = 0; i < size; i++)
				buffer[i] = (char)value[i];
		};

		FString str;
		FNameEntry_AppendNameToString_a(name, &str);

		if (str.Data[0] == '\0')
			return "";

		char buffer[120];
		widechar_to_multibyte(str.Data, str.Count, buffer);*/
		return "";
	};

	bool operator == (const FName& A) const
	{
		return (Index == A.Index);
	};
};

struct FScriptDelegate
{
	unsigned char UnknownData[0xC];
};

#include <src/sdk/bltps/Core_structs.h>
#include <src/sdk/bltps/Core_classes.h>
#include <src/sdk/bltps/Core_f_structs.h>
#include <src/sdk/bltps/Engine_structs.h>
#include <src/sdk/bltps/Engine_classes.h>
#include <src/sdk/bltps/Engine_f_structs.h>
#include <src/sdk/bltps/GameFramework_structs.h>
#include <src/sdk/bltps/GameFramework_classes.h>
#include <src/sdk/bltps/GameFramework_f_structs.h>
#include <src/sdk/bltps/GFxUI_structs.h>
#include <src/sdk/bltps/GFxUI_classes.h>
#include <src/sdk/bltps/GFxUI_f_structs.h>
#include <src/sdk/bltps/GearboxFramework_structs.h>
#include <src/sdk/bltps/GearboxFramework_classes.h>
#include <src/sdk/bltps/GearboxFramework_f_structs.h>
#include <src/sdk/bltps/WillowGame_structs.h>
#include <src/sdk/bltps/WillowGame_classes.h>
#include <src/sdk/bltps/WillowGame_f_structs.h>

#include <cryptography/hash.hpp>

#include "src/bltps/ui/easing.hpp"
#include "src/sdk/osk.hpp"

#include "src/bltps/base/menu_base.hpp"
#include "src/bltps/base/menu_item.hpp"
#include "src/bltps/base/menu_tab.hpp"

#include "src/bltps/game/functions.hpp"
#include "src/bltps/ui/toast.hpp"
#include "src/scheduler.hpp"

#include "src/bltps/ui/render/gui.hpp"
#include "src/bltps/ui/ui.hpp"
#include "src/bltps/ui/animations.hpp"
