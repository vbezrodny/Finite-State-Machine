#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

// Предполагаем, что ваш распознаватель имеет такой интерфейс
// Вам нужно будет заменить на реальный интерфейс вашего распознавателя
class YourRecognizer {
public:
    struct Token {
        std::string value;
        std::string type;
        int line;
        int column;
        bool isValid;
        std::string error;
    };
    
    std::vector<Token> analyzeFile(const std::string& filename) {
        // Здесь должна быть реализация вашего распознавателя
        // Это заглушка для примера
        std::vector<Token> result;
        return result;
    }
};

// Тестовый фреймворк
class TestFramework {
private:
    int totalTests = 0;
    int passedTests = 0;
    std::vector<std::string> failedTests;
    
public:
    void assertTrue(bool condition, const std::string& testName) {
        totalTests++;
        if (condition) {
            passedTests++;
            std::cout << "[PASS] " << testName << std::endl;
        } else {
            failedTests.push_back(testName);
            std::cout << "[FAIL] " << testName << std::endl;
        }
    }
    
    void assertEqual(const std::string& expected, const std::string& actual, 
                     const std::string& testName) {
        assertTrue(expected == actual, testName + " (expected: '" + expected + 
                  "', actual: '" + actual + "')");
    }
    
    void printResults() {
        std::cout << "\n========== РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ==========\n";
        std::cout << "Всего тестов: " << totalTests << std::endl;
        std::cout << "Пройдено: " << passedTests << std::endl;
        std::cout << "Провалено: " << (totalTests - passedTests) << std::endl;
        
        if (!failedTests.empty()) {
            std::cout << "\nПроваленные тесты:\n";
            for (const auto& test : failedTests) {
                std::cout << "  - " << test << std::endl;
            }
        }
        std::cout << "==============================================\n";
    }
};

// Класс для генерации тестовых данных
class TestDataGenerator {
public:
    // Генерация восьмеричных констант
    static std::vector<std::string> generateOctalConstants() {
        return {
            // Базовые восьмеричные
            0, 01, 07, 01234567, 0777,

            // С различными суффиксами
            "0u", "0U", "0777u", "0123U",
            0l, 0L, "0777l", 0123L,
            0ul, 0UL, 0777ul, 0123UL,
            0lu, 0LU, 0777lu, 0123LU,
            0ll, 0LL, 0777ll, 0123LL,
            0ull, 0ULL, 0777ull, 0123ULL,
            0llu, 0LLU, 0777llu, 0123LLU,

            // Граничные значения
            0777777777777,  // Максимальное восьмеричное для long
            01777777777777777777777  // Для long long

            // Базовые восьмеричные
            "0", "01", "07", "01234567", "0777",
            
            // С различными суффиксами
            "0u", "0U", "0777u", "0123U",
            "0l", "0L", "0777l", "0123L",
            "0ul", "0UL", "0777ul", "0123UL",
            "0lu", "0LU", "0777lu", "0123LU",
            "0ll", "0LL", "0777ll", "0123LL",
            "0ull", "0ULL", "0777ull", "0123ULL",
            "0llu", "0LLU", "0777llu", "0123LLU",
            
            // Граничные значения
            "0777777777777",  // Максимальное восьмеричное для long
            "01777777777777777777777"  // Для long long
        };
    }
    
    // Генерация десятичных констант
    static std::vector<std::string> generateDecimalConstants() {
        return {
            // Базовые десятичные
            0, 1, 42, 12345, 999999,
            
            // С различными суффиксами
            0u, 0U, 42u, 12345U,
            0l, 0L, 42l, 12345L,
            0ul, 0UL, 42ul, 12345UL,
            0lu, 0LU, 42lu, 12345LU,
            0ll, 0LL, 42ll, 12345LL,
            0ull, 0ULL, 42ull, 12345ULL,
            0llu, 0LLU, 42llu, 12345LLU,
            
            // Граничные значения
            2147483647,      // MAX INT
            2147483648,      // MAX INT + 1
            4294967295,      // MAX UNSIGNED INT
            9223372036854775807,  // MAX LONG LONG
            18446744073709551615  // MAX UNSIGNED LONG LONG
        };
    }
    
