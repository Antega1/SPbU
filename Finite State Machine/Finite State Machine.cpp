//Красиков Андрей 23.Б16

#include <iostream>
#include <map>
#include <set>
#include <vector>

template<typename T>
class DFA {
private:
    std::set<int> states; // Множество состояний автомата
    std::set<T> alphabet; // Алфавит автомата
    std::map<std::pair<int, T>, int> transitionFunction; // Функция переходов
    int initialState; // Начальное состояние
    std::set<int> finalStates; // Множество конечных состояний

public:
    DFA() : initialState(-1) { }

    void addState(int state) {
        states.insert(state);
    }

    void setInitialState(int state) {
        if (states.find(state) != states.end()) {
            initialState = state;
        }
    }

    void addFinalState(int state) {
        if (states.find(state) != states.end()) {
            finalStates.insert(state);
        }
    }

    void addTransition(int fromState, T symbol, int toState) {
        if (states.find(fromState) != states.end() && states.find(toState) != states.end()) {
            transitionFunction[{fromState, symbol}] = toState;
            alphabet.insert(symbol);
        }
    }

    bool isAccepted(const std::vector<T>& inputString) {
        if (initialState == -1) return false; // Проверка наличия начального состояния

        int currentState = initialState;
        for (T symbol : inputString) {
            auto it = transitionFunction.find({currentState, symbol});
            if (it == transitionFunction.end()) {
                return false; // Возвращаем false, если символ не найден в функции переходов
            }
            currentState = it->second;
        }
        return finalStates.find(currentState) != finalStates.end();
    }
};

