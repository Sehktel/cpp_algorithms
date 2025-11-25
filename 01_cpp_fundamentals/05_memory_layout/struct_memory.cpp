#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

struct CELL {   // Declare CELL bit field
    unsigned short character  : 8;  // 00000000 ????????
    unsigned short foreground : 3;  // 00000??? 00000000
    unsigned short intensity  : 1;  // 0000?000 00000000
    unsigned short background : 3;  // 0???0000 00000000
    unsigned short blink      : 1;  // ?0000000 00000000
} ;

int main(void){
    CELL orange;
    orange.character = 'g';
    orange.foreground = 6;
    orange.intensity = false;
    orange.background = 3;
    orange.blink = true; 
    
    // Вывод исходных значений
    cout << "Исходные значения структуры:" << endl;
    cout << "orange.character = "  <<  orange.character << endl;
    cout << "orange.foreground = " << orange.foreground << endl;
    cout << "orange.intensity = "  << orange.intensity  << endl;
    cout << "orange.background = " << orange.background << endl;
    cout << "orange.blink = "      << orange.blink      << endl; 
    
    CELL *pointer = (&orange);

    // Преобразование в unsigned short для битовых операций
    unsigned short *rawValue = reinterpret_cast<unsigned short*>(pointer);

    // Вывод исходного hex значения
    cout << "\nИсходное hex значение: 0x" 
         << std::hex << std::uppercase << *rawValue << std::dec << endl;

    // Побайтовое представление для наглядности
    unsigned char* bytes = reinterpret_cast<unsigned char*>(pointer);
    cout << "\nПобайтовое представление:" << endl;
    cout << "Байт 1 (мл.): 0x" << std::hex << static_cast<int>(bytes[0]) << endl;
    cout << "Байт 2 (ст.): 0x" << std::hex << static_cast<int>(bytes[1]) << std::dec << endl;

    // Битовая операция OR для установки 6 и 7 бит ПЕРВОГО байта в 1
    // Маска: 0b11000000 (0xC0)
    bytes[0] |= 0xC0;

    // Битовая операция AND для обнуления 6 и 3 бит ВТОРОГО байта
    // Маска: 0b10111011 (0xBB)
    bytes[1] &= 0xBB;

    /*

    Before OR:  [ABCDEFGH][IJKLMNOP]
    OR Mask:    [11000000][--------]
    After OR:   [11CDEFGH][IJKLMNOP]
 
    Before AND: [--------][IJKLMNOP]
    AND Mask:   [--------][10111011]
    After AND:  [--------][I0KLM0OP]

    */

    // Вывод измененных байтов
    cout << "\nИзмененные байты:" << endl;
    cout << "Байт 1 (мл.): 0x" << std::hex << static_cast<int>(bytes[0]) << endl;
    cout << "Байт 2 (ст.): 0x" << std::hex << static_cast<int>(bytes[1]) << std::dec << endl;

    // Вывод hex значения для проверки
//    unsigned short *rawValue = reinterpret_cast<unsigned short*>(pointer);
    cout << "\nИзмененное hex значение: 0x" 
         << std::hex << std::uppercase << *rawValue << std::dec << endl;

    // Вывод измененных значений структуры
    cout << "\nИзмененные значения структуры:" << endl;
    cout << "orange.character = "  <<  orange.character << endl;
    cout << "orange.foreground = " << orange.foreground << endl;
    cout << "orange.intensity = "  << orange.intensity  << endl;
    cout << "orange.background = " << orange.background << endl;
    cout << "orange.blink = "      << orange.blink      << endl; 

    // Метод 2: Использование stringstream для точного форматирования
    cout << "\nМетод 2 - Преобразование через stringstream:" << endl;
    std::stringstream ss;
    ss << std::hex << std::setw(4) << std::setfill('0') << *rawValue;
    std::string formattedHex = ss.str();
    cout << "Форматированное hex значение: 0x" << formattedHex << endl;

    return 0;
}