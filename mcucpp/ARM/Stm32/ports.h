#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef STM32_PORTS_H
#define STM32_PORTS_H

#include <static_assert.h>
#define USE_SPLIT_PORT_CONFIGURATION 8

namespace IO
{	
	class NativePortBase :public GpioBase
	{
	public:
		enum{Width=16};
		typedef NativePortBase Base;
		typedef uint16_t DataT;
		enum Configuration
		{
			AnalogIn = 0,
			In = 0x04,
			PullUpOrDownIn = 0x08,
			Out = 0x03,
			OpenDrainOut = 0x07,
			AltOut = 0x0B,
			AltOpenDrain = 0x0f
		};
		
		static inline unsigned UnpackConfig(unsigned mask, unsigned value, Configuration configuration)
		{
			mask = (mask & 0xf0) << 12 | (mask & 0x0f);
			mask = (mask & 0x000C000C) << 6 | (mask & 0x00030003);
			mask = (mask & 0x02020202) << 3 | (mask & 0x01010101);
			return (value & ~(mask*0x0f)) | mask * configuration;
		}
		
		static Configuration MapConfiguration(GenericConfiguration config)
		{
			switch(config)
			{
			case GpioBase::In: return In;
			case GpioBase::AnalogIn: return AnalogIn;
			case GpioBase::PullUpOrDownIn: return PullUpOrDownIn;
			case GpioBase::OpenDrainOut: return OpenDrainOut;
			case GpioBase::AltOut: return AltOut;
			case GpioBase::AltOpenDrain: return AltOpenDrain;
			//case GpioBase::Out: 
			default:
			  return Out;
			}
		}
	};

	namespace Private
	{
		template<unsigned mask>
		class ConfigurationMask
		{
			static const unsigned mask1 = (mask & 0xf0) << 12 | (mask & 0x0f);
			static const unsigned mask2 = (mask1 & 0x000C000C) << 6 | (mask1 & 0x00030003);
			static const unsigned mask3 = (mask2 & 0x02020202) << 3 | (mask2 & 0x01010101);
		public:
			static const unsigned value = mask3;
		};
		
		template<class ConfigReg, unsigned mask, NativePortBase::Configuration config>
		struct WriteConfig
		{
			static void Write()
			{
			  const unsigned configMask = ConfigurationMask<mask>::value;
			  unsigned result = (ConfigReg::Get() & ~(configMask*0x0f)) | configMask * config;
			  ConfigReg::Set(result);
			}
		};
		  
		template<class CRL, class CRH, class IDR, class ODR, class BSRR, class BRR, class LCKR, int ID>
		class PortImplementation :public NativePortBase
		{
		public:
			static DataT Read()
			{
			  return ODR::Get();
			}
			static void Write(DataT value)
			{
				ODR::Set(value);
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				BSRR::Set(value | (uint32_t)clearMask << 16);
			}
			static void Set(DataT value)
			{
				BSRR::Set(value);
			}
			static void Clear(DataT value)
			{
				BSRR::Set((uint32_t)value << 16);
			}
			static void Toggle(DataT value)
			{
				ODR::Xor(value);
			}
			static DataT PinRead()
			{
				return IDR::Get();
			}
			
			// constant interface

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				BSRR::Set(value | (uint32_t)clearMask << 16);
			}

			template<DataT value>
			static void Toggle()
			{
				ODR::Xor(value);
			}

			template<DataT value>
			static void Set()
			{
				BSRR::Set(value);
			}

