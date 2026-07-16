#include<iostream>
using namespace std;

class Cube
{
	friend ostream& operator <<(ostream& cout, Cube& cube);
public:
	static const int MAX = 100;
	Cube(float L, float W, float H) : L(L), W(W), H(H)
	{
	}
	virtual float CalS() = 0;

	virtual ~Cube()
	{
		cout << "CubeѾ" << endl;
	}
	void SetL() const
	{
		/*this->L = 100;
		this = NULL;*/
	}
protected:

	float L;
	float W;
	float H;
};

float Cube::CalS()
{
	cout << " Cube CalS()" << endl;
	return this->L * this->H * this->W;
}
class Rectangle : public Cube
{
public:

	Rectangle(float L, float W, float H) : Cube(L, W, H)
	{
	}
	float CalS() override
	{
		int res = this->L * this->H * this->W;
		cout << "Rectangle : S == " << res << endl;
		return res;
	}
	~Rectangle()
	{
		cout << "RectangleѾ" << endl;
	}
};

class Triangle : public Cube
{
public:
	Triangle(float L, float W, float H) : Cube(L, W, H)
	{
	}
	virtual float CalS() override
	{
		int res = this->H * this->W / 2;
		cout << "Triangle : S == " << res << endl;
		return res;
	}
	~Triangle()
	{
		cout << "TriangleѾ" << endl;
	}
};
float CalS(Cube& cube)
{
	return cube.CalS();
}
ostream& operator <<(ostream& cout, Cube& cube)
{
	cout << " Cube = " << cube.CalS() << endl;
	return cout;
}

class Test1
{
protected:
	float a;
	float b;
	float c;
};
class Test2 : Test1
{
};
void main3544458594()
{
	Triangle c1 = Triangle(0, 6, 4);

	Triangle c2(0, 10, 12);
	Rectangle c3 = { 6, 2, 4 };

	/*int res = c1.CalS();
	res = c2.CalS();
	res = c3.CalS();*/

	Cube& cube = c1;
	//Cube* cube = &c1;

	int res = CalS(c1);
	res = CalS(c2);
	res = CalS(c3);
	res = CalS(cube);

	cout << sizeof(c1) << endl;
	cout << c1 << endl;

	Test2 t1;
	cout << sizeof(t1);
}
#include<iostream>
#include<string>
#include<vector>

using namespace std;

int main5124312412()
{
	string s;
	cin >> s;
	getline(cin, s);
	vector<int> str(26);
	for (char c : s)
	{
		int idx = c - 'A';
		str[idx]++;
	}
	for (int i = 0; i < 26; ++i)
	{
		char c = (i + 'A');
		cout << c << ":" << str[i] << endl;
	}
	return 0;
}