/*
10.14. Матрица (6)

В матрице A размером N×N (3 ≤ N ≤ 1000) находятся без повторения числа от 1 до N^2.
Нужно определить, может ли эта матрица получиться из отсортированной матрицы, в которой
последовательно по строкам и слева направо в каждой строке записаны в порядке возрастания числа
1, 2, …, N^2, путем выполнения следующих двух операций:
обмен любых двух строк;
обмен любых двух столбцов.
Если это возможно, найти минимальное количество обменов, после которых заданная матрица окажется
отсортированной.
Ввод из файла INPUT.TXT. В первой строке содержатся число N. В следующих N строках находятся
элементы матрицы A по N чисел в строке через пробел.
Вывод в файл OUTPUT.TXT. В первой строке нужно вывести Yes, если матрицу можно привести к
отсортированной, или No, если это невозможно. Если данное преобразование возможно, то во второй
строке нужно вывести минимальное число обменов, необходимое для преобразования матрицы.

Visual Studio 2019 (v142)
Microsoft C++ Compiler (MSVC)
ISO C++17 Standard (/std:c++17)
Windows 10 (10.0.19041) x64

Мушкин Андрей ПС-21
*/

#include <iostream>
#include <fstream>
#include <vector>

int SelectSort(std::vector<int>& vect)
{
	int swapsCount = 0, len = vect.size(), minIndex;

	for (size_t i = 0; i < len - 1; ++i)
	{
		minIndex = i;
		for (size_t j = i + 1; j < len; ++j)
		{
			if (vect[j] < vect[minIndex])
			{
				minIndex = j;
			}
		}
		if (minIndex != i)
		{
			std::swap(vect[i], vect[minIndex]);
			++swapsCount;
		}
	}

	return swapsCount;
}

int main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	int N;
	in >> N >> N;

	std::vector<std::vector<int>> matrix;
	int val;

	// colRemainders[X] = Y - значит, что все элементы в X-ом столбце должны давать остаток от деления на N равный Y
	// если остаток от деления на N равен 0, то в colRemainders помещается N для корректной сортировки
	// rowIndexes[X] = Y - значит, что X-ая строка данной матрицы должна быть Y-ой строкой в отсортированной
	std::vector<int> colRemainders, rowIndexes;
	rowIndexes.push_back(0);
	for (int col = 0; col < N; ++col)
	{
		in >> val;
		if (!(val % N))
		{
			colRemainders.push_back(N);
			if (val / N > N)
			{
				out << "No" << std::endl;
				return 0;
			}
			else
			{
				if (col == 0)
				{
					rowIndexes[0] = val / N - 1;
				}
				else if (rowIndexes[0] != val / N - 1)
				{
					out << "No" << std::endl;
					return 0;
				}
			}
		}
		else
		{
			if (val / N >= N)
			{
				out << "No" << std::endl;
				return 0;
			}
			else
			{
				if (col == 0)
				{
					rowIndexes[0] = val / N;
				}
				else if (rowIndexes[0] != val / N)
				{
					out << "No" << std::endl;
					return 0;
				}
			}
			colRemainders.push_back(val % N);
		}
	}

	for (int row = 1; row < N; ++row)
	{
		rowIndexes.push_back(0);
		for (int col = 0; col < N; ++col)
		{
			in >> val;
			if (!(val % N))
			{
				if ((colRemainders[col] != N) || (val / N > N))
				{
					out << "No" << std::endl;
					return 0;
				}
				else
				{
					if (col == 0)
					{
						rowIndexes[row] = val / N - 1;
					}
					else if (rowIndexes[row] != val / N - 1)
					{
						out << "No" << std::endl;
						return 0;
					}
				}
			}
			else
			{
				if ((colRemainders[col] != val % N) || (val / N >= N))
				{
					out << "No" << std::endl;
					return 0;
				}
				else
				{
					if (col == 0)
					{
						rowIndexes[row] = val / N;
					}
					else if (rowIndexes[row] != val / N)
					{
						out << "No" << std::endl;
						return 0;
					}
				}
			}
		}
	}

	int swapsCount = SelectSort(colRemainders) + SelectSort(rowIndexes);

	out << "Yes" << std::endl
		<< swapsCount << std::endl;

	return 0;
}