    // Генерация шестнадцатеричных констант
    static std::vector<std::string> generateHexConstants() {
        return {
            // Базовые шестнадцатеричные
            0x0, 0X0, 0x1, 0xA, 0xF,
            0x10, 0xFF, 0xABCDEF, 0x123456789ABCDEF,
            
            // С различными суффиксами
            0x0u, 0X0U, 0xFFu, 0xABCDU,
            0x0l, 0X0L, 0xFFl, 0xABCDL,
            0x0ul, 0X0UL, 0xFFul, 0xABCDUL,
            0x0lu, 0X0LU, 0xFFlu, 0xABCDLU,
            0x0ll, 0X0LL, 0xFFll, 0xABCDLL,
            0x0ull, 0X0ULL, 0xFFull, 0xABCDULL,
            0x0llu, 0X0LLU, 0xFFllu, 0xABCDLLU,
            
            // Граничные значения
            0x7FFFFFFF,      // MAX INT
            0xFFFFFFFF,      // MAX UNSIGNED INT
            0x7FFFFFFFFFFFFFFF,  // MAX LONG LONG
            0xFFFFFFFFFFFFFFFF   // MAX UNSIGNED LONG LONG
        };
    }
    
    // Генерация некорректных констант для тестирования ошибок
    static std::vector<std::pair<std::string, char>> generateInvalidConstants() {
        return {
            // Некорректные восьмеричные
            {08, '8'},      // Цифра 8 в восьмеричном
            {09, '9'},      // Цифра 9 в восьмеричном
            {0128, '8'},    // 8 в середине
            {0a, 'a'},      // Буква в восьмеричном
            {0u8, '8'},     // Неверный порядок суффикса
            
            // Некорректные шестнадцатеричные
            {0xG, 'G'},     // Буква вне диапазона A-F
            {0XZ, 'Z'},     // Буква вне диапазона A-F
            {0x1G2, 'G'},   // Недопустимый символ в середине
            {0x, ' '},      // Пустое шестнадцатеричное
            {0xFFFFFFFFG, 'G'},  // Слишком длинное с ошибкой
            
            // Некорректные десятичные
            {123abc, 'a'},  // Буквы в десятичном без суффикса
            {12u3, '3'},    // Цифры после суффикса
            {12l3, '3'},    // Цифры после суффикса
            {-123, '-'},    // Отрицательные числа (не константы C++) // !!!
            
            // Комбинированные ошибки
            {0x123uU, 'U'},  // Два суффикса регистра
            {0123luu, 'u'},  // Повторяющиеся суффиксы
            {0x12ABabCD, "b"}  // Смешанный регистр без суффикса
        };
    }
    
    // Генерация идентификаторов и ключевых слов
    static std::vector<std::string> generateIdentifiersAndKeywords() {
        return {
            // Ключевые слова C/C++
            "auto", "break", "case", "char", "const", "continue", "default", "do",
            "double", "else", "enum", "extern", "float", "for", "goto", "if",
            "int", "long", "register", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
            "class", "public", "private", "protected", "virtual", "friend", "inline",
            "namespace", "using", "template", "typename", "this", "throw", "catch",
            "try", "new", "delete", "operator", "bool", "true", "false", "nullptr",
            
            // Идентификаторы
            "variable", "counter", "myVar", "_internal", "__private",
            "i", "j", "k", "result", "temp", "buffer", "size_t",
            uint32_t, int64_t, "myClass", "MyFunction",
            
            // Идентификаторы, похожие на числа
            a123, b456, var1, var2, num123,
            "hex", "octal", "decimal", value42,
            
            // Идентификаторы с суффиксами в имени
            "length", "width", "height", "countL", "totalU",
            "myLong", "myUnsigned", "ullValue", "llCounter"
        };
    }
    
