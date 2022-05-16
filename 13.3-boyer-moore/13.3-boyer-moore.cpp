/*
13.3. Алгоритм Бойера – Мура(6)
Требуется найти все вхождения образца в текстовом файле методом Бойера – Мура.
В файле нет переноса слов. Образец может включать пробелы и переходить с одной строки файла на другую.
Конец строки файла может интерпретироваться как пробел.Результаты поиска не должны зависеть от
регистра букв, то есть каждая буква в образце и файле может быть как строчной, так и прописной.
Ввод из файла INPUT.TXT.
Первая строка файла является образцом и имеет длину от 1 до 255.
Вторая строка задает имя текстового файла.Вывод в файл OUTPUT.TXT.
Вывести в каждой строке через пробел
последовательность номеров строк и позиций в строке, начиная с которых образец входит в текст.
Нумерация строк и позиций в строке начинается с 1. Если вхождений нет, вывести No.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <deque>

std::map<char, size_t> GetSymbolsOffset(const std::string& needle);

bool CompareFrameWithNeedle(const std::deque<char>& frame, const std::string& needle);

size_t GetOffsetForSymbol(char ch, const std::map<char, size_t>& offsets, size_t needleLength);

void MoveFrame(std::istream& in, std::deque<char>& frame, size_t offset, size_t& curLine, size_t& curLineOffset);

int main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	if (!in.is_open() || !out.is_open())
	{
		std::cout << "Failed to open 'input.txt' or 'output.txt' file." << std::endl;
		return 1;
	}

	std::string needle, haystackFileName;

	std::getline(in, needle);
	std::getline(in, haystackFileName);

	std::ifstream haystack(haystackFileName);

	if (!haystack.is_open())
	{
		std::cout << "Failed to open " << haystackFileName << " file." << std::endl;
		return 1;
	}

	std::map<char, size_t> symbolOffsets = GetSymbolsOffset(needle);
	size_t needleLength = needle.size();

	std::deque<char> frame;
	while (haystack && frame.size() < needleLength)
		frame.push_back(haystack.get());

	if (frame.size() != needleLength)
	{
		out << "No." << std::endl;
		return 0;
	}

	size_t curLine = 1, curLineOffset = 1, offset;

	std::ostringstream outss;

	while (haystack)
	{
		if (CompareFrameWithNeedle(frame, needle))
			outss << curLine << " " << curLineOffset << std::endl;

		offset = GetOffsetForSymbol(frame.back(), symbolOffsets, needleLength);

		MoveFrame(haystack, frame, offset, curLine, curLineOffset);
	}

	if (!outss.str().empty())
	{
		out << outss.str();
	}
	else
	{
		out << "No." << std::endl;
	}

	return 0;
}

std::map<char, size_t> GetSymbolsOffset(const std::string& needle)
{
	std::map<char, size_t> offsets;

	auto it = needle.rbegin();
	offsets[std::tolower(*it++)] = needle.size();

	for (size_t counter = 1; it != needle.rend(); ++it, ++counter)
	{
		unsigned char lowCh = std::tolower(*it);
		if (!offsets.contains(lowCh) || offsets[lowCh] > counter)
		{
			offsets[lowCh] = counter;
		}
	}

	return offsets;
}

constexpr auto cmpPred = [](char a, char b) {
	return std::tolower(a) == std::tolower(b) || ((a == '\n' || a == ' ') && (b == '\n' || b ==' '));
};

bool CompareFrameWithNeedle(const std::deque<char>& frame, const std::string& needle)
{
	return std::equal(frame.rbegin(), frame.rend(), needle.rbegin(), needle.rend(), cmpPred);
}

size_t GetOffsetForSymbol(char symbol, const std::map<char, size_t>& offsets, size_t needleLength)
{
	if (!offsets.contains(symbol))
	{
		return needleLength;
	}

	return offsets.at(symbol);
}

void MoveFrame(std::istream& in, std::deque<char>& frame, size_t offset, size_t& curLine, size_t& curLineOffset)
{
	for (size_t i = 0; i < offset && !in.eof(); ++i)
	{
		if (frame.front() == '\n')
		{
			++curLine;
			curLineOffset = 0;
		}
		++curLineOffset;
		frame.pop_front();
		frame.push_back(in.get());
	}
}