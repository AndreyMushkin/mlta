#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

struct Domino
{
	pair<short, short> val;
	bool excluded;
};

vector<Domino> ReadDominoes(istream& in)
{
	short n, val1, val2;
	vector<Domino> dl;

	in >> n;

	for (short i = 0; i < n; ++i)
	{
		in >> val1 >> val2;
		dl.push_back({ pair<short, short>(val1, val2), false });
	}

	return dl;
}

void AppendDominoToStr(string& str, Domino d)
{
	str.append(to_string(get<0>(d.val))).append(to_string(get<1>(d.val)));
}

void ExcludeDominoAtPos(vector<Domino>& dl, size_t pos, bool exclude)
{
	dl[pos].excluded = exclude;
}

Domino SwapDomino(Domino d)
{
	return { pair(get<1>(d.val), get<0>(d.val)), d.excluded };
}

string AddNextDomino(string str, Domino d, vector<Domino>& dl)
{
	AppendDominoToStr(str, d);

	string nextStr, maxStr = str;
	short nextDomino = get<1>(d.val);
	size_t i = 0;
	for (Domino d : dl)
	{
		if (d.excluded) 
		{
			++i;
			continue;
		}

		if (nextDomino == get<0>(d.val))
		{
			ExcludeDominoAtPos(dl, i, true);
			if ((nextStr = AddNextDomino(str, d, dl)).compare(maxStr) > 0 || 
				nextStr.length() > maxStr.length())
			{
				maxStr = nextStr;
			}
			ExcludeDominoAtPos(dl, i, false);
		}

		if (nextDomino == get<1>(d.val))
		{
			ExcludeDominoAtPos(dl, i, true);
			if ((nextStr = AddNextDomino(str, SwapDomino(d), dl)).compare(maxStr) > 0 || 
				nextStr.length() > maxStr.length())
			{
				maxStr = nextStr;
			}
			ExcludeDominoAtPos(dl, i, false);
		}

		++i;
	}

	return maxStr;
}

string Start(vector<Domino>& dl)
{
	string resStr, curStr;
	size_t i = 0;
	for (Domino d : dl)
	{
		ExcludeDominoAtPos(dl, i, true);
		curStr = AddNextDomino("", d, dl);
		if ((curStr.length() > resStr.length()) || ((curStr.length() == resStr.length()) && curStr.compare(resStr) > 0))
		{
			resStr = curStr;
		}

		curStr = AddNextDomino("", SwapDomino(d), dl);
		if ((curStr.length() > resStr.length()) || (curStr.length() == resStr.length() && curStr.compare(resStr) > 0))
		{
			resStr = curStr;
		}
		ExcludeDominoAtPos(dl, i, false);

		++i;
	}

	return resStr;
}

int main()
{
	auto start = std::chrono::steady_clock::now();

	ifstream in("input.txt");
	ofstream out("output.txt");

	if (!in.is_open() || !out.is_open())
	{
		cout << "Failed to open files\n";
		return 1;
	}

	vector<Domino> dominoes = ReadDominoes(in);

	out << Start(dominoes) << '\n';

	auto end = std::chrono::steady_clock::now();

	cout << "Running time " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";
	return 0;
}
