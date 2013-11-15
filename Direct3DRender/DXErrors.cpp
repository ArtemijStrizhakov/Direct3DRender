#include "stdafx.h"
#include "DXErrors.h"
#include <DxErr.h>

namespace DXErrors
{
	void DXErrors::ReportError(HRESULT hr)
	{
		if (FAILED(hr)) 
		{
			MessageBox(nullptr,  DXGetErrorDescription(hr), DXGetErrorString(hr), MB_ICONHAND);
		}
	}
}