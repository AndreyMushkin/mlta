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
			return -1;
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
	std::ifstream in("input2.txt");
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
			firstWallVector = curProjectileVector;
			firstIteration = false;
		}

		curProjectileVector = vectors[nextIndex] - curVector;
		wallLength += curProjectileVector.length();
		curVector = vectors[nextIndex];
	}

	wallLength += acos(curProjectileVector.angleCos(firstWallVector)) * L;

	out << std::setprecision(0) << std::fixed << wallLength << std::endl;
}
