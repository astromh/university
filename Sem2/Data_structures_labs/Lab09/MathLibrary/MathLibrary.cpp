#include <cmath>

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

// Internal function: calculates the series for variant 27
double SeriesFunction(double x, double epsilon) {
    double term = 1.0;
    double sum = 1.0;

    int k = 1;

    while (std::abs(term) >= epsilon) {
        term = term * x * (k + 1) / (k * k);
        sum += term;
        k++;

        // Protection from infinite loop
        if (k > 100000) {
            break;
        }
    }

    return sum;
}

// Internal function: calculates math value
double MathFunction(double x) {
    return std::exp(x) * (1.0 + x);
}

extern "C" __declspec(dllexport)
char* GetFunctionName() {
    return (char*)"e^x(1+x)";
}

extern "C" __declspec(dllexport)
char* GetAuthorName() {
    return (char*)"Dastanboo";
}

extern "C" __declspec(dllexport)
ColorPair GetHeaderColors() {
    ColorPair colors;
    colors.textColor = 14; // yellow text
    colors.bgColor = 1;    // blue background
    return colors;
}

extern "C" __declspec(dllexport)
ColorPair GetFooterColors() {
    ColorPair colors;
    colors.textColor = 10; // green text
    colors.bgColor = 0;    // black background
    return colors;
}

extern "C" __declspec(dllexport)
double CalculateMathFunction(double x, double param) {
    // param is not used in variant 27
    return MathFunction(x);
}

extern "C" __declspec(dllexport)
double CalculateFunction(
    double x,
    double param,
    double epsilon,
    CallbackFunc callback,
    int callCount
) {
    // param is not used in variant 27

    double f_series = SeriesFunction(x, epsilon);
    double F_math = MathFunction(x);

    double delta = std::sqrt(
        std::abs(f_series * f_series - F_math * F_math)
    );

    if (callback != nullptr) {
        callback(x, f_series, F_math, delta, callCount);
    }

    return f_series;
}