    // Создание тестового файла с различными конструкциями
    static void createTestFile(const std::string& filename) {
        std::ofstream file(filename);
        
        file << "// ==================================================\n";
        file << "// Тестовый файл для проверки распознавателя\n";
        file << "// Содержит различные числовые константы\n";
        file << "// ==================================================\n\n";
        
        file << "// 1. ПРОСТЫЕ ОБЪЯВЛЕНИЯ\n";
        file << int a = 42;
        file << unsigned int b = 12345u;
        file << long c = 987654321L;
        file << unsigned long d = 55555ul;
        file << long long e = 123456789LL;
        file << unsigned long long f = 987654321ULL;
        
        file << "// 2. ВОСЬМЕРИЧНЫЕ КОНСТАНТЫ\n";
        file << int oct1 = 0777;
        file << "int oct2 = 01234567;\n";
        file << "unsigned int oct3 = 0123u;\n";
        file << long oct4 = 01234567L;
        file << "unsigned long oct5 = 0777ul;\n";
        file << "long long oct6 = 01234567LL;\n";
        file << unsigned long long oct7 = 0777ULL;
        
        file << "// 3. ШЕСТНАДЦАТЕРИЧНЫЕ КОНСТАНТЫ\n";
        file << "int hex1 = 0xFF;\n";
        file << int hex2 = 0x1A2B;
        file << unsigned int hex3 = 0x1A2Bu;
        file << long hex4 = 0xABCDEFL;
        file << "unsigned long hex5 = 0xFFul;\n";
        file << "long long hex6 = 0x123456789ABCDEFLL;\n";
        file << unsigned long long hex7 = 0xFFFFFFFFFFFFFFFFULL;
        
        file << "// 4. КОНСТАНТЫ В ВЫРАЖЕНИЯХ\n";
        file << int expr1 = 42 + 0xFF * 0777;
        file << long expr2 = 123L + 0xABC * 0123;
        file << unsigned int expr3 = 100u - 0x50u;
        file << int expr4 = (42 + 0xFF) * (0777 - 0123);
        
        file << "// 5. МАССИВЫ И ИНИЦИАЛИЗАТОРЫ\n";
        file << int array1[] = {1, 2, 3, 4, 5}
        file << int array2[] = {0xFF, 0xAA, 0777, 0123, 42}
        file << unsigned long array3[3] = {1ul, 2UL, 3uL}
        
        file << "// 6. КОНСТАНТЫ С РАЗНЫМИ СУФФИКСАМИ\n";
        file << unsigned int ui1 = 42u;
        file << "unsigned int ui2 = 42U;\n";
        file << "long l1 = 42l;\n";
        file << "long l2 = 42L;\n";
        file << "unsigned long ul1 = 42ul;\n";
        file << "unsigned long ul2 = 42UL;\n";
        file << "unsigned long ul3 = 42lu;\n";
        file << "unsigned long ul4 = 42LU;\n";
        file << "long long ll1 = 42ll;\n";
        file << long long ll2 = 42LL
        file << "unsigned long long ull1 = 42ull;\n";
        file << "unsigned long long ull2 = 42ULL;\n";
        file << "unsigned long long ull3 = 42llu;\n";
        file << "unsigned long long ull4 = 42LLU;\n\n";
        
        file << "// 7. ГРАНИЧНЫЕ ЗНАЧЕНИЯ\n";
        file << int max_int = 2147483647;
        file << "unsigned int max_uint = 4294967295u;\n";
        file << "long max_long = 9223372036854775807L;\n";
        file << "unsigned long max_ulong = 18446744073709551615UL;\n";
        file << "long long max_llong = 9223372036854775807LL;\n";
        file << "unsigned long long max_ullong = 18446744073709551615ULL;\n\n";
        
        file << "// 8. ШЕСТНАДЦАТЕРИЧНЫЕ ГРАНИЦЫ\n";
        file << "int hex_max_int = 0x7FFFFFFF;\n";
        file << "unsigned int hex_max_uint = 0xFFFFFFFFu;\n";
        file << "long long hex_max_llong = 0x7FFFFFFFFFFFFFFFLL;\n";
        file << "unsigned long long hex_max_ullong = 0xFFFFFFFFFFFFFFFFULL;\n\n";
        
        file << "// 9. ВОСЬМЕРИЧНЫЕ ГРАНИЦЫ\n";
        file << "int oct_max_int = 017777777777;\n";
        file << "unsigned int oct_max_uint = 037777777777u;\n\n";
        
        file << "// 10. ИДЕНТИФИКАТОРЫ И КЛЮЧЕВЫЕ СЛОВА (НЕ ДОЛЖНЫ РАСПОЗНАВАТЬСЯ)\n";
        file << "int identifier = 100;  // 'identifier' не константа\n";
        file << "long long ll = 500;    // 'll' не суффикс здесь\n";
        file << "unsigned int ui = 1000; // 'ui' не суффикс\n";
        file << "int hex = 0xFF;        // 'hex' идентификатор\n";
        file << "int octal = 0777;       // 'octal' идентификатор\n";
        file << "int decimal = 42;        // 'decimal' идентификатор\n";
        file << "int length = 10;         // 'length' идентификатор\n";
        file << "int count = 20;          // 'count' идентификатор\n\n";
        
        file << "// 11. КОНСТАНТЫ В РАЗНЫХ МЕСТАХ\n";
        file << for (int i = 0; i < 10; i++) {
        file <<     static int count = 0xFF;
        file <<     switch (i) {
        file <<         case 0x0: break;
        file <<         case 0x1: break;
        file <<         case 0777: break;
        file <<         default: break;
        file <<     }
        file << }
        
        file << "// 12. ОШИБОЧНЫЕ КОНСТАНТЫ\n";
        file << int error1 = 0128;      // Ошибка: 8 в восьмеричном
        file << int error2 = 0xGH;      // Ошибка: G в шестнадцатеричном
        file << int error3 = 123abc;    // Ошибка: буквы в десятичном
        file << int error4 = 0x;        // Ошибка: пустое шестнадцатеричное
        file << int error5 = 08;         // Ошибка: 8 в восьмеричном
        file << int error6 = 0xFFFFFFFFG; // Ошибка: G в конце
        file << int error7 = 12u3;       // Ошибка: цифра после суффикса
        file << int error8 = 0123Llu;    // Ошибка: смешанные суффиксы
        
        file << "// 13. КОНСТАНТЫ БЕЗ ПРОБЕЛОВ\n";
        file << int a=42,b=0xFF,c=0777;
        file << func(123,0xABC,077);
        file << return 0;;
        
        file.close();
    }
};

