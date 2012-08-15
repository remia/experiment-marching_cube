#pragma once

namespace GLRenderer
{
	class ShaderLocation
	{
		public:
			static const int Position = 0;
			static const int Color = 1;
			static const int Normal = 2;
			static const int TexCoord = 3;

			static const int GlobalMatricesBindingIndex = 1;
	};
}
