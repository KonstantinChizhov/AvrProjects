#include <avr/io.h>

#include "iopins.h"
#include "pinlist.h"
/*
template<class Rows, class Columns>
class MatrixKeybord
{
	public:
	static const unsigned KeysCount = Rows::Length * Columns::Length;
	typedef typename IO::IoPrivate::SelectSize<KeysCount>::Result DataT;
	static DataT ReadKeys()
	{
		DataT result = 0;
		// TODO: implement
		return result;
	}
private:

};

using namespace IO;
typedef MatrixKeybord<
		PinList<Pb0, Pb1, Pb2, Pb3>,
		PinList<Pb4, Pb5, Pb6, Pb7>
	> Keys;
*/
#define VIA_PTR 0

#if VIA_PTR
__attribute((noinline))
inline void Foo(uint64_t *v)
{
	(*v)<<=1;
}
#endif

uint32_t value = 0;

int main()
{
#if VIA_PTR
	Foo(&value);
#else
	value>>=1;
#endif


	//Keys::ReadKeys();
	return 0;
}
