#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#ifndef WIERSZE
#define WIERSZE 8
#endif // !WIERSZE

#ifndef KOLUMNY
#define KOLUMNY 8
#endif // !KOLUMNY

#ifndef DLUGOSC
#define DLUGOSC 5
#endif // !DLUGOSC

#define a_IN_ASCII 97

void PrintTable(char table[WIERSZE][KOLUMNY])//funkcja wypisuj¹ca stan planszy w ka¿dej turze
{
	for (int i = 0; i < WIERSZE; ++i)
	{
		for (int j = 0; j < KOLUMNY; ++j)
		{
			putchar(' ');
			putchar(table[i][j]);
		}
		printf("\n");
	}
	for (int i = 0; i < KOLUMNY; ++i)
	{
		putchar(' ');
		printf("%c", (char)(a_IN_ASCII + i));//wypisanie nazw kolumn pod plansza
		
	}
	printf("\n");
}

int ReserveSpot(char table[WIERSZE][KOLUMNY], int column, char player)//funkcja ta znajduje pierwszy wolny element w danej kolumnie i "stawia" tam ¿eton danego gracza
{
	int row = WIERSZE - 1;
	while (row > 0 && table[row][column] != '-') { --row; }
	table[row][column] = player;

	return row;//zwracany wiiersz w ktorym dany gracz ustawil swoj zeton
}

bool HorizontalWinCondition(char table[WIERSZE][KOLUMNY],int row, int column)//sprawdzenie czy dany 'player' wygral "w pionie"
{
	int beg;
	if (column + 1 - DLUGOSC < 0) { beg = 0; }//zdefiniowanie pocztku ciagu zwyciestwa
	else { beg = column + 1 - DLUGOSC; }
	int end;
	if (column + DLUGOSC >= KOLUMNY) { end = KOLUMNY - DLUGOSC; }
	else { end = column; }

	bool bDidSomeoneWin = false;
	while (beg <= end && !bDidSomeoneWin)
	{
		int localBeg = beg;
		int localEnd = beg + DLUGOSC - 1;
		while (table[row][localBeg] == table[row][localEnd] && localBeg < localEnd)//badanie, czy ciag dlugosci DLUGOSC jest palindromem
		{
			++localBeg;
			--localEnd;
		}
		if (localBeg >= localEnd) { bDidSomeoneWin = true; }

		++beg;
	}

	return bDidSomeoneWin;//jesli beg > end to DLUGOSC jest parz, beg == end to DLUGOSC nieparz
	//jesli beg >= end to mamy palindrom, nie w przeciwnym wypadku
}

bool VerticalWinCondition(char table[WIERSZE][KOLUMNY], int row, int column)//sprawdzenie czy dany 'player' wygral w poziomie
{
	int beg;
	if (row - 1 + DLUGOSC >= WIERSZE) { beg = WIERSZE -1; }//zdefiniowanie poczatku ciagu zwyciestwa
	else { beg = row - 1 + DLUGOSC; }
	int end;
	if (row + 1 - DLUGOSC < 0) { end = DLUGOSC - 1; }
	else { end = row; }

	bool bDidSomeoneWin = false;
	while (beg >=end && !bDidSomeoneWin)
	{
		int localBeg = beg;
		int localEnd = beg - DLUGOSC + 1;
		printf("%d%d\n", localBeg, localEnd);
		while (table[localBeg][column] == table[localEnd][column] && localBeg > localEnd)//badanie czy ciag dlugosci DLUGOSC jest palindromem
		{
			--localBeg;
			++localEnd;
		}
		if (localBeg <= localEnd) { bDidSomeoneWin = true; }

		--beg;
	}

	return bDidSomeoneWin;//jesli beg < end to DLUGOSC jest parz, beg == end to DLUGOSC nieparz
	//jesli beg <= end to mamy palindrom, nie w przeciwnym wypadku

}

bool DiagonalRisingWinCondition(char table[WIERSZE][KOLUMNY], int row, int column)//sprawdzenie, czy ktos wygral po przekatnej rosnacej
{
	int begx;
	int begy;
	if (row - 1 + DLUGOSC >= WIERSZE || column + 1 - DLUGOSC < 0) { return false; }
	else
	{
		begx = column + 1 - DLUGOSC;
		begy = row - 1 + DLUGOSC;
	}

	int endx = column;
	int endy = row;

	while (table[begy][begx] == table[endy][endx] && begx < endx && begy > endy)
	{
		++begx;
		--begy;
		--endx;
		++endy;
	}

	return ((begx >= endx) && (begy <= endy));
}

bool DiagonalLoweringWinCondition(char table[WIERSZE][KOLUMNY], int row, int column)//sprawdzenie, czy ktos wygral po przekatnej malejacej
{
	int begx;//nr kolumny w ktorej znajduje sie wart poczatkowa ciagu zwyciestwa
	int begy;//nr wiersza w ktorym znajduje sie wart poczatkowa ciagu zwyciestwa
	if (row + 1 -  DLUGOSC < 0 || column + 1 - DLUGOSC  < 0 ) { return false; }
	else
	{
		begx = column + 1 - DLUGOSC;
		begy = 0;
		while (table[begy][begx] == '-')
			++begy;
	}

	int endx = column;
	int endy = row;

	while (table[begy][begx] == table[endy][endx] && begx < endx && begy < endy)
	{
		++begx;
		++begy;
		--endx;
		--endy;
	}

	return ((begx >= endx) && (begy >= endy));
}

bool CheckWinConditions(char table[WIERSZE][KOLUMNY], int row, int column)//sprawdza, czy ktos wygral w przynajmniej jeden z mozliwych sposobow
{
	if (HorizontalWinCondition(table, row, column)) { return true; }
	else if (VerticalWinCondition(table, row, column)) { return true; }
	//else if (DiagonalRisingWinCondition(table, row, column)) { return true; }
	//else if (DiagonalLoweringWinCondition(table, row, column)) { return true; }

	return false;
}

int main(void)
{
	char table[WIERSZE][KOLUMNY];//plansza gry
	for (int i = 0; i < WIERSZE; ++i)
	{
		for (int j = 0; j < KOLUMNY; ++j) { table[i][j] = '-'; } //inicjalizacja tablicy "table" pustymi wartoœciami	
	}

	char CurrentTurn = '1';//który gracz w danej turze gra ('1' lub '2');
	bool bIsGameOver = false;//Zmienna odpowiedzialna za stan gry
	PrintTable(table);
	printf("%c%c\n", CurrentTurn, ':');
	while (!bIsGameOver)
	{
		char order = getchar();
		
		if (order != '\n' && order != '.')//ignorujemy  zeby while sie nie wykonal podwojnie (getchar jak debil czyta znak i enter)
		{
			int column = ((int)(order)) % a_IN_ASCII;
			int row = ReserveSpot(table, column, CurrentTurn);
			bIsGameOver = CheckWinConditions(table, row, column);

			PrintTable(table);
			if (!bIsGameOver)
			{
				if (CurrentTurn == '1') { CurrentTurn = '2'; }
				else { CurrentTurn = '1'; }
				printf("%c%c\n", CurrentTurn, ':');
			}
		}
		else if (order == '.') { bIsGameOver = true; }
	}
	printf("%c%c\n", CurrentTurn, '!');

	return 0;
}

