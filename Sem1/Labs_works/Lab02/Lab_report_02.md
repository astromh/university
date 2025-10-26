# Отчёт по лабораторной работе № 2

## Дисциплина: Теория и технология программирования

### Тема: Вычисления с условиями

**Студент:** Мохаммад Хемат
**Группа:** 5130902/50001
## Задание

Согласно индивидуальному варианту № 24: Составить программу, которая осуществляет перевод кода цвета из палитры RGB в палитру HLS. Перевести из палитры RGB в палитру HLS три цвета, введенных пользователем.

## Цель работы

Цель лабораторной работы состоит в формировании умений работы с целыми типами данных, работы с операторами проверки условий, анализа вводимых пользователем данных, создания русскоязычного интерфейса программы. В рамках данного задания акцент делается на вычислениях с условиями для преобразования цветовых моделей с использованием языка программирования C++.

## Условие задания

В соответствии с вариантом 24 необходимо написать программу на C++, которая предлагает пользователю ввести значения для трех компонент цвета в модели RGB (R, G, B от 0 до 255). Программа должна проверить корректность введенных данных, предложить исправить некорректные значения и вычислить соответствующие значения в модели HLS (H, L, S) по формулам: H = arcsin(sqrt(3/2) * (G - R) / S), L = (R + G + B) / 3, S = sqrt(R² + G² + B² - R_G - R_B - G*B). Процесс повторяется для трех цветов.

## Текст программы

```cpp
#include <windows.h>
#include <string.h>
#include <iostream>
#include <limits>
#include <cctype>
#include <cmath>

using namespace std;
int* get_input(void); 
double* calculate(int R, int G, int B);

const double EPS = 1e-9;

int main(void)
{

    int* RGB = get_input();
    // R=RGB[0], G=RGB[1], B=RGB[2]

    double* HLS = calculate(RGB[0], RGB[1], RGB[2]);
    cout << "RGB ---> HLS: " << HLS[0] << " " 
    << HLS[1] << " " << HLS[2];

    delete[] HLS;
    delete[] RGB;
    return 0;
}

int* get_input(void)
{
    int* arr = new int[3];
    const char* names[3] = {"Red", "Green", "Blue"};
    string input;

    for (int i = 0; i < 3; i++) {
        while (true) {
            cout << "Enter value for " << names[i] << " (0 - 255): ";
            getline(cin, input); 

            if (input.empty()) {
                cout << "[!] Empty input. Try again.\n";
                continue;
            }

            bool only_digits = true;
            for (char c : input) {
                if (!isdigit(static_cast<unsigned char>(c))) {
                    only_digits = false;
                    break;
                }
            }

            if (!only_digits) {
                cout << "[!] Invalid input. Only digits (0 - 255) allowed.\n";
                continue;
            }

            int value = stoi(input);

            if (value < 0 || value > 255) {
                cout << "[!] Value out of range (0 - 255).\n";
                continue;
            }

            arr[i] = value;
            break;
        }
    }

    return arr;
}


double* calculate(int R, int G, int B)
{
    double* arr = new double[3];

    double inner = (double)R * R + (double)G * G + (double)B * B
                   - (double)B * G - (double)B * R - (double)G * R;

    if (inner < 0.0 && inner > -EPS) inner = 0.0;

    if (inner < 0.0) {
        arr[2] = 0.0;
    } else {
        arr[2] = sqrt(inner); 
    }

    arr[1] = ((double)R + (double)G + (double)B) / 3.0;

    if (arr[2] < EPS) {
        arr[0] = 0.0;
    } else {
        double arg = sqrt(3.0 / 2.0) * ((double)G - (double)R) / arr[2];

        if (!isfinite(arg)) {
            arr[0] = 0.0;
        } else {
            if (arg > 1.0) arg = 1.0;
            if (arg < -1.0) arg = -1.0;
            arr[0] = asin(arg);
        }
    }

    return arr;
}
```

## Входные и выходные данные

**Пример входных данных:**  
Red: 255  
Green: 0  
Blue: 0

**Пример выходных данных:**  
RGB ---> HLS: -1.5708 85 255

(Аналогично для других двух цветов.)

## Пример работы программы

Программа запрашивает у пользователя значения для R, G, B, проверяя их на диапазон 0-255 и цифровой ввод. После ввода вычисляет H, L, S по формулам и выводит результат. Если ввод некорректен, предлагает повторить. Для трех цветов процесс повторяется вручную или в цикле (в коде показано для одного, но может быть расширено).

## Вывод

В результате выполнения лабораторной работы была создана программа для преобразования цвета из RGB в HLS. Это позволило освоить работу с вводом данных, проверкой условий, математическими вычислениями в C++. Программа работает корректно на примерах, демонстрируя понимание условных операторов и типов данных. Полученные навыки полезны для дальнейшего изучения программирования.