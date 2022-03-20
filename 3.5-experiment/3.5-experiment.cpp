/*
3.5. Эксперимент (6)
Результат эксперимента представляет собой матрицу из N × M (1 ≤ N, M ≤ 2000), 
заполненную целыми числами, по модулю не превосходящими 500.
Отвечающим условиям эксперимента считаются такие подматрицы размера K строк и L столбцов (0 < K < N, 0 < L < M), 
что сумма элементов в каждой из них в точности равна заданному числу S.
Определить, сколько подматриц в исходной матрице отвечают условиям эксперимента.
Ввод.
В первой строке файла INPUT.TXT находятся 5 чисел N, M, K, L и S, разделенных пробелами.
В каждой из следующих N строк находятся через пробел по M чисел, являющихся элементами матрицы.
Вывод.
В первой строке файла OUTPUT.TXT вывести единственное число – количество подматриц размера K×L, 
сумма элементов в которых равна S.

Visual Studio 2019 (v142)
Microsoft C++ Compiler (MSVC)
Windows 10 (10.0.19041) x64

Мушкин Андрей ПС-21
*/

#include <iostream>
#include <fstream>
#include <vector>

typedef std::vector<std::vector<short>> Matrix;

short g_N, g_M, g_K, g_L;
int g_S;

Matrix ReadMatrix(std::istream& in)
{
	Matrix m;
	short val;
	for (unsigned short i = 0; i < g_N; ++i)
	{
		m.push_back({});
		for (unsigned short j = 0; j < g_M; ++j)
		{
			in >> val;
			m[i].push_back(val);
		}
	}

	return m;
}

int VerticalStep(
	const Matrix& mat,
	const short i, const short j,
	const int prevFirstRowSum, const int prevSum
)
{
	if (i + g_K > g_N)
	{
		return 0;
	}

	int curLastRowSum = 0, curFirstRowSum = 0;
	short curLastRowIndex = i + g_K - 1;
	for (unsigned short _j = j; _j < j + g_L; ++_j)
	{
		curLastRowSum += mat[curLastRowIndex][_j];
		curFirstRowSum += mat[i][_j];
	}

	int rowSumDiff = curLastRowSum - prevFirstRowSum;
	bool success = rowSumDiff == (g_S - prevSum);

	return (success ? 1 : 0) + VerticalStep(mat, i + 1, j, curFirstRowSum, prevSum + rowSumDiff);
}

int HorizontalStep(
	const Matrix& mat,
	const short j,
	const int prevFirstColSum, const int prevSum
)
{
	if (j + g_L > g_M)
	{
		return 0;
	}

	int curLastColSum = 0, curFirstColSum = 0, curFirstRowSum = 0;
	short curLastColIndex = j + g_L - 1;
	for (unsigned short i = 0; i < g_K; ++i)
	{
		curLastColSum += mat[i][curLastColIndex];
		curFirstColSum += mat[i][j];
	}

	for (unsigned short _j = j; _j <= curLastColIndex; ++_j)
	{
		curFirstRowSum += mat[0][_j];
	}

	int colSumDiff = curLastColSum - prevFirstColSum;
	bool success = colSumDiff == (g_S - prevSum);

	return 
		(success ? 1 : 0) + 
		VerticalStep(mat, 1, j, curFirstRowSum, prevSum + colSumDiff) + 
		HorizontalStep(mat, j + 1, curFirstColSum, prevSum + colSumDiff);
}

int InitialStep(const Matrix& mat)
{
	int initSum = 0, initFirstColSum = 0, initFirstRowSum = 0;

	for (unsigned short i = 0; i < g_K; ++i)
	{
		for (unsigned short j = 0; j < g_L; ++j)
		{
			initSum += mat[i][j];
			if (i == 0)
				initFirstRowSum += mat[i][j];
			if (j == 0)
				initFirstColSum += mat[i][j];
		}
	}

	int successCount = (initSum == g_S) ? 1 : 0;
	successCount += VerticalStep(mat, 1, 0, initFirstRowSum, initSum);
	successCount += HorizontalStep(mat, 1, initFirstColSum, initSum);

	return successCount;
}

int main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	if (!in.is_open() || !out.is_open())
	{
		std::cout << "Failed to open file(s)" << std::endl;
		return 1;
	}

	in >> g_N >> g_M >> g_K >> g_L >> g_S;

	Matrix mat = ReadMatrix(in);

	int successCount = InitialStep(mat);

	out << successCount << std::endl;

	return 0;
}
