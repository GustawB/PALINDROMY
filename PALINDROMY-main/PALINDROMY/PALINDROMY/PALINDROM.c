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

//funkcja wypisuj¹ca stan planszy w ka¿dej turze
void PrintTable(char table[WIERSZE][KOLUMNY])
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

//funkcja ta znajduje pierwszy wolny element w danej kolumnie i "stawia" tam ¿eton danego gracza,
//a na koniec zwraca wiersz w którym gracz postawi³ swój ¿eton
int ReserveSpot(char table[WIERSZE][KOLUMNY], int column, char player)
{
	int row = WIERSZE - 1;
	while (row > 0 && table[row][column] != '-') { --row; }
	table[row][column] = player;

	return row;//zwracany wiersz w ktorym dany gracz ustawil swoj zeton
}

//funkcja ustawiaj¹ca wartoœæ pocz¹tkow¹ zmiennej beg na minimalna wartosc
//pocz¹tku ciagu zwyciestwa w przypadku sprawdzania poziomego
int SetBegHorizontal(int column)
{
	if (column + 1 - DLUGOSC < 0) { return 0; }//zdefiniowanie pocztku ciagu zwyciestwa
	else { return (column + 1 - DLUGOSC); }
}

//funkcja ustawiaj¹ca wartoœæ pocz¹tkow¹ zmiennej beg na minimalna wartosc
//pocz¹tku ciagu zwyciestwa w przypadku sprawdzania pionowego
int SetBegVertical(int row)
{
	if (row - 1 + DLUGOSC >= WIERSZE) { return (WIERSZE - 1); }//zdefiniowanie poczatku ciagu zwyciestwa
	else { return (row - 1 + DLUGOSC); }
}

//funkcja ustawiaj¹ca wartoœæ pocz¹tkow¹ zmiennej end na maksymalna wartosc
//konca ciagu zwyciestwa w przypadku sprawdzania poziomego
int SetEndHorizontal(int column)
{
	if (column + DLUGOSC >= KOLUMNY) { return (KOLUMNY - DLUGOSC); }//zdefiniowanie konca ciagu zwyciestwa
	else { return column; }
}

//funkcja ustawiaj¹ca wartoœæ pocz¹tkow¹ zmiennej end na maksymalna wartosc
//konca ciagu zwyciestwa w przypadku sprawdzania pionowego
int SetEndVertical(int row)
{
	if (row + 1 - DLUGOSC < 0) { return (DLUGOSC - 1); }//zdefiniowanie konca ciagu zwyciestwa
	else { return row; }
}

