#include "../include/HilbertCurve.h"

namespace raytracer
{
	Vector2 HilbertCurve::Evaluate(int max, int i)
	{
		int rx, ry, s, t = i;
		Vector2 v;
		for(s = 1; s < max; s *= 2)
		{
			rx = 1 & (t / 2);
			ry = 1 & (t ^ rx);
			v = Rotate(s, v, rx, ry);
			v[0] += s * rx;
			v[1] += s * ry;
			t /= 4;
		}

		return v;
	}

	Vector2 HilbertCurve::Rotate(int n, Vector2 prev, int rx, int ry)
	{
		Vector2 v = prev;
		if(ry == 0)
		{
			if(rx == 1)
			{
				v[0] = n - 1 - v[0];
				v[1] = n - 1 - v[1];
			}

			int t = v[0];
			v[0] = v[1];
			v[1] = t;
		}

		return v;
	}

	//rotate/flip a quadrant appropriately
	void rot(int n, int *x, int *y, int rx, int ry)
	{
		if(ry == 0)
		{
			if(rx == 1)
			{
				*x = n - 1 - *x;
				*y = n - 1 - *y;
			}

			//Swap x and y
			int t = *x;
			*x = *y;
			*y = t;
		}
	}
}