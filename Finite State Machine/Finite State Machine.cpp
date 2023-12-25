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

    void processChar(char ch) {
        if (ch == '(' || ch == '[' || ch == '{') {
            bracketStack.push(ch);
        } else {
            if (bracketStack.empty()) {
                currentState = State::INVALID;
                return;
            }

            char top = bracketStack.top();
            bracketStack.pop();

            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                currentState = State::INVALID;
            }
        }
    }

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
    }

    if (automaton.isValid()) {
        std::cout << "Valid\n";
    } else {
        std::cout << "Invalid\n";
    }

    return 0;
}
