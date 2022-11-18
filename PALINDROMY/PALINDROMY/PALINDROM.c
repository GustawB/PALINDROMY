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

bool HorizontalWinCondition(char table[WIERSZE][KOLUMNY],int row, int column, char player)//sprawdzenie czy dany 'player' wygral "w pionie"
{
	int beg;
	if (column + 1 - DLUGOSC < 0) { return false; }//zdefiniowanie pocztku ciagu zwyciestwa
	else { beg = column + 1 - DLUGOSC; }
	int end = column;//koniec ciagu zwyciestwa

	while (table[row][beg] == table[row][end] && beg < end)//badanie, czy ciag dlugosci DLUGOSC jest palindromem
	{
		++beg;
		--end;
	}

	return (beg >= end);//jesli beg > end to DLUGOSC jest parz, beg == end to DLUGOSC nieparz
	//jesli beg >= end to mamy palindrom, nie w przeciwnym wypadku
}

bool VerticalWinCondition(char table[WIERSZE][KOLUMNY], int row, int column, char player)//sprawdzenie czy dany 'player' wygral w poziomie
{
	int beg;
	if (row - 1 + DLUGOSC >= WIERSZE) { return false; }//zdefiniowanie poczatku ciagu zwyciestwa
	else { beg = row - 1 + DLUGOSC; }
	int end = row;//koniec ciagu zwyciestwa

	while (table[beg][column] == table[end][column] && beg > end)//badanie czy ciag dlugosci DLUGOSC jest palindromem
	{
		--beg;
		++end;
	}

	return (beg <= end);//jesli beg < end to DLUGOSC jest parz, beg == end to DLUGOSC nieparz
	//jesli beg <= end to mamy palindrom, nie w przeciwnym wypadku

}

bool DiagonalRisingWinCondition(char table[WIERSZE][KOLUMNY], int row, int column, char player)//sprawdzenie, czy dany 'player' wygral po przekatnej rosnacej
{// zamiana row i column daje druga przekatna
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

	printf("%d%d%d%d\n", begx, begy, endx, endy);
	while (table[begy][begx] == table[endy][endx] && begx < endx && begy > endy)
	{
		++begx;
		--begy;
		--endx;
		++endy;
	}

	return ((begx >= endx) && (begy <= endy));
}

bool DiagonalLoweringWinCondition(char table[WIERSZE][KOLUMNY], int row, int column, char player)//sprawdzenie, czy dany 'player' wygral po przekatnej rosnacej
{
	int begx;
	int begy;
	if (row + 1 -  DLUGOSC < 0 || column + 1 - DLUGOSC  < 0 ) { return false; }
	else
	{
		begx = column + 1 - DLUGOSC;
		begy = 0;
		while (table[begy][begx] != '-')
			++begy;
		--begy;
	}

	int endx = column - 1 + DLUGOSC;
	int endy = row - 1 + DLUGOSC;

	printf("%d%d\n", begx, begy);
	while (table[begy][begx] == table[endy][endx] && begx < endx && begy > endy)
	{
		++begx;
		--begy;
		--endx;
		++endy;
	}

	return ((begx >= endx) && (begy <= endy));
}

bool CheckWinConditions(char table[WIERSZE][KOLUMNY], int column, char player)
{
	bool wynik = false;//Wartosc zwracana przez funkcje CheckWinCondition. Tru gdy player wygra, false w przeciwnym razie
	//sprawdzenie w poziomie
	int beg;
	if (column + 1 - DLUGOSC < 0) { beg = 0; }//zdefiniowanie minimalnego poczatku ciagu zwyciestwa
	//else {beg = }

}

int main(void)
{
	
	char table[WIERSZE][KOLUMNY];//plansza gry
	for (int i = 0; i < WIERSZE; ++i)
	{
		for (int j = 0; j < KOLUMNY; ++j)
		{
			table[i][j] = '-'; //inicjalizacja tablicy "table" pustymi wartoœciami
		}
			
	}

	char CurrentTurn = '1';//który gracz w danej turze gra ('1' lub '2');

	bool bIsGameOver = false;//Zmienna odpowiedzialna za stan gry, 0 czyli gra trwa, 1 kiedy siê konczy

	
	
	while (!bIsGameOver)
	{
		char order = getchar();
		
		if (order != '\n')//ignorujemy  zeby while sie nie wykonal podwojnie (getchar jak debil czyta znak i enter)
		{
			if (order == '.') { bIsGameOver = true; }
			else
			{
				int column = ((int)(order)) % a_IN_ASCII;
				int row = ReserveSpot(&table, column, CurrentTurn);
				//printf("%d%d\n", row, column);

				if (DiagonalLoweringWinCondition(table, row, column, CurrentTurn) == true)
					bIsGameOver = true;

				PrintTable(table);
				if (CurrentTurn == '1') { CurrentTurn = '2'; }
				else { CurrentTurn = '1'; }
			}
		}
		

	}


	/*
	while (isGameOver == 0)
	{
		char order;
		scanf("%c\n", &order);//Ta wariacja scanfa nie czyta if'a, i while nie wykonuje siê podwójnie
		putchar('x');

	}*/

	return 0;
}