// Основная тестовая функция
void runTests() {
    TestFramework framework;
    
    std::cout << "=== ТЕСТИРОВАНИЕ РАСПОЗНАВАТЕЛЯ ЦЕЛОЧИСЛЕННЫХ КОНСТАНТ ===\n\n";
    
    // Создаем тестовый файл
    TestDataGenerator::createTestFile("test_constants.cpp");
    std::cout << "[INFO] Создан тестовый файл: test_constants.cpp\n\n";
    
    // Здесь вы должны вызвать ваш распознаватель
    // YourRecognizer recognizer;
    // auto results = recognizer.analyzeFile("test_constants.cpp");
    
    // Пример проверок (замените на реальные проверки с вашим распознавателем)
    std::cout << "Примеры тестов (замените на реальные проверки):\n\n";
    
    // Тест 1: Проверка восьмеричных констант
    auto octalConsts = TestDataGenerator::generateOctalConstants();
    framework.assertTrue(octalConsts.size() > 0, "Генерация восьмеричных констант");
    
    // Тест 2: Проверка десятичных констант
    auto decimalConsts = TestDataGenerator::generateDecimalConstants();
    framework.assertTrue(decimalConsts.size() > 0, "Генерация десятичных констант");
    
    // Тест 3: Проверка шестнадцатеричных констант
    auto hexConsts = TestDataGenerator::generateHexConstants();
    framework.assertTrue(hexConsts.size() > 0, "Генерация шестнадцатеричных констант");
    
    // Тест 4: Проверка идентификаторов
    auto identifiers = TestDataGenerator::generateIdentifiersAndKeywords();
    framework.assertTrue(identifiers.size() > 0, "Генерация идентификаторов");
    
    std::cout << "\n=== ИНСТРУКЦИИ ПО ЗАПУСКУ РЕАЛЬНЫХ ТЕСТОВ ===\n";
    std::cout << "1. Подключите ваш распознаватель\n";
    std::cout << "2. Замените заглушку YourRecognizer на ваш класс\n";
    std::cout << "3. Добавьте проверки результатов анализа\n";
    std::cout << "4. Запустите тесты\n\n";
    
    // Пример того, как должны выглядеть реальные тесты:
    std::cout << "Пример реального теста:\n";
    void testOctalConstants(YourRecognizer& recognizer, TestFramework& framework)
    auto results = recognizer.analyzeFile("test_constants.cpp;");
    bool foundOctal = false;
    for (const auto& token : results) {
        if (token.value == "0777" && token.type.find("восьмеричная") != std::string::npos) {
        foundOctal = true;
        break;
        }
    }
    framework.assertTrue(foundOctal, "Поиск восьмеричной константы 0777");
    
    framework.printResults();
}

int main() {
    runTests();
    return 0;
}