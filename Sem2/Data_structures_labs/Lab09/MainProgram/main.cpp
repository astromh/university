#include <iostream>
#include <iomanip>
#include <cmath>
#include <windows.h>

using namespace std;

struct ColorPair {
    int textColor;
    int bgColor;
};

typedef void (*CallbackFunc)(
    double x,
    double f_series,
    double F_math,
    double delta,
    int callCount
);

typedef char* (*GetFunctionNameFunc)();
typedef char* (*GetAuthorNameFunc)();
typedef ColorPair (*GetHeaderColorsFunc)();
typedef ColorPair (*GetFooterColorsFunc)();
typedef double (*CalculateFunctionFunc)(
    double x,
    double param,
    double epsilon,
    CallbackFunc callback,
    int callCount
);
typedef double (*CalculateMathFunctionFunc)(
    double x,
    double param
);

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

const int DEFAULT_COLOR = 7;

// You can change these later according to "Гадалкин дом"
const int NAME_COLOR = 11;
const int PATRONYMIC_COLOR = 5;

int currentTextColor = DEFAULT_COLOR;
int currentBgColor = 0;

void SetColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void ResetColor() {
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);
}

void PrintLine() {
    cout << "+------------+--------------------+--------------------+--------------------+\n";
}

void PrintHeader(const char* exeAuthor, const char* dllAuthor, const char* functionName) {
    PrintLine();
    cout << "| Author EXE: " << setw(61) << left << exeAuthor << "|\n";
    cout << "| Author DLL: " << setw(61) << left << dllAuthor << "|\n";
    cout << "| Function:   " << setw(61) << left << functionName << "|\n";
    PrintLine();
    cout << "| " << setw(10) << left << "x"
         << " | " << setw(18) << left << "f_series"
         << " | " << setw(18) << left << "F_math"
         << " | " << setw(18) << left << "delta"
         << " |\n";
    PrintLine();
}

void PrintFooter() {
    PrintLine();
}

void TableRowCallback(
    double x,
    double f_series,
    double F_math,
    double delta,
    int callCount
) {
    if (callCount == NAME_COLOR) {
        currentTextColor = NAME_COLOR;
    }

    if (callCount == PATRONYMIC_COLOR) {
        currentBgColor = PATRONYMIC_COLOR;
    }

    SetColor(currentTextColor, currentBgColor);

    cout << "| " << setw(10) << left << fixed << setprecision(4) << x
         << " | " << setw(18) << left << setprecision(10) << f_series
         << " | " << setw(18) << left << setprecision(10) << F_math
         << " | " << setw(18) << left << setprecision(10) << delta
         << " |\n";

    ResetColor();
}

bool IsInputValid(double xStart, double xEnd, double step, double epsilon) {
    if (epsilon <= 0) {
        cout << "Error: epsilon must be greater than 0.\n";
        return false;
    }

    if (step == 0) {
        cout << "Error: step cannot be 0.\n";
        return false;
    }

    if (xStart < xEnd && step < 0) {
        cout << "Error: step must be positive for this interval.\n";
        return false;
    }

    if (xStart > xEnd && step > 0) {
        cout << "Error: step must be negative for this interval.\n";
        return false;
    }

    return true;
}

int main() {
    SetConsoleOutputCP(65001);

    const char* exeAuthor = "Hemmat";

    double xStart, xEnd, step, epsilon;

    cout << "Enter x start: ";
    cin >> xStart;

    cout << "Enter x end: ";
    cin >> xEnd;

    cout << "Enter step: ";
    cin >> step;

    cout << "Enter epsilon: ";
    cin >> epsilon;

    if (!cin) {
        cout << "Error: invalid input.\n";
        return 1;
    }

    if (!IsInputValid(xStart, xEnd, step, epsilon)) {
        return 1;
    }

    HINSTANCE hDll = LoadLibraryA("MathLibrary/MathLibrary.dll");

    if (hDll == nullptr) {
        cout << "Error: could not load MathLibrary.dll\n";
        cout << "Make sure MathLibrary.dll is in the same folder as the EXE file.\n";
        return 1;
    }

    GetFunctionNameFunc GetFunctionName =
        (GetFunctionNameFunc)GetProcAddress(hDll, "GetFunctionName");

    GetAuthorNameFunc GetAuthorName =
        (GetAuthorNameFunc)GetProcAddress(hDll, "GetAuthorName");

    GetHeaderColorsFunc GetHeaderColors =
        (GetHeaderColorsFunc)GetProcAddress(hDll, "GetHeaderColors");

    GetFooterColorsFunc GetFooterColors =
        (GetFooterColorsFunc)GetProcAddress(hDll, "GetFooterColors");

    CalculateFunctionFunc CalculateFunction =
        (CalculateFunctionFunc)GetProcAddress(hDll, "CalculateFunction");

    CalculateMathFunctionFunc CalculateMathFunction =
        (CalculateMathFunctionFunc)GetProcAddress(hDll, "CalculateMathFunction");

    if (
        GetFunctionName == nullptr ||
        GetAuthorName == nullptr ||
        GetHeaderColors == nullptr ||
        GetFooterColors == nullptr ||
        CalculateFunction == nullptr ||
        CalculateMathFunction == nullptr
    ) {
        cout << "Error: DLL does not contain all required functions.\n";
        FreeLibrary(hDll);
        return 1;
    }

    const char* functionName = GetFunctionName();
    const char* dllAuthor = GetAuthorName();

    ColorPair headerColors = GetHeaderColors();

    SetColor(headerColors.textColor, headerColors.bgColor);
    PrintHeader(exeAuthor, dllAuthor, functionName);
    ResetColor();

    currentTextColor = DEFAULT_COLOR;
    currentBgColor = 0;

    int callCount = 1;
    double x = xStart;

    if (step > 0) {
        while (x <= xEnd + 1e-12) {
            CalculateFunction(x, 0, epsilon, TableRowCallback, callCount);
            x += step;
            callCount++;
        }
    } else {
        while (x >= xEnd - 1e-12) {
            CalculateFunction(x, 0, epsilon, TableRowCallback, callCount);
            x += step;
            callCount++;
        }
    }

    ColorPair footerColors = GetFooterColors();

    SetColor(footerColors.textColor, footerColors.bgColor);
    PrintFooter();
    ResetColor();

    double xIdeal;

    cout << "\nEnter x for second table: ";
    cin >> xIdeal;

    if (!cin) {
        cout << "Error: invalid x value.\n";
        FreeLibrary(hDll);
        return 1;
    }

    cout << "\nTable 2: different epsilon values\n";
    PrintLine();

    cout << "| " << setw(10) << left << "epsilon"
         << " | " << setw(18) << left << "f_series"
         << " | " << setw(18) << left << "F_math"
         << " | " << setw(18) << left << "delta"
         << " |\n";

    PrintLine();

    for (int power = 1; power <= 7; power++) {
        double eps = pow(10.0, -power);

        double f_series = CalculateFunction(xIdeal, 0, eps, nullptr, 0);
        double F_math = CalculateMathFunction(xIdeal, 0);

        double delta = sqrt(abs(f_series * f_series - F_math * F_math));

        cout << "| " << setw(10) << left << scientific << setprecision(1) << eps
             << " | " << setw(18) << left << fixed << setprecision(10) << f_series
             << " | " << setw(18) << left << F_math
             << " | " << setw(18) << left << delta
             << " |\n";
    }

    PrintLine();

    FreeLibrary(hDll);

    return 0;
}