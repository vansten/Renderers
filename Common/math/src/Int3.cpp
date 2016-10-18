#include "../include/Int3.h"

Int3::Int3() : A(0), B(0), C(0)
{

}

Int3::Int3(int a, int b, int c) : A(a), B(b), C(c)
{}

Int3::~Int3()
{

}

int Int3::operator[](int index)
{
	if(index >= 0 && index < 3)
	{
		return Values[index];
	}

	return 0;
}
