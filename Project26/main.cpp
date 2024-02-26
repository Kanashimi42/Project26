#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"

HINSTANCE hInstance;

DWORD WINAPI Thread1(LPVOID lp);
DWORD WINAPI Thread2(LPVOID lp);
DWORD WINAPI Thread3(LPVOID lp);

bool isPrime(int);

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const int maxIndex = 200000;

const int numberList = 3;
const int numberButton = 6;

int hListIds[numberList] = { IDC_LIST1, IDC_LIST2, IDC_LIST3 };
HWND hLists[numberList] = {};

int hButtonIds[numberButton] = { IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4, IDC_BUTTON5, IDC_BUTTON6 };
HWND hButtons[numberButton] = {};

HANDLE hThread1, hThread2, hThread3;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG: {
			for (int i = 0; i < numberList; i++) {
				hLists[i] = GetDlgItem(hwnd, hListIds[i]);
			}

			for (int i = 0; i < numberButton; i++) {
				hButtons[i] = GetDlgItem(hwnd, hButtonIds[i]);
			}

			hThread1 = CreateThread(NULL, 0, Thread1, hLists[0], CREATE_SUSPENDED, NULL);
			hThread2 = CreateThread(NULL, 0, Thread2, hLists[1], CREATE_SUSPENDED, NULL);
			hThread3 = CreateThread(NULL, 0, Thread3, hLists[2], CREATE_SUSPENDED, NULL);

			return TRUE;
		}
		case WM_COMMAND: {
			if (IDC_BUTTON1 == LOWORD(wParam) && HIWORD(wParam) == BN_CLICKED) {
				ResumeThread(hThread1);
			}
			else if (IDC_BUTTON2 == LOWORD(wParam) && HIWORD(wParam) == BN_CLICKED) {
				SuspendThread(hThread1);
			}

			else if (IDC_BUTTON3 == LOWORD(wParam) && HIWORD(wParam) == BN_CLICKED) {
				ResumeThread(hThread2);
			}
			else if (IDC_BUTTON4 == LOWORD(wParam) && HIWORD(wParam) == BN_CLICKED) {
				SuspendThread(hThread2);
			}

			else if (IDC_BUTTON5 == LOWORD(wParam) && HIWORD(wParam) == BN_CLICKED) {
				ResumeThread(hThread3);
			}
			else if (IDC_BUTTON6 == LOWORD(wParam) && HIWORD(wParam) == BN_CLICKED) {
				SuspendThread(hThread3);
			}

			return TRUE;
		}
		case WM_CLOSE: {
			EndDialog(hwnd, 0);

			return TRUE;
		}
	}

	return FALSE;
}

DWORD WINAPI Thread1(LPVOID lp) {
	DWORD pr = GetPriorityClass(GetCurrentProcess());

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	HWND hWnd = HWND(lp);

	for (int i = 0; i <= maxIndex; i++) {
		if (isPrime(i)) {
			TCHAR buff[MAX_PATH];

			wsprintf(buff, TEXT("%d"), i);
			int index = SendMessage(hWnd, LB_ADDSTRING, 0, LPARAM(buff));
			SendMessage(hWnd, LB_SETCARETINDEX, index, FALSE);
		}
	}

	SetPriorityClass(GetCurrentProcess(), pr);

	return 0;
}

DWORD WINAPI Thread2(LPVOID lp) {
	DWORD pr = GetPriorityClass(GetCurrentProcess());

	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	HWND hWnd = HWND(lp);

	for (int i = 0; i <= maxIndex; i++) {
		if (i % 2 == 0) {
			TCHAR buff[MAX_PATH];

			wsprintf(buff, TEXT("%d"), i);
			int index = SendMessage(hWnd, LB_ADDSTRING, 0, LPARAM(buff));
			SendMessage(hWnd, LB_SETCARETINDEX, index, FALSE);
		}
	}

	SetPriorityClass(GetCurrentProcess(), pr);

	return 0;
} 

DWORD WINAPI Thread3(LPVOID lp) {
	DWORD pr = GetPriorityClass(GetCurrentProcess());

	SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);

	HWND hWnd = HWND(lp);

	for (int i = 0; i <= maxIndex; i++) {
		TCHAR buff[MAX_PATH];

		wsprintf(buff, TEXT("%d"), i);
		int index = SendMessage(hWnd, LB_ADDSTRING, 0, LPARAM(buff));
		SendMessage(hWnd, LB_SETCARETINDEX, index, FALSE);
	}

	SetPriorityClass(GetCurrentProcess(), pr);

	return 0;
}


bool isPrime(int n) {
	if (n <= 1) {
		return false;
	}
	for (int i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}