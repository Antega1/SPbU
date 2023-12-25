//Красиков Андрей 23.Б16

#include <iostream>
#include <stack>
#include <string>

enum class State {
    VALID,
    INVALID
};

class BracketAutomaton {
private:
    std::stack<char> bracketStack;
    State currentState;

public:
    BracketAutomaton() : currentState(State::VALID) {}

    // Обработка одного символа
    void processChar(char ch) {
        switch (ch) {
            // Если символ - открывающая скобка, добавляем её в стек
            case '(': case '[': case '{':
                bracketStack.push(ch);
                break;
            // Если символ - закрывающая скобка
            default:
                // Проверка на пустоту стека и соответствие скобок
                if (bracketStack.empty() || 
                    (ch == ')' && bracketStack.top() != '(') ||
                    (ch == ']' && bracketStack.top() != '[') ||
                    (ch == '}' && bracketStack.top() != '{')) {
                    currentState = State::INVALID;
                    return; // Прекращаем обработку при обнаружении ошибки
                }
                bracketStack.pop();
        }
    }

    // Проверка на валидность скобочной последовательности
    bool isValid() const {
        return currentState == State::VALID && bracketStack.empty();
    }
};

int main() {
    BracketAutomaton automaton;
    std::string input;
    std::cin >> input;

    for (char ch : input) {
        automaton.processChar(ch);
        if (automaton.isValid() == false) {
            break; // Прекращаем чтение, если последовательность невалидна
        }
    }

    std::cout << (automaton.isValid() ? "Valid\n" : "Invalid\n");

    return 0;
}
