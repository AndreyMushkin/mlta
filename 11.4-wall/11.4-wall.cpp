/*
11.4. Стена (6)

Жил-был жадный Король. Он приказал своему главному Архитектору построить стену вокруг его замка.
Король был таким жадным, что не послушал предложение Архитектора построить красивую кирпичную стену
совершенной формы с изящными высокими башнями. Вместо этого он приказал построить стену вокруг всего
замка, используя минимальное количество камня, но потребовал, чтобы стена не подходила к замку ближе
некоторого расстояния. Если Король узнает, что Архитектор использовал больше ресурсов для постройки
стены, чем было абсолютно необходимо для удовлетворения требований, Архитектор лишится головы.
Более того, Архитектор должен представить проект стены, где указано точное количество ресурсов.

Ваша задача - помочь Архитектору сохранить голову, написав программу, определяющую минимальную длину
стены, которую можно построить вокруг замка, удовлетворив требования Короля.
Задача слегка упрощается тем, что замок Короля представляет собой многоугольник и расположен на
плоской поверхности. Архитектор уже сопоставил замку прямоугольную декартову систему координат и точно
определил координаты каждого угла замка в футах.

Ввод из файла INPUT.TXT. Первая строка содержит два целых числа N и L, разделённых пробелом:
N - число углов в замке Короля, а L - минимальное число футов, на которое Король разрешил приблизить
стену к замку.

Следующие N строк описывают координаты углов замка в порядке обхода по часовой стрелке. Каждая строка
содержит два целых числа xi и yi, разделённых пробелом и представляющих собой координаты i-го угла
в футах. Все углы имеют различные координаты, и стены замка не пересекаются иначе как в углах.

Ограничения: 3 ≤ N ≤ 1000, 1 ≤ L ≤ 1000, -10000 ≤ xi, yi ≤ 10000.

Вывод в файл OUTPUT.TXT. Выводится единственное число - минимальная длина стены в футах,
которая может быть построена вокруг замка согласно требованиям Короля с точностью до фута.

Visual Studio 2019 (v142)
Microsoft C++ Compiler (MSVC)
ISO C++17 Standard (/std:c++17)
Windows 10 (10.0.19041) x64

Мушкин Андрей ПС-21
*/

#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

class Vector2d
{
public:
	Vector2d(double x = 0, double y = 0)
		: x(x)
		, y(y)
	{
	}

	double x;
	double y;

	double length() const { return hypot(x, y); }

	double const operator*(const Vector2d& v) const { return x * v.x + y * v.y; }

	Vector2d const operator-(const Vector2d& v) const { return Vector2d(x - v.x, y - v.y); }

	double angleCos(const Vector2d& v) const
	{
		if ((v.x == 0 && v.y == 0) || (x == 0 && y == 0))
		{
			return -1;
		}

		return (*this * v) / (length() * v.length());
	}
};

std::vector<Vector2d> ReadVectors(std::istream& input, unsigned count, size_t& rightBottomVectIndex)
{
	std::vector<Vector2d> vectors;
	short x, y, maxX, minY;

	input >> maxX >> minY;
	rightBottomVectIndex = 0;

	vectors.push_back(Vector2d(maxX, minY));

	for (size_t index = 1; index < count; ++index)
	{
		input >> x >> y;

		if (y < minY)
		{
			rightBottomVectIndex = index;
			minY = y;
			maxX = x;
		}
		else if (y == minY && x > maxX)
		{
			rightBottomVectIndex = index;
			maxX = x;
		}

		vectors.push_back(Vector2d(x, y));
	}

	return vectors;
}

int main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	if (!in.is_open() || !out.is_open())
	{
		std::cout << "Failed to open file(s)." << std::endl;
		return 1;
	}

	unsigned N, L;

	in >> N >> L;

	size_t startIndex;
	std::vector<Vector2d> vectors = ReadVectors(in, N, startIndex);

	size_t nextIndex = std::string::npos;
	Vector2d curVector = vectors[startIndex];
	Vector2d curProjectileVector(1, 0);
	Vector2d firstWallVector;
	double maxAngleCos, angleCos, wallLength = 0;
	bool firstIteration = true;

	while (nextIndex != startIndex)
	{
		maxAngleCos = (vectors[0] - curVector).angleCos(curProjectileVector);
		nextIndex = 0;

		for (size_t index = 1; index < vectors.size(); ++index)
		{
			if ((angleCos = (vectors[index] - curVector).angleCos(curProjectileVector)) >= maxAngleCos)
			{
				maxAngleCos = angleCos;
				nextIndex = index;
			}
		}

		if (!firstIteration)
		{
			wallLength += acos(curProjectileVector.angleCos(vectors[nextIndex] - curVector)) * L;
		}
		else
		{
			firstWallVector = vectors[nextIndex] - curVector;
			firstIteration = false;
		}

		curProjectileVector = vectors[nextIndex] - curVector;
		wallLength += curProjectileVector.length();
		curVector = vectors[nextIndex];
	}

	wallLength += acos(curProjectileVector.angleCos(firstWallVector)) * L;

	out << std::setprecision(0) << std::fixed << wallLength << std::endl;

	return 0;
}
