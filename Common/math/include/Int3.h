#pragma once
class Int3
{
public:
	union
	{
		struct
		{
			int A;
			int B;
			int C;
		};
		int Values[3];
	};

public:
	Int3();
	Int3(int a, int b, int c);
	~Int3();

	int operator[](int index);
};

