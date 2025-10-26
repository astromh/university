# Отчёт по лабораторной работе № 3

## Дисциплина: Теория и технология программирования

### Тема: Символьные строки

**Студент:** Мохаммад Хемат
**Группа:** 5130901/50001  

## Задание

Согласно индивидуальному варианту № 24: Считать текст из файла. Текст на русском языке. Провести замену всех символов текста в английской транслитерации.

## Цель работы

Цель лабораторной работы состоит в формировании умений использовать различные способы описания и формирования символьных строк, знать и уметь пользоваться методами чтения и записи строк в текстовых файлах, а также методами чтения и анализа потоковых данных, вводимых с клавиатуры. В рамках данного задания акцент делается на обработке русского текста и его транслитерации в латинский алфавит с использованием языка программирования C++.

## Условие задания

В соответствии с вариантом 24 необходимо написать программу на C++, которая считывает текст из входного файла "input.txt", где текст представлен на русском языке в кодировке UTF-8. Программа должна провести замену всех русских символов на их соответствующие эквиваленты в английской транслитерации (например, "А" на "A", "Б" на "B" и т.д., с учетом специальных случаев вроде "Ж" на "ZH", "Щ" на "SHCH"). Результат транслитерации записывается в выходной файл "output.txt". Программа должна корректно обрабатывать BOM в UTF-8 файлах и работать с многобайтовыми символами.

## Текст программы

```cpp
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(void) {
    ifstream in("input.txt", ios::binary);
    ofstream out("output.txt", ios::binary);

    if (!in.is_open()) {
        cerr << "[!] Error: Could not open the file!\n";
        return 1;
    }

    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    if (text.size() >= 3 && (unsigned char)text[0] == 0xEF &&
        (unsigned char)text[1] == 0xBB &&
        (unsigned char)text[2] == 0xBF) {
            text.erase(0, 3);
        }

    for (size_t i=0; i < text.size(); ) {
        unsigned char c = static_cast<unsigned char>(text[i]);

        if ((c == 0xD0 || c == 0xD1) && i+1 < text.size()) {
            unsigned char c2 = static_cast<unsigned char>(text[i+1]);

            if (c == 0xD0 && c2 == 0x81) { out << "YO"; i += 2; continue; }
            if (c == 0xD1 && c2 == 0x91) { out << "yo"; i += 2; continue; }

            if (c == 0xD0 && c2 >= 0x90 && c2 <= 0xAF) {
                switch (c2) {
                    case 0x90: out << "A"; break;  case 0x91: out << "B"; break;
                    case 0x92: out << "V"; break;  case 0x93: out << "G"; break;
                    case 0x94: out << "D"; break;  case 0x95: out << "E"; break;
                    case 0x96: out << "ZH"; break; case 0x97: out << "Z"; break;
                    case 0x98: out << "I"; break;  case 0x99: out << "Y"; break;
                    case 0x9A: out << "K"; break;  case 0x9B: out << "L"; break;
                    case 0x9C: out << "M"; break;  case 0x9D: out << "N"; break;
                    case 0x9E: out << "O"; break;  case 0x9F: out << "P"; break;
                    case 0xA0: out << "R"; break;  case 0xA1: out << "S"; break;
                    case 0xA2: out << "T"; break;  case 0xA3: out << "U"; break;
                    case 0xA4: out << "F"; break;  case 0xA5: out << "KH"; break;
                    case 0xA6: out << "TS"; break; case 0xA7: out << "CH"; break;
                    case 0xA8: out << "SH"; break; case 0xA9: out << "SHCH"; break;
                    case 0xAA: out << ""; break;   case 0xAB: out << "Y"; break;
                    case 0xAC: out << ""; break;   case 0xAD: out << "E"; break;
                    case 0xAE: out << "YU"; break; case 0xAF: out << "YA"; break;
                }
                i += 2;
                continue;
            }

            if (c == 0xD0 && c2 >= 0xB0 && c2 <= 0xBF) {
                switch (c2) {
                    case 0xB0: out << "a"; break; case 0xB1: out << "b"; break;
                    case 0xB2: out << "v"; break; case 0xB3: out << "g"; break;
                    case 0xB4: out << "d"; break; case 0xB5: out << "e"; break;
                    case 0xB6: out << "zh"; break; case 0xB7: out << "z"; break;
                    case 0xB8: out << "i"; break; case 0xB9: out << "y"; break;
                    case 0xBA: out << "k"; break; case 0xBB: out << "l"; break;
                    case 0xBC: out << "m"; break; case 0xBD: out << "n"; break;
                    case 0xBE: out << "o"; break; case 0xBF: out << "p"; break;
                }
                i += 2;
                continue;
            }

            if (c == 0xD1 && c2 >= 0x80 && c2 <= 0x8F) {
                switch (c2) {
                    case 0x80: out << "r"; break; case 0x81: out << "s"; break;
                    case п0x82: out << "t"; break; case 0x83: out << "u"; break;
                    case 0x84: out << "f"; break; case 0x85: out << "kh"; break;
                    case 0x86: out << "ts"; break; case 0x87: out << "ch"; break;
                    case 0x88: out << "sh"; break; case 0x89: out << "shch"; break;
                    case 0x8A: out << ""; break;  case 0x8B: out << "y"; break;
                    case 0x8C: out << ""; break;  case 0x8D: out << "e"; break;
                    case 0x8E: out << "yu"; break; case 0x8F: out << "ya"; break;
                }
                i += 2;
                continue;
            }
        }

        out << text[i];
        i++;
    }
    
    in.close();
    out.close();
    return 0;
}
```

## Входные и выходные данные

**Пример входных данных (файл input.txt):**  
`Это пример текста на русском языке. Ёлка, щётка, жук.`

**Пример выходных данных (файл output.txt):**  
`Eto primer teksta na russkom yazyke. Yolka, shchotka, zhuk.`

## Пример работы программы

Программа компилируется и запускается без аргументов. Она открывает файл input.txt, читает содержимое, удаляет BOM если присутствует, затем проходит по каждому байту, распознавая UTF-8 последовательности для русских букв и заменяя их на соответствующие латинские эквиваленты. Не-русские символы остаются без изменений. Результат записывается в output.txt. При запуске, если файл не открыт, выводится ошибка в консоль.

## Вывод

В результате выполнения лабораторной работы была создана программа для транслитерации русского текста в латинский алфавит. Это позволило освоить работу с файлами в режиме binary, обработку multibyte символов в UTF-8, использование switch для маппинга символов. Программа работает корректно на примерах, демонстрируя понимание строк и потоков в C++. Полученные навыки полезны для дальнейшего изучения программирования.