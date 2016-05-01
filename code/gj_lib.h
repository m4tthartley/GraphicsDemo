
// gj lib, The Giantjelly library?

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int uint;
typedef unsigned long long u64;
typedef char s8;
typedef short s16;
typedef long long s64;

#define bool int

#define assert(expression) {if (!(expression)) { *((int*)0) = 0; }}
#define null 0
#define PI 3.14159265359f

#define kilobytes(num) (num*1024)
#define megabytes(num) (kilobytes(num)*1024)
#define gigabytes(num) (megabytes(num)*1024)

#define arraySize(array) (sizeof(array)/sizeof(array[0]))
#define fiz(count) for (int i = 0; i < count; ++i)
#define fjz(count) for (int j = 0; j < count; ++j)
#define fkz(count) for (int k = 0; k < count; ++k)

inline float gj_sin (float num) {
	float result = sinf(num);
	return result;
}

inline float gj_cos (float num) {
	float result = cosf(num);
	return result;
}

struct gj_Data {
	char *mem;
	size_t size;
};

struct gj_Mem_Stack {
	char *mem;
	size_t size;
	size_t used;
};

gj_Mem_Stack gj_initMemStack (size_t size) {
	gj_Mem_Stack memStack = {};

	memStack.mem = (char*)VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	if (memStack.mem) {
		memStack.size = size;
	} else {
		assert(false);
	}

	return memStack;
}

char *gj_pushMemStack (gj_Mem_Stack *memStack, size_t size) {
	if (memStack->used + size <= memStack->size) {
		char *result = memStack->mem + memStack->used;
		memStack->used += size;
		return result;
	} else {
		// printf("Ran out of memory %i/%i \n", memStack->used + size, memStack->size);
		assert(false);
	}

	return NULL;
}

void gj_popMemStack (gj_Mem_Stack *memStack, size_t size) {
	if (size <= memStack->used) {
		memStack->used -= size;
	} else {
		assert(false);
	}
}

void testFunction () {

	gj_Mem_Stack memStack = {};

	char *mem = gj_pushMemStack(&memStack, 1024);

	// fuck

}

void GetWin32ErrorString (char *str, int size) {
	DWORD error = GetLastError();
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error,
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), str, size, NULL);
}

gj_Data gj_readFile (char *file, gj_Mem_Stack *memStack) {
	gj_Data data = {};

	HANDLE handle = CreateFileA(file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (handle != INVALID_HANDLE_VALUE) {
		LARGE_INTEGER size64;
		if (GetFileSizeEx(handle, &size64)) {
			unsigned int size = (unsigned int)size64.QuadPart;
			data.mem = gj_pushMemStack(memStack, size);

			if (data.mem) {
				DWORD bytesRead;
				if (ReadFile(handle, data.mem, size, &bytesRead, 0)
					&& size == bytesRead) {
					data.size = size;
				} else {
					VirtualFree(data.mem, 0, MEM_RELEASE);
					data.mem = 0;
				}
			}
		}

		CloseHandle(handle);
	} else {
		// DWORD error = GetLastError();
		char str[256];
		GetWin32ErrorString(str, 256);
		assert(false);
	}

	return data;
}