//Funkcja sprawdzajaca, czy ktos wygral w poziomie
bool HorizontalWinCondition(char table[WIERSZE][KOLUMNY],int row, int column)
{
	int beg = SetBegHorizontal(column);
	int end = SetEndHorizontal(column);

	bool bDidSomeoneWin = false;
	while (beg <= end && !bDidSomeoneWin)
	{
		int localBeg = beg;
		int localEnd = beg + DLUGOSC - 1;
		while (table[row][localBeg] == table[row][localEnd] && localBeg < localEnd && table[row][localBeg] != '-')//badanie, czy ciag dlugosci DLUGOSC jest palindromem
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

//funkcja sprawdzajaca, czy ktos wygral w pionie
bool VerticalWinCondition(char table[WIERSZE][KOLUMNY], int row, int column)
{
	int beg = SetBegVertical(row);
	int end = SetEndVertical(row);

	bool bDidSomeoneWin = false;
	while (beg >=end && !bDidSomeoneWin)
	{
		int localBeg = beg;
		int localEnd = beg - DLUGOSC + 1;
		while (table[localBeg][column] == table[localEnd][column] && localBeg > localEnd)
		{
			--localBeg;
			++localEnd;
		}
		int x = (localBeg + localEnd) / 2;
		if (localBeg <= localEnd && table[x][column] != '-') { bDidSomeoneWin = true; }

		--beg;
	}

	return bDidSomeoneWin;
}

//Funkcja sprawdzajaca, czy ktos wygral po przekatnej rosnacej
bool DiagonalRisingWinCondition(char table[WIERSZE][KOLUMNY], int row, int column)
{
	int begx = SetBegHorizontal(column);
	int begy = SetBegVertical(row);
	int endx = SetEndHorizontal(column);
	int endy = SetEndVertical(row);
	
	bool bDidSomeoneWin = false;
	while (begx <= endx && begy >= endy && !bDidSomeoneWin)
	{
		int localBegX = begx;
		int localBegY = begy;
		int localEndX = begx + DLUGOSC - 1;
		int localEndY = begy - DLUGOSC + 1;

		while (table[localBegY][localBegX] == table[localEndY][localEndX] && localBegX < localEndX && localBegY > localEndY && table[localBegY][localBegX] != '-')
		{
			++localBegX;
			--localBegY;
			--localEndX;
			++localEndY;
		}
		int x = (localBegX + localEndX) / 2;
		int y = (localBegY + localEndY) / 2;
		if (localBegX >= localEndX && localBegY <= localEndY && table[y][x]) { bDidSomeoneWin = true; }

		++begx;
		--begy;
	}

	return bDidSomeoneWin;	
}

//Funkcja sprawdzajaca,czy kto wygral po przekatnej malejacej
bool DiagonalLoweringWinCondition(char table[WIERSZE][KOLUMNY], int row, int column)
{
	int begx = column;
	int begy = row;
	while (begx > 0 && begy > 0)
	{
		--begx;
		--begy;
	}

	bool bDidSomeoneWin = false;
	while (begx <= KOLUMNY - DLUGOSC && begy <= WIERSZE - DLUGOSC && !bDidSomeoneWin)
	{
		int localBegX = begx;
		int localBegY = begy;
		int localEndX = begx + DLUGOSC - 1;
		int localEndY = begy + DLUGOSC - 1;
		while (table[localBegY][localBegX] == table[localEndY][localEndX] && localBegX < localEndX && localBegY < localEndY && table[localBegY][localBegX] != '-')
		{
			++localBegX;
			++localBegY;
			--localEndX;
			--localEndY;
		}
		if (localBegX >= localEndX && localBegY >= localEndY) { bDidSomeoneWin = true; }
		++begx;
		++begy;
	}

	return bDidSomeoneWin;
}

//Funkcja sprawdzajaca, czy ktos wygral w jeden z 4 mozliwych sposobow
bool CheckWinConditions(char table[WIERSZE][KOLUMNY], int row, int column)
{
	
	if (HorizontalWinCondition(table, row, column)) { return true; }
	else if (VerticalWinCondition(table, row, column)) { return true; }
	else if (DiagonalRisingWinCondition(table, row, column)) { return true; }
	else if (DiagonalLoweringWinCondition(table, row, column)) { return true; }

	return false;
}

int main(void)
{
	char table[WIERSZE][KOLUMNY];//deklaracja planszy do gry
	for (int i = 0; i < WIERSZE; ++i)
	{
		//inicjalizacja planszy pustymi wartoœciami	
		for (int j = 0; j < KOLUMNY; ++j) { table[i][j] = '-'; } 
	}

	char CurrentTurn = '1';//który gracz w danej turze gra ('1' lub '2');
	PrintTable(table);
	printf("%c%c\n", CurrentTurn, ':');
	char order;//zmienna przechowujaca jedna instruckje gracza w jego turze
	bool bIsGameOver = false;//Zmienna odpowiedzialna za stan gry
	while (!bIsGameOver)
	{
		order = (char)(getchar());
		//przy wpisywaniu recznym danych, getchar() czyta jako polecenie enter, 
		//wiec ponizej sprawdzamy, czy order przechowuje prawidlowe polecenie
		//oraz czy gracz nie poddal sie (nie podal jako polecenia kropki)
		if (order != '\n' && order != '.')
		{
			//przekonwertowanie chara na int, zeby moc operowac na tablicy 'table'
			int column = ((int)(order)) % a_IN_ASCII;
			int row = ReserveSpot(table, column, CurrentTurn);
			bIsGameOver = CheckWinConditions(table, row, column);

			PrintTable(table);
			if (!bIsGameOver)
			{
				//ustawienie, kto gra w nastepnej turze (gracz 1 lub 2)
				if (CurrentTurn == '1') { CurrentTurn = '2'; }
				else { CurrentTurn = '1'; }
				printf("%c:\n", CurrentTurn);
			}
		}
		else if (order == '.') { bIsGameOver = true; }//gdy gracz sie podda, to konczy sie gra
	}
	//jezeli gracz sie nie poddal, to ktos musial wygrac, i ponizej 
	//na konsoli "piszemy", ktory gracz jest zwyciezca ('1' lub '2')
	if (order != '.') { printf("%c!\n", CurrentTurn); }

	return 0;
}

