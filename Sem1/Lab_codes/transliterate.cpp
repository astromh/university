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
                    case 0xB6: out << "zh"; break;case 0xB7: out << "z"; break;
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
                    case 0x82: out << "t"; break; case 0x83: out << "u"; break;
                    case 0x84: out << "f"; break; case 0x85: out << "kh"; break;
                    case 0x86: out << "ts"; break;case 0x87: out << "ch"; break;
                    case 0x88: out << "sh"; break;case 0x89: out << "shch"; break;
                    case 0x8A: out << ""; break;  case 0x8B: out << "y"; break;
                    case 0x8C: out << ""; break;  case 0x8D: out << "e"; break;
                    case 0x8E: out << "yu"; break;case 0x8F: out << "ya"; break;
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