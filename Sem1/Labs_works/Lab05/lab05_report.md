# ОТЧЕТ  
## по дисциплине «Алгоритмизация и программирование»  
### Лабораторная работа №5  
### Тема: «Массивы»

---

## Задание

В соответствии с индивидуальным вариантом необходимо выполнить задание №27.

Дана матрица A размером N × M. Требуется:

1. Вывести сумму всех элементов исходной матрицы.  
2. Преобразовать матрицу, интерпретируя её элементы как линейную последовательность:  
   - заменить первый элемент разностью первого и второго,  
   - второй — разностью второго и третьего,  
   - …  
   - последний элемент остаётся неизменным.  
3. Вывести сумму элементов преобразованной матрицы.

Пользователь вручную вводит размеры матрицы и её элементы.  
Необходимо выполнить защиту от некорректного ввода.

---

## Решение

Ниже представлен текст программы на языке C++.

```cpp
#include <iostream>
#include <limits>

using namespace std;

int ReadPositiveInt(const string& prompt, int MaxValue);
void TransformDifferences(int** A, int N, int M);
long long NewSum(int** A, int N, int M);
long long OriginalSum(int** A, int N, int M);

int main()
{
    const int MAXN = 100;
    const int MAXM = 100;

    int N = ReadPositiveInt("Enter number of rows N: ", MAXN);
    int M = ReadPositiveInt("Enter number of columns M: ", MAXM);

    int** Arr = new int*[N];
    for (int i=0; i<N; ++i){
        Arr[i] = new int[M];
    }

    cout << "Now Enter " << N*M << " elements (row by row):\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cout << "Arr["<<i<<"]["<<j<<"] = ";
            while (!(cin >> Arr[i][j])){
                cout << "[-] Invalid input! Please enter an integer number!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Arr["<<i<<"]["<<j<<"] = ";
            }
        }
    }

    cout << "\nOriginal Matrix:\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    long long Sum = OriginalSum(Arr, N, M);
    cout << "\nThe sum of the original matrix is: " << Sum << "\n";

    TransformDifferences(Arr, N, M);

    cout << "\nTransformed Matrix:\n";
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cout << Arr[i][j] << "\t";
        }
        cout << "\n";
    }

    long long TransformedSum = NewSum(Arr, N, M);
    cout << "\nThe sum of the new transformed matrix is: " << TransformedSum << "\n";

    for (int i=0; i<N; ++i){
        delete[] Arr[i];
    }
    delete[] Arr;

    return 0;
}

int ReadPositiveInt(const string& prompt, int MaxValue){
    int x;
    while (true){
        cout << prompt;
        if (!(cin >> x)){
            cout << "[-] Invalid input! Please enter an integer number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x <= 0 || x > MaxValue){
            cout << "Value must be between 1 and " << MaxValue << ". Try again!\n";
            continue;
        }
        return x;
    }
}

long long OriginalSum(int** A, int N, int M){
    long long sum = 0;
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            sum += A[i][j];
        }
    }
    return sum;
}

void TransformDifferences(int** A, int N, int M){
    int total = N*M;

    for (int k = 0; k < total - 1; ++k){
        int i = k / M;
        int j = k % M;
        int ni = (k+1) / M;
        int nj = (k+1) % M;

        A[i][j] = A[i][j] - A[ni][nj];
    }
}

long long NewSum(int** A, int N, int M){
    long long new_sum = 0;
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            new_sum += A[i][j];
        }
    }
    return new_sum;
}
```
## Разбор решения

1. Ввод размеров матрицы.  
   Пользователь вводит значения N и M. Ввод защищён от некорректных данных — при ошибке программа очищает поток и повторяет запрос.

2. Динамическое выделение памяти.  
   Создаётся двумерный массив через конструкцию:
   ```cpp
   int** Arr = new int*[N];
   Arr[i] = new int[M];
   ```
   Такой способ позволяет работать с матрицами переменного размера.

3. Безопасный ввод элементов матрицы.  
   Ввод каждого элемента происходит в цикле. Если пользователь вводит символы, буквы или некорректные значения, программа выводит предупреждение и запрашивает ввод повторно:
   ```cpp
   while (!(cin >> Arr[i][j])) { ... }
   ```

4. Вывод исходной матрицы.  
   Матрица выводится в табличном формате, что позволяет визуально убедиться в корректности ввода.

5. Вычисление суммы исходной матрицы.  
   Сумма считается отдельной функцией OriginalSum, проходящей по всем элементам:
   ```cpp
   sum += A[i][j];
   ```

6. Преобразование матрицы по правилу линейных разностей.  
   Матрица рассматривается как последовательность длиной N×M.  
   Каждый элемент заменяется разностью со следующим:
   ```cpp
   A[k] = A[k] - A[k+1];
   ```
   Последний элемент остаётся неизменным.  
   Итерация идёт вперёд, чтобы использовать только исходные значения.

7. Вывод преобразованной матрицы.  
   После преобразования программа выводит обновлённую матрицу построчно.

8. Вычисление суммы преобразованной матрицы.  
   Используется функция NewSum, проходящая по всей матрице:
   ```cpp
   new_sum += A[i][j];
   ```

9. Освобождение памяти.  
   Каждая строка удаляется отдельно, затем удаляется массив указателей:
   ```cpp
   delete[] Arr[i];
   delete[] Arr;
   ```
   Это предотвращает утечки памяти и завершает программу корректно.
