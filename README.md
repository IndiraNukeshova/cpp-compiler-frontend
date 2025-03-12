# cpp-compiler-frontend
Файл `lexer.hpp` содержит реализацию лексического анализатора, разбивающего входной текст на токены. Он определяет правила распознавания различных типов токенов, таких как ключевые слова, операторы и идентификаторы, а затем преобразует входные строки в последовательность токенов для последующего синтаксического анализа.  

Файл `dictionary.hpp` представляет собой структуру данных для хранения и поиска элементов в словаре, где каждому ключу соответствует значение. Он используется для быстрого доступа к данным, например, для хранения таблицы идентификаторов или ключевых слов, применяемых в компиляторе или интерпретаторе.  

Файл `main.cpp` служит точкой входа в программу, использующую лексический анализатор. Он принимает ввод от пользователя, передает его в `Lexer::parse()`, получает список токенов и выводит их на экран. Этот файл связывает лексический анализ с дальнейшей обработкой данных.  

Файл `alphabet.hpp` содержит описание алфавита языка, включая допустимые символы и их классификацию. Он используется лексическим анализатором для определения валидных токенов и их типов при разборе входного текста.  

Файл `syntaxer (4).hpp` реализует синтаксический анализатор, который на основе полученных токенов проверяет корректность их последовательности согласно грамматике языка. Он строит синтаксическое дерево и выявляет возможные ошибки в структуре кода.  

Файл `tree (4).hpp` описывает структуру синтаксического дерева, используемого в процессе разбора программы. Оно представлено в виде узлов, соответствующих операторам и выражениям, что позволяет эффективно анализировать и трансформировать код.  

---

The `lexer.hpp` file implements a lexical analyzer that breaks the input text into tokens. It defines rules for recognizing different token types, such as keywords, operators, and identifiers, and then converts input strings into a sequence of tokens for further syntactic analysis.  

The `dictionary.hpp` file provides a data structure for storing and retrieving key-value pairs. It is used for quick access to data, such as storing identifier tables or keywords used in a compiler or interpreter.  

The `main.cpp` file serves as the entry point for the program that utilizes the lexical analyzer. It takes user input, passes it to `Lexer::parse()`, retrieves a list of tokens, and prints them. This file connects lexical analysis with further data processing.  

The `alphabet.hpp` file defines the alphabet of the language, including valid characters and their classification. It is used by the lexical analyzer to determine valid tokens and their types when parsing input text.  

The `syntaxer (4).hpp` file implements a syntax analyzer that checks whether the sequence of tokens follows the language’s grammar. It constructs a syntax tree and detects possible structural errors in the code.  

The `tree (4).hpp` file describes the structure of the syntax tree used in parsing programs. It represents nodes corresponding to operators and expressions, enabling efficient analysis and transformation of code.