			template<DataT value>
			static void Clear()
			{
				BSRR::Set((uint32_t)value << 16);
			}
			
			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(pin < 8)
				{
					CRL::AndOr(~(0x0fu << pin*4), (unsigned)configuration << pin*4);
				}
				else
				{
					CRH::AndOr(~(0x0fu << (pin-8)*4), (unsigned)configuration << (pin-8)*4);
				}
			}
			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				CRL::Set(UnpackConfig(mask, CRL::Get(), configuration));
				CRH::Set(UnpackConfig(mask>>8, CRH::Get(), configuration));
			}
			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
			  WriteConfig<CRL, mask, configuration>::Write();
			  WriteConfig<CRH, mask>>8, configuration>::Write();
			}
			enum{Id = ID};
		};
		
		/*Lower part of port. Need for effective configuration writing*/
		template<class CRL, class CRH, class IDR, class ODR, class BSRR, class BRR, class LCKR, int ID>
		class PortImplementationL :public PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ID>
		{
			typedef PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ID> Base;
			public:
				typedef typename Base::Configuration Configuration;
				typedef typename Base::DataT DataT;
				
				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					BOOST_STATIC_ASSERT(pin < 8);
					CRL::AndOr(~(0x0fu << pin*4), (uint32_t)configuration << pin*4);
				}
				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					CRL::Set(NativePortBase::UnpackConfig(mask, CRL::Get(), configuration));
				}
				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					 WriteConfig<CRL, mask, configuration>::Write();
				}
		};
		
		 /*Hight part of port. Need for effective configuration writing*/
		template<class CRL, class CRH, class IDR, class ODR, class BSRR, class BRR, class LCKR, int ID>
		class PortImplementationH :public PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ID>
		{
			typedef PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ID> Base;
			public:
				typedef typename Base::Configuration Configuration;
				typedef typename Base::DataT DataT;
				
			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin >= 8 && pin < 16);
				CRH::AndOr(~(0x0fu << (pin-8)*4), (uint32_t)configuration << (pin-8)*4);
			}
			static void SetConfiguration(DataT mask, Configuration configuration)
			{
			  CRH::Set(NativePortBase::UnpackConfig(mask>>8, CRH::Get(), configuration));
			}
			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				WriteConfig<CRH, mask>>8, configuration>::Write();
			}
		};
	}

