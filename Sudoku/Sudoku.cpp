#include "stdafx.h"
#include "Sudoku.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <Uxtheme.h>
#include <vector>
#pragma comment(lib,"UxTheme.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
bool gAktywny = false;
HINSTANCE hInst;
int licznik[9] = {0,0,0,0,0,0,0,0,0};
int maxN = 45;
int tHour=0, tMin = 0;
std::clock_t tStart=NULL;
UINT tID = NULL;
bool CALLBACK robPlansze(int);
int CALLBACK sprawdzLiczbe(int, int, int*, int);
void CALLBACK losujPoczatkowe(HWND);
void CALLBACK timerFunkcja(HWND, UINT, UINT, DWORD);
void CALLBACK zacznij(HWND);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Sudoku(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Wybor(HWND, UINT, WPARAM, LPARAM);
int losPlansza[81] = {
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};
int obecnaPlansza[81] = {
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	HWND hDlg;
	MSG msg;
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_SUDOKU), 0, Sudoku, 0);
	ShowWindow(hDlg, nCmdShow);
	SetWindowTheme(hDlg, L"Explorer", NULL);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
bool CALLBACK robPlansze(int p) {
	srand(std::time(0));
	int r = 0, s = 0;
	bool s2 = false;
	bool bylo[9];
	for (int i = 0; i < 9; i++)bylo[i] = false;
	if (p == 81)return true;
	for (int i = 0; i < 9; i++) {
		do {
			r = (std::rand() % 9) + 1;
		} while (bylo[r - 1] == true);
		s = sprawdzLiczbe(r, p,losPlansza,0);
		if (s == -1) {
			losPlansza[p] = r;
			s2 = robPlansze(p + 1);
			if (s2 == true)return true;
		}
		bylo[r - 1] = true;
	}
	losPlansza[p] = 0;
	return false;
}
int CALLBACK sprawdzLiczbe(int l, int k, int* plansza, int tryb) {
	int x=0, y=0,tmp = 9,a=0,b=0;
	x = (k) / 9;
	y = (k) % 9;
	if (x > 0) {
		if (tryb == 0)tmp = x;
		for (int i = 0; i < tmp; i++) {
			if (plansza[i * 9 + y] == l)return i*9+y;//pionowo
		}
	}
	if (y > 0) {
		if (tryb == 0)tmp = y;
		for (int j = 0; j < tmp; j++) {
			if (plansza[x * 9 + j] == l)return x*9+j;//poziomo
		}
	}
	if (x < 3)a = 0;
	else if (x > 2 && x < 6)a = 3;
	else if (x > 5 && x < 9)a = 6;
	if (y < 3)b = 0;
	else if (y > 2 && y < 6)b = 3;
	else if (y > 5 && y < 9)b = 6;
	for (int j = 0; j < 9; j++) {
		if (plansza[a * 9 + b] == l&&a != x&&b != y)return a*9+b;
		b++;
		if (b % 3 == 0) {
			a++;
			b -= 3;
		}
	}//mniejsze kwadraty
	return -1;
}
void CALLBACK losujPoczatkowe(HWND e) {
	int r;
	std::vector<int> t;
	t.reserve(maxN);
	bool s;
	std::string ss;
	for (int i = 0; i < maxN; i++)t.push_back(0);
	for (int i = 0; i < 81; i++)SendMessage(GetDlgItem(e, 1000 + i), WM_SETTEXT, 0, (LPARAM)"");
	for (int i = 0; i < maxN; i++) {
		do {
			s = false;
			r = (std::rand() % 81);
			for (int j = 0; j < maxN; j++) {
				if (t[j] == r) {
					s = true;
					break;
				}
			}
		} while (s == true);
		t[i] = r;
		obecnaPlansza[r] = losPlansza[r];
		ss =std::to_string(losPlansza[r]);
		SendMessage(GetDlgItem(e, r + 1000), WM_SETTEXT, 0, (LPARAM)ss.c_str());
		licznik[losPlansza[r]-1]++;
	}
	for (int i = 0; i < 9; i++) {
		ss=std::to_string(licznik[i]);
		SendMessage(GetDlgItem(e, 1085 + i), WM_SETTEXT, 0, (LPARAM)ss.c_str());
	}
}
void CALLBACK timerFunkcja(HWND hWnd, UINT message, UINT nIDEvent, DWORD dwTime) {
	long dur = (std::clock() - tStart)/(long)CLOCKS_PER_SEC;
	std::string t="";
	if (dur > 59) {
		tMin++;
		dur = 0;
		tStart = std::clock();
	}
	std::string ss;
	if (tMin > 59) {
		tHour++;
		tMin = 0;
	}
	if (tHour < 10)t = "0";
	ss =std::to_string(tHour);
	t += ss +":";
	if (tMin < 10)t += "0";
	ss = std::to_string(tMin);
	t += ss +":";
	if (dur < 10)t += "0";
	ss = std::to_string(dur);
	t += ss;
	SendMessage(GetDlgItem(hWnd, IDC_TIME), WM_SETTEXT, 0, (LPARAM)t.c_str());
}
void CALLBACK zacznij(HWND hDlg) {
	for (int i = 0; i < 81; i++) {
		losPlansza[i] = 0;
		obecnaPlansza[i] = 0;
	}
	for (int i = 0; i < 9; i++)licznik[i] = 0;
	robPlansze(0);
/*	std::fstream plik;
	plik.open("p.txt", std::ios::out);
	for (int i = 0; i < 81; i++) {
		plik << losPlansza[i];
		if (i % 9 == 8 && i > 0)plik << "\n";
	}
	plik.close();*/
	losujPoczatkowe(hDlg);
	if (tID != NULL)KillTimer(hDlg, 1);
	tID = SetTimer(hDlg, 1, 1000, &timerFunkcja);
	tStart = std::clock();
	tHour = 0;
	tMin = 0;
	gAktywny = true;
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Sudoku(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		srand(std::time(NULL));
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDM_EXIT)DestroyWindow(hDlg);
		else if (LOWORD(wParam) == IDM_ABOUT)DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
		else if (LOWORD(wParam) == IDM_DEASY) {
			maxN = 45;
			zacznij(hDlg);
		}
		else if (LOWORD(wParam) == IDM_DMEDIUM) {
			maxN = 35;
			zacznij(hDlg);
		}
		else if (LOWORD(wParam) == IDM_DHARD) {
			maxN = 30;
			zacznij(hDlg);
		}
		else if (LOWORD(wParam) == IDC_SUD&&gAktywny==true)zacznij(hDlg);
		else if(gAktywny==true){
			std::string ss;
			int x = 0;
			int tmp = obecnaPlansza[LOWORD(wParam) - 1000];
			x = DialogBox(hInst, MAKEINTRESOURCE(IDD_WYBOR), hDlg, Wybor);
			if (x > 0) {
				if (tmp != 0) {
					licznik[tmp - 1]--;
					ss = std::to_string(licznik[tmp - 1]);
					SendMessage(GetDlgItem(hDlg, 1085 + tmp - 1), WM_SETTEXT, 0, (LPARAM)ss.c_str());
				}
				if (licznik[x - 1] == 9)MessageBox(hDlg, "You have put nine of this number already.", "Note", MB_OK);
				else {
					int y = sprawdzLiczbe(x, LOWORD(wParam) - 1000, obecnaPlansza, 1);
					if (y == -1) {
						licznik[x - 1]++;
						ss = std::to_string(licznik[x - 1]);
						SendMessage(GetDlgItem(hDlg, 1085 + x - 1), WM_SETTEXT, 0, (LPARAM)ss.c_str());
						ss = std::to_string(x);
						SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), WM_SETTEXT, 0, (LPARAM)ss.c_str());
						bool sss = true;
						for (int i = 0; i < 9; i++) {
							if (licznik[i] < 9) {
								sss = false;
								break;
							}
						}
						obecnaPlansza[LOWORD(wParam) - 1000] = x;
						if (sss == true) {
							MessageBox(hDlg, "You did it!\n Congratulations!", "The end", MB_OK);
							KillTimer(hDlg, 1);
						}
					}
					else MessageBox(hDlg, "You cannot do it.", "Bad move", MB_OK);
				}
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hDlg);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Wybor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		POINT pt;
		GetCursorPos(&pt);
		SetWindowPos(hDlg, 0, pt.x-60, pt.y-80, 0, 0, SWP_NOSIZE);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, 0);
			return (INT_PTR)TRUE;
		}
		else {
			char* x = new char[4];
			GetWindowText(GetDlgItem(hDlg, LOWORD(wParam)), x, 4);
			std::string tmp(x);
			delete[] x;
			int i = std::stoi(tmp, nullptr, 10);
			EndDialog(hDlg, i);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}