#include <iostream>
#include <string>
#include <cmath>


using namespace std;

class Calculator {
public:
    long double evaluateExpression(const string& expression) {
        int* numbers = new int [expression.length()];
        int n = 0;
        string* operators = new string [expression.length()];
        int o = 0;

        for (size_t i = 0; i < expression.size(); ++i) {
            if (isspace(expression[i])) {
                continue;
            } 
            else if (isdigit(expression[i]) || (expression[i] == '.' && i + 1 < expression.size() && isdigit(expression[i + 1]))) {
                size_t j = i;
                while (j < expression.size() && (isdigit(expression[j]) || expression[j] == '.')) {
                    ++j;
                }
                string numberStr = expression.substr(i, j - i);
                long double number = stold(numberStr);
                numbers[n] = number;
                n++;
                i = j - 1;
            } 
            else if (isOperator(expression[i])) {
                while (o!=0 && hasHigherPrecedence(operators[o-1], expression[i])) {
                    applyOperator(numbers, n, operators[o-1]);
                    operators[o-1] = nullptr;
                    o--;
                }
                //operators.push(expression[i]);
                operators[o] = expression[i];
                o++;
            } 
            else if (expression[i] == '(' || expression[i] == '[') {
                //operators.push(expression[i]);
                operators[o] = expression[i];
                o++;
            } 
            else if (expression[i] == ')' || expression[i] == ']') {
                while (o!=0 && operators[o-1] != '(' && operators[o-1] != '[') {
                    applyOperator(numbers, n, operators[o-1]);
                    operators[o-1] = nullptr;
                    o--;
                }
                operators[o-1] = nullptr;
                o--;
            } 
            else {
                throw invalid_argument("Caracter invalid in expresie");
            }
        }

        while (o!=0) {
            applyOperator(numbers, n, operators[o-1]);
            operators[o-1] = nullptr;
            o--;
        }

        if (n != 1) {
            throw invalid_argument("Expresie invalida");
        }

        return numbers[n-1];
    }

    void run() {
        string expression;
        while (true) {
            cout << "Introdu o expresie (scrie 'exit' pentru a termina): ";
            getline(cin, expression);

            if (expression == "exit") {
                break;
            }

            try {
                long double result = evaluateExpression(expression);
                cout << "Rezultat: " << result << endl;
            } 
            catch (const exception& e) {
                cerr << "Eroare: " << e.what() << endl;
            }

            break;
        }
    }

private:
    bool isOperator(string opS) {
        char op;
        op = opS[0];
        return op == '+' || op == '-' || op == '*' || op == '/' || op == '^' || op == '#';
    }

    int getPrecedence(string opS) {
        char op;
        op = opS[0];
        if (op == '^' || op == '#') {
            return 3;
        } else if (op == '*' || op == '/') {
            return 2;
        } else if (op == '+' || op == '-') {
            return 1;
        } else {
            return 0; // Lowest precedence for non-operators
        }
    }

    bool hasHigherPrecedence(string opS1, char opS2){
        char op1, op2;
        op1 = opS1[0];
        op2 = opS2[0];
        int precedence1 = getPrecedence(op1);
        int precedence2 = getPrecedence(op2);
        return precedence1 >= precedence2;
    }

    void applyOperator(int* numbers, int n, string opS) {
        if (n < 2) {
            throw invalid_argument("Nu sunt suficienți de termeni pentru operator");
        }

        char op;
        op = opS[0];

        long double operand2 = numbers[n-1];
        numbers[n-1] = -1;
        n--;

        long double operand1 = numbers[n-1];
        numbers[n-1] = -1;
        n--;

        long double result;
        switch (op) {
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                if (operand2 == 0) {
                    throw invalid_argument("Impartire la 0");
                }
                result = operand1 / operand2;
                break;
            case '^':
                result = pow(operand1, operand2);
                break;
            case '#':
                result = pow(operand1, 1.0 / operand2);
                break;
            default:
                throw invalid_argument("Operator invalid");
        }

        numbers[n] = result;
        n++;
    }
};

int main(){

    Calculator calculatorFinal;
    calculatorFinal.run();
    

    return 0;
}