#define MAKE_PORT(CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, className, ID) \
   namespace Private{\
		IO_REG_WRAPPER(CRL, CRL ## _t, uint32_t);\
		IO_REG_WRAPPER(CRH, CRH ## _t, uint32_t);\
		IO_REG_WRAPPER(IDR, IDR ## _t, uint16_t);\
		IO_REG_WRAPPER(ODR, ODR ## _t, uint16_t);\
		IO_REG_WRAPPER(BSRR, BSRR ## _t, uint32_t);\
		IO_REG_WRAPPER(BRR, BRR ## _t, uint16_t);\
		IO_REG_WRAPPER(LCKR, LCKR ## _t, uint16_t);\
	}\
	  typedef Private::PortImplementation<\
			Private::CRL ## _t, \
			Private::CRH ## _t, \
			Private::IDR ## _t, \
			Private::ODR ## _t, \
			Private::BSRR ## _t, \
			Private::BRR ## _t, \
			Private::LCKR ## _t, \
			ID> className; \
		typedef Private::PortImplementationL<\
			Private::CRL ## _t, \
			Private::CRH ## _t, \
			Private::IDR ## _t, \
			Private::ODR ## _t, \
			Private::BSRR ## _t, \
			Private::BRR ## _t, \
			Private::LCKR ## _t, \
			ID> className ## L; \
		typedef Private::PortImplementationH<\
			Private::CRL ## _t, \
			Private::CRH ## _t, \
			Private::IDR ## _t, \
			Private::ODR ## _t, \
			Private::BSRR ## _t, \
			Private::BRR ## _t, \
			Private::LCKR ## _t, \
			ID> className ## H; \
		  
	  
//==================================================================================================
#if defined(__ICCARM__)

#ifdef USE_PORTA
MAKE_PORT(GPIOA_CRL, GPIOA_CRH, GPIOA_IDR, GPIOA_ODR, GPIOA_BSRR, GPIOA_BRR, GPIOA_LCKR, Porta, 'A')
#endif

#ifdef USE_PORTB
MAKE_PORT(GPIOB_CRL, GPIOB_CRH, GPIOB_IDR, GPIOB_ODR, GPIOB_BSRR, GPIOB_BRR, GPIOB_LCKR, Portb, 'B')
#endif

#ifdef USE_PORTC
MAKE_PORT(GPIOC_CRL, GPIOC_CRH, GPIOC_IDR, GPIOC_ODR, GPIOC_BSRR, GPIOC_BRR, GPIOC_LCKR, Portc, 'C')
#endif

#ifdef USE_PORTD
MAKE_PORT(GPIOD_CRL, GPIOD_CRH, GPIOD_IDR, GPIOD_ODR, GPIOD_BSRR, GPIOD_BRR, GPIOD_LCKR, Portd, 'D')
#endif

#ifdef USE_PORTE
MAKE_PORT(GPIOE_CRL, GPIOE_CRH, GPIOE_IDR, GPIOE_ODR, GPIOE_BSRR, GPIOE_BRR, GPIOE_LCKR, Porte, 'E')
#endif

#ifdef USE_PORTF
MAKE_PORT(GPIOF_CRL, GPIOF_CRH, GPIOF_IDR, GPIOF_ODR, GPIOF_BSRR, GPIOF_BRR, GPIOF_LCKR, Portf, 'F')
#endif

#ifdef USE_PORTG
MAKE_PORT(GPIOG_CRL, GPIOG_CRH, GPIOG_IDR, GPIOG_ODR, GPIOG_BSRR, GPIOG_BRR, GPIOG_LCKR, Portg, 'G')
#endif

#ifdef USE_PORTH
MAKE_PORT(GPIOH_CRL, GPIOH_CRH, GPIOH_IDR, GPIOH_ODR, GPIOH_BSRR, GPIOH_BRR, GPIOH_LCKR, Porth, 'H')
#endif

#ifdef USE_PORTJ
MAKE_PORT(GPIOJ_CRL, GPIOJ_CRH, GPIOJ_IDR, GPIOJ_ODR, GPIOJ_BSRR, GPIOJ_BRR, GPIOJ_LCKR, Portj, 'J')
#endif

#ifdef USE_PORTK
MAKE_PORT(GPIOK_CRL, GPIOK_CRH, GPIOK_IDR, GPIOK_ODR, GPIOK_BSRR, GPIOK_BRR, GPIOK_LCKR, Portk, 'K')
#endif
//==================================================================================================
#elif defined(__CC_ARM) || defined (__arm__)

#ifdef USE_PORTA
MAKE_PORT(GPIOA->CRL, GPIOA->CRH, GPIOA->IDR, GPIOA->ODR, GPIOA->BSRR, GPIOA->BRR, GPIOA->LCKR, Porta, 'A')
#endif

#ifdef USE_PORTB
MAKE_PORT(GPIOB->CRL, GPIOB->CRH, GPIOB->IDR, GPIOB->ODR, GPIOB->BSRR, GPIOB->BRR, GPIOB->LCKR, Portb, 'B')
#endif

#ifdef USE_PORTC
MAKE_PORT(GPIOC->CRL, GPIOC->CRH, GPIOC->IDR, GPIOC->ODR, GPIOC->BSRR, GPIOC->BRR, GPIOC->LCKR, Portc, 'C')
#endif

#ifdef USE_PORTD
MAKE_PORT(GPIOD->CRL, GPIOD->CRH, GPIOD->IDR, GPIOD->ODR, GPIOD->BSRR, GPIOD->BRR, GPIOD->LCKR, Portd, 'D')
#endif

#ifdef USE_PORTE
MAKE_PORT(GPIOE->CRL, GPIOE->CRH, GPIOE->IDR, GPIOE->ODR, GPIOE->BSRR, GPIOE->BRR, GPIOE->LCKR, Porte, 'E')
#endif

#ifdef USE_PORTF
MAKE_PORT(GPIOF->CRL, GPIOF->CRH, GPIOF->IDR, GPIOF->ODR, GPIOF->BSRR, GPIOF->BRR, GPIOF->LCKR, Portf, 'F')
#endif

#ifdef USE_PORTG
MAKE_PORT(GPIOG->CRL, GPIOG->CRH, GPIOG->IDR, GPIOG->ODR, GPIOG->BSRR, GPIOG->BRR, GPIOG->LCKR, Portg, 'G')
#endif

#ifdef USE_PORTH
MAKE_PORT(GPIOH->CRL, GPIOH->CRH, GPIOH->IDR, GPIOH->ODR, GPIOH->BSRR, GPIOH->BRR, GPIOH->LCKR, Porth, 'H')
#endif

#ifdef USE_PORTJ
MAKE_PORT(GPIOJ->CRL, GPIOJ->CRH, GPIOJ->IDR, GPIOJ->ODR, GPIOJ->BSRR, GPIOJ->BRR, GPIOJ->LCKR, Portj, 'J')
#endif

#ifdef USE_PORTK
MAKE_PORT(GPIOK->CRL, GPIOK->CRH, GPIOK->IDR, GPIOK->ODR, GPIOK->BSRR, GPIOK->BRR, GPIOK->LCKR, Portk, 'K')
#endif
//==================================================================================================
#else
#error Not supported compiler
#endif

}//namespace IO

#endif
