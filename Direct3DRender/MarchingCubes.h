#pragma once

#include <functional>

namespace MarchingCubes
{
	typedef struct 
	{
		double x;
		double y;
		double z;
	} XYZ;

	typedef struct 
	{
		XYZ p[8];
		double val[8];
	} GRIDCELL;

	void Polygonise(GRIDCELL grid, double isolevel, std::function<void (double x, double y, double z)> vertex) ;

};

