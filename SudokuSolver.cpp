#include <fstream>
using namespace std;

ifstream f("sudoku.in");
ofstream g("sudoku.out");

unsigned inStack[101][3], puzzle[10][10], emptyCells;

/* Functia fillStack adauga indicii celulelor necompletate din puzzle-ul Sudoku
   in variabila inStack (aceasta joaca rol de stiva) si contorizeaza numarul de celule necompletate din puzzle
   in variabila emptyCells */
void fillStack()
{
	unsigned i, j;
	for (i = 1; i <= 9; i++)
		for (j = 1; j <= 9; j++)
			if (puzzle[i][j] == 0)
			{
				emptyCells++;
				inStack[emptyCells][1] = i;
				inStack[emptyCells][2] = j;
			}
}

/* Functia validateCell verifica, pe rand, regulile ce dicteaza corectitudinea completarii puzzle-ului Sudoku:
   - daca valoarea atribuita celulei corespunzatoare nivelului k din stiva se repeta pe linia pe care se afla
     celula in matricea puzzle
   - daca valoarea atribuita celulei corespunzatoare nivelului k din stiva se repeta pe coloana pe care se afla
     celula in matricea puzzle
   - determina, dupa ce a fost completata, careul de 3x3 in care se afla celula corespunzatoare nivelului k din stiva
     si verifica daca valoarea celulei se repeta in careul ei */
unsigned validateCell(unsigned k)
{
	unsigned i, j, startRow, startCol;
	for (i = 1; i <= 9; i++)
		if (puzzle[i][inStack[k][2]] == puzzle[inStack[k][1]][inStack[k][2]] && i != inStack[k][1])
			return 0;
	for (j = 1; j <= 9; j++)
		if (puzzle[inStack[k][1]][j] == puzzle[inStack[k][1]][inStack[k][2]] && j != inStack[k][2])
			return 0;
	if (inStack[k][1] <= 3)
		startRow = 1;
	else if (inStack[k][1] >= 4 && inStack[k][1] <= 6)
		startRow = 4;
	else
		startRow = 7;
	if (inStack[k][2] <= 3)
		startCol = 1;
	else if (inStack[k][2] >= 4 && inStack[k][2] <= 6)
		startCol = 4;
	else
		startCol = 7;
	for (i = startRow; i <= startRow + 2; i++)
		for (j = startCol; j <= startCol + 2; j++)
			if (puzzle[i][j] == puzzle[inStack[k][1]][inStack[k][2]] && (i != inStack[k][1] || j != inStack[k][2]))
				return 0;
	return 1;
}

/* Functia printPuzzle afiseaza prima solutie gasita de completare a puzzle-ului initial si opreste executia
   programului */
void printPuzzle()
{
	unsigned i, j;
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
			g << puzzle[i][j] << ' ';
		g << endl;
	}
	g.close();
	exit(0);
}

/* Se utilizeaza o implementare recursiva a tehnicii backtracking in scopul de a asigna fiecarei celule necompletate
   din puzzle o valoare (un candidat), iar valoarea sa-i fie corect atribuita in raport cu puzzle-ul */
void backtracking(unsigned k)
{
	unsigned candidate;
	for (candidate = 1; candidate <= 9; candidate++)
	{
		puzzle[inStack[k][1]][inStack[k][2]] = candidate;
		if (validateCell(k))
			if (k == emptyCells)
				printPuzzle();
			else
				backtracking(k + 1);
		/* In cazul in care celulei de pe nivelul curent din stiva nu ii este atribuita in mod valid o valoare,
          	   i-o restauram pe cea initiala (0, insemnand ca va ramane necompletata), iar la revenirile din lanturile
                   de autoapeluri se vor face prelucrarile necesare altor celule */
		puzzle[inStack[k][1]][inStack[k][2]] = 0;
	}
}

/* Programul principal. Input-ul utilizatorului se considera valid */
int main()
{
	unsigned i, j;
	for (i = 1; i <= 9; i++)
		for (j = 1; j <= 9; j++)
			f >> puzzle[i][j];
	f.close();
	fillStack();
	backtracking(1);
	return 0;
}
