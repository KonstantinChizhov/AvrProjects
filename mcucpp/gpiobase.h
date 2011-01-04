
#pragma once

#ifndef IOPORTBASE_HPP
#define IOPORTBASE_HPP

namespace IO
{
	class GpioBase
	{
		GpioBase();
	public:
		enum GenericConfiguration
		{
			AnalogIn 		= 0,
			In 				= 1,
			PullUpOrDownIn 	= 2,

			Out 			= 4,
			OpenDrainOut 	= 5,
			AltOut 			= 6,
			AltOpenDrain 	= 7
		};

		// define 'Configuration' to this type to indicate that derived port class does not need any configuration
		enum DontCareConfiguration{None};
	};
}
#endif
