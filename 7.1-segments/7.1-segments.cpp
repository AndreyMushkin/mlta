/*
7.1. Отрезки (7)

На числовой прямой задано N отрезков, пронумерованных в порядке их ввода, начиная с 1.
Отрезки могут пересекаться. Отрезок называется простым, если в нем не содержится целиком никакой другой.
Найдите все простые отрезки из заданного набора.
Ввод из файла INPUT.TXT. В первой строке записано число N (1 ≤ N ≤ 5×10^5).
В следующих N строках заданы целочисленные координаты левого и правого концов отрезка a_i, b_i (-10^9 ≤ a_i, b_i ≤ 10^9).
Вывод. В первую строка файла OUTPUT.TXT вывести количество простых отрезков. Во вторую – в порядке возрастания последовательность номеров простых отрезков.
Если простых отрезков нет, в единственной строке вывести 0.
Ограничения: время 1 с.

Visual Studio 2019 (v142)
Microsoft C++ Compiler (MSVC)
ISO C++20 Standard (/std:c++20)
Windows 10 (10.0.19041) x64

Мушкин Андрей ПС-21
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <map>
//#include <chrono>

enum class EdgeType
{
	Begin,
	End
};

enum class SegmentResult
{
	Complex,
	Simple,
	Collised
};

struct ScanEdge
{
	size_t id;
	int pos;
	EdgeType type;
};

int main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");
	
	std::vector<ScanEdge> scanVector;
	std::vector<int> segmentsLength = { 0 };
	std::map<int, bool> simpleSegments;
	std::map<int, SegmentResult> segmentsResult;

	//auto start = std::chrono::steady_clock::now();

	int N, beg, end;
	in >> N;
	for (size_t i = 1; i <= N; ++i)
	{
		in >> beg >> end;
		scanVector.push_back({ i, beg, EdgeType::Begin });
		scanVector.push_back({ i, end, EdgeType::End });
		segmentsLength.push_back(end - beg);
	}

	//auto afterReading = std::chrono::steady_clock::now();
	//std::chrono::duration<double> elapsed_seconds = afterReading - start;
	//std::cout << "Reading time: " << elapsed_seconds.count() << std::endl;

	std::sort(scanVector.begin(), scanVector.end(), [&](ScanEdge& edge1, ScanEdge& edge2) {
		if (edge1.pos < edge2.pos)
		{
			return true;
		}
		else if (edge1.pos == edge2.pos)
		{
			if (edge1.type == EdgeType::Begin && edge2.type == EdgeType::Begin)
			{
				if (segmentsLength[edge1.id] > segmentsLength[edge2.id])
				{
					return true;
				}
				else if (segmentsLength[edge1.id] == segmentsLength[edge2.id])
				{
					segmentsResult[edge1.id] = SegmentResult::Collised;
					segmentsResult[edge2.id] = SegmentResult::Collised;
					return edge1.id < edge2.id;
				}
				else
				{
					return false;
				}
			}
			else if (edge1.type == EdgeType::End && edge2.type == EdgeType::Begin)
			{
				return false;
			}
			else if (edge1.type == EdgeType::Begin && edge2.type == EdgeType::End)
			{
				return true;
			}
			else
			{
				if (segmentsLength[edge1.id] < segmentsLength[edge2.id])
				{
					return true;
				}
				else if (segmentsLength[edge1.id] == segmentsLength[edge2.id])
				{
					segmentsResult[edge1.id] = SegmentResult::Collised;
					segmentsResult[edge2.id] = SegmentResult::Collised;
					return edge1.id > edge2.id;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	});

	std::queue<int> indexQueue;
	int lastPoppedValue;

	for (const ScanEdge& se : scanVector)
	{
		if (se.type == EdgeType::Begin)
		{
			indexQueue.push(se.id);
		}
		else
		{
			lastPoppedValue = 0;
			if (segmentsResult.contains(se.id) && segmentsResult[se.id] == SegmentResult::Complex)
			{
				continue;
			}

			do
			{
				segmentsResult[lastPoppedValue] = SegmentResult::Complex;
				lastPoppedValue = indexQueue.front();
				indexQueue.pop();
			} while (lastPoppedValue != se.id);

			if (!segmentsResult.contains(lastPoppedValue))
			{
				simpleSegments[lastPoppedValue] = true;
			}
		}
	}

	out << simpleSegments.size() << std::endl;
	for (const auto& [id, _] : simpleSegments)
	{
		out << id << std::endl;
	}

	//auto finish = std::chrono::steady_clock::now();
	//std::chrono::duration<double> algorithmElapsed = finish - afterReading;
	//std::cout << "Algorithm running time: " << algorithmElapsed.count() << std::endl;

	//std::chrono::duration<double> totalElapsed = finish - start;
	//std::cout << "Total time: " << totalElapsed.count() << std::endl;

	return 0;
}
