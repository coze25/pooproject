#include <iostream>
#include <string>
#include <cmath>


using namespace std;

class operators_{
public:

    static bool isOperator(char op) {
        return op == '+' || op == '-' || op == '*' || op == '/' || op == '^' || op == '#';
    }

    static int getPrecedence(char op) {
        if (op == '^' || op == '#') {
            return 3;
        } else if (op == '*' || op == '/') {
            return 2;
        } else if (op == '+' || op == '-') {
            return 1;
        } 
        else {
            return 0;
        }
    };

    static bool hasHigherPrecedence(char op1, char op2){
        int precedence1 = getPrecedence(op1);
        int precedence2 = getPrecedence(op2);
        return precedence1 >= precedence2;
    };

    static void applyOperator(long double* numbers, int& n, char op) {
        if (n < 2) {
            throw invalid_argument("Nu sunt suficienți de termeni pentru operator");
        }

        long double operand2 = numbers[n-1];
        n--;

        long double operand1 = numbers[n-1];
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
    };

    ~operators_(){

    }
    

};


class Expression{
public:

string expression = "";
long double* numbers = new long double [expression.length()];
int n = 0;
char* operators = new char [expression.length()];
int o = 0;

    Expression(const string expression_): expression(expression_){
    }
    
    static long double evaluateExpression(Expression expressionC, const string& expression) {
        
        for (int i = 0; i < expression.size(); ++i) {
            if (isspace(expression[i])) {
                continue;
            } 
            else if (isdigit(expression[i]) || (expression[i] == '.' && i + 1 < expression.size() && isdigit(expression[i + 1]))) {
                int j = i;
                while (j < expression.size() && (isdigit(expression[j]) || expression[j] == '.')) {
                    ++j;
                }
                string numberStr = expression.substr(i, j - i);
                long double number = stold(numberStr);
                expressionC.numbers[expressionC.n] = number;
                expressionC.n++;
                i = j - 1;
            } 
            else if (operators_::isOperator(expression[i])) {
                while (expressionC.o!=0 && operators_::hasHigherPrecedence(expressionC.operators[expressionC.o -1], expression[i])) {
                    operators_::applyOperator(expressionC.numbers, expressionC.n, expressionC.operators[expressionC.o-1]);
                    expressionC.o--;
                }
                expressionC.operators[expressionC.o] = expression[i];
                expressionC.o++;
            } 
            else if (expression[i] == '(' || expression[i] == '[') {
                expressionC.operators[expressionC.o] = expression[i];
                expressionC.o++;
            } 
            else if (expression[i] == ')' || expression[i] == ']') {
                while (expressionC.o!=0 && expressionC.operators[expressionC.o-1] != '(' && expressionC.operators[expressionC.o-1] != '[') {
                    operators_::applyOperator(expressionC.numbers, expressionC.n, expressionC.operators[expressionC.o-1]);
                    expressionC.o--;
                }
                expressionC.o--;
            }
            else {
                throw invalid_argument("Caracter invalid in expresie");
            }
        }

        while (expressionC.o!=0) {
            operators_::applyOperator(expressionC.numbers, expressionC.n, expressionC.operators[expressionC.o-1]);
            expressionC.o--;
        }

        if (expressionC.n != 1) {
            throw invalid_argument("Expresie invalida");
        }

        long double result = expressionC.numbers[expressionC.n-1];

        return result;

    }

    /*
    ~Expression(){
        delete[] numbers;
        delete[] operators;
    };
    */

};


class Calculator {
public:

    void run() {
        string expression;
        while (true) {
            cout << "Introdu o expresie (scrie 'exit' pentru a termina): ";
            getline(cin, expression);

            if (expression == "exit") {
                break;
            }
            
            Expression expressionF(expression);

            try {
                long double result = Expression::evaluateExpression(expressionF, expression);
                cout << "Rezultat: " << result << endl;
            } 
            catch (const exception& e) {
                cerr << "Eroare: " << e.what() << endl;
            }

            //break;
        }
    }

    ~Calculator(){
        
    }

private:
   
    
};

int main(){

    Calculator calculatorFinal;
    calculatorFinal.run();
   

    return 0;
}