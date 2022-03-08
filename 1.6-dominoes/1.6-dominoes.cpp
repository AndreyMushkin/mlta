#include <iostream>
#include <fstream>
#include <list>
#include <string>

using namespace std;

typedef pair<short, short> Domino;

list<Domino> ReadDominoes(istream& in)
{
	short n;
	Domino d;
	list<Domino> dl;

	in >> n;

	for (short i = 0; i < n; ++i)
	{
		in >> get<0>(d) >> get<1>(d);
		dl.push_back(d);
	}

	return dl;
}

void AppendDominoToStr(string& str, Domino d)
{
	str.append(to_string(get<0>(d))).append(to_string(get<1>(d)));
}

list<Domino> ExcludeDominoAtPos(list<Domino> dl, size_t pos)
{
	auto it = dl.begin();
	advance(it, pos);
	dl.erase(it);

	return dl;
}

Domino SwapDomino(Domino d)
{
	return pair(get<1>(d), get<0>(d));
}

string AddNextDomino(string str, Domino d, list<Domino> dl)
{
	AppendDominoToStr(str, d);

	string nextStr, maxStr = str;
	short nextDomino = get<1>(d);
	size_t i = 0;
	for (Domino d : dl)
	{
		if (nextDomino == get<0>(d))
		{
			if ((nextStr = AddNextDomino(str, d, ExcludeDominoAtPos(dl, i))).compare(maxStr) > 0 || 
				nextStr.length() > maxStr.length())
			{
				maxStr = nextStr;
			}
		}

		if (nextDomino == get<1>(d))
		{
			if ((nextStr = AddNextDomino(str, SwapDomino(d), ExcludeDominoAtPos(dl, i))).compare(maxStr) > 0 || 
				nextStr.length() > maxStr.length())
			{
				maxStr = nextStr;
			}
		}

		++i;
	}

	return maxStr;
}

string Start(list<Domino> dl)
{
	string resStr, curStr;
	size_t i = 0;
	for (Domino d : dl)
	{
		curStr = AddNextDomino("", d, ExcludeDominoAtPos(dl, i));
		if ((curStr.length() > resStr.length()) || ((curStr.length() == resStr.length()) && curStr.compare(resStr) > 0))
		{
			resStr = curStr;
			cout << "changed\n";
		}

		cout << curStr << '\n';

		curStr = AddNextDomino("", SwapDomino(d), ExcludeDominoAtPos(dl, i));
		if ((curStr.length() > resStr.length()) || (curStr.length() == resStr.length() && curStr.compare(resStr) > 0))
		{
			resStr = curStr;
			cout << "changed\n";
		}

		cout << curStr << '\n';

		++i;
	}

	return resStr;
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	list<Domino> dominoes = ReadDominoes(in);

	out << Start(dominoes) << '\n';

	return 0;
}
