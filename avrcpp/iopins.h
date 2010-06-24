
//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: Atmel mega AVR, TINY avr AND Xmega Series
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#pragma once

#include "iopin.h"
#include "ioports.h"
#include <avr/io.h>

namespace IO
{

	//Short pin definations 
	#ifdef PORTA
	typedef TPin<Porta, 0> Pa0;
	typedef TPin<Porta, 1> Pa1;
	typedef TPin<Porta, 2> Pa2;
	typedef TPin<Porta, 3> Pa3;
	typedef TPin<Porta, 4> Pa4;
	typedef TPin<Porta, 5> Pa5;
	typedef TPin<Porta, 6> Pa6;
	typedef TPin<Porta, 7> Pa7;
	#endif

	#ifdef PORTB
	typedef TPin<Portb, 0> Pb0;
	typedef TPin<Portb, 1> Pb1;
	typedef TPin<Portb, 2> Pb2;
	typedef TPin<Portb, 3> Pb3;
	typedef TPin<Portb, 4> Pb4;
	typedef TPin<Portb, 5> Pb5;
	typedef TPin<Portb, 6> Pb6;
	typedef TPin<Portb, 7> Pb7;
	#endif

	#ifdef PORTC
	typedef TPin<Portc, 0> Pc0;
	typedef TPin<Portc, 1> Pc1;
	typedef TPin<Portc, 2> Pc2;
	typedef TPin<Portc, 3> Pc3;
	typedef TPin<Portc, 4> Pc4;
	typedef TPin<Portc, 5> Pc5;
	typedef TPin<Portc, 6> Pc6;
	typedef TPin<Portc, 7> Pc7;
	#endif

	#ifdef PORTD
	typedef TPin<Portd, 0> Pd0;
	typedef TPin<Portd, 1> Pd1;
	typedef TPin<Portd, 2> Pd2;
	typedef TPin<Portd, 3> Pd3;
	typedef TPin<Portd, 4> Pd4;
	typedef TPin<Portd, 5> Pd5;
	typedef TPin<Portd, 6> Pd6;
	typedef TPin<Portd, 7> Pd7;
	#endif

	#ifdef PORTE
	typedef TPin<Porte, 0> Pe0;
	typedef TPin<Porte, 1> Pe1;
	typedef TPin<Porte, 2> Pe2;
	typedef TPin<Porte, 3> Pe3;
	typedef TPin<Porte, 4> Pe4;
	typedef TPin<Porte, 5> Pe5;
	typedef TPin<Porte, 6> Pe6;
	typedef TPin<Porte, 7> Pe7;
	#endif

	#ifdef PORTF
	typedef TPin<Portf, 0> Pf0;
	typedef TPin<Portf, 1> Pf1;
	typedef TPin<Portf, 2> Pf2;
	typedef TPin<Portf, 3> Pf3;
	typedef TPin<Portf, 4> Pf4;
	typedef TPin<Portf, 5> Pf5;
	typedef TPin<Portf, 6> Pf6;
	typedef TPin<Portf, 7> Pf7;
	#endif

	#ifdef PORTG
	typedef TPin<Portg, 0> Pg0;
	typedef TPin<Portg, 1> Pg1;
	typedef TPin<Portg, 2> Pg2;
	typedef TPin<Portg, 3> Pg3;
	typedef TPin<Portg, 4> Pg4;
	typedef TPin<Portg, 5> Pg5;
	typedef TPin<Portg, 6> Pg6;
	typedef TPin<Portg, 7> Pg7;
	#endif

	#ifdef PORTH
	typedef TPin<Porth, 0> Ph0;
	typedef TPin<Porth, 1> Ph1;
	typedef TPin<Porth, 2> Ph2;
	typedef TPin<Porth, 3> Ph3;
	typedef TPin<Porth, 4> Ph4;
	typedef TPin<Porth, 5> Ph5;
	typedef TPin<Porth, 6> Ph6;
	typedef TPin<Porth, 7> Ph7;
	#endif

	#ifdef PORTJ
	typedef TPin<Portj, 0> Pj0;
	typedef TPin<Portj, 1> Pj1;
	typedef TPin<Portj, 2> Pj2;
	typedef TPin<Portj, 3> Pj3;
	typedef TPin<Portj, 4> Pj4;
	typedef TPin<Portj, 5> Pj5;
	typedef TPin<Portj, 6> Pj6;
	typedef TPin<Portj, 7> Pj7;
	#endif

	#ifdef PORTK
	typedef TPin<Portk, 0> Pk0;
	typedef TPin<Portk, 1> Pk1;
	typedef TPin<Portk, 2> Pk2;
	typedef TPin<Portk, 3> Pk3;
	typedef TPin<Portk, 4> Pk4;
	typedef TPin<Portk, 5> Pk5;
	typedef TPin<Portk, 6> Pk6;
	typedef TPin<Portk, 7> Pk7;
	#endif

	#ifdef PORTQ
	typedef TPin<Portq, 0> Pq0;
	typedef TPin<Portq, 1> Pq1;
	typedef TPin<Portq, 2> Pq2;
	typedef TPin<Portq, 3> Pq3;
	typedef TPin<Portq, 4> Pq4;
	typedef TPin<Portq, 5> Pq5;
	typedef TPin<Portq, 6> Pq6;
	typedef TPin<Portq, 7> Pq7;
	#endif

	#ifdef PORTR
	typedef TPin<Portr, 0> Pr0;
	typedef TPin<Portr, 1> Pr1;
	typedef TPin<Portr, 2> Pr2;
	typedef TPin<Portr, 3> Pr3;
	typedef TPin<Portr, 4> Pr4;
	typedef TPin<Portr, 5> Pr5;
	typedef TPin<Portr, 6> Pr6;
	typedef TPin<Portr, 7> Pr7;
	#endif
}
