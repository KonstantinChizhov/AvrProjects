#include <avr/io.h>

#include "iopins.h"
#include "pinlist.h"

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

int main()
{
	Keys::ReadKeys();
	return 0;
}
