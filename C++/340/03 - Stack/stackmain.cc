#include <iostream>
#include <iomanip>
#include <string>

#include "stack.h"

int main()
{
    Stack s;
    std::string op;
    int val = 0;

    std::cout << "operation -- size front end" << std::endl;
    std::cin >> op;
    while (!std::cin.eof())
    {
        if (op == "push")
        {
            std::cin >> val;
            s.push(val);
            std::cout << op << " " << val << "    -- ";
        }
        else if (op == "pop")
        {
            s.pop();
            std::cout << op << "       -- ";
        }
        else
        {
            std::cerr << "Error input: " << op << std::endl;
            return 1;
        }
        std::cout << std::setw(3) << s.size() << std::setw(5) << s.top() << std::endl;
        std::cin >> op;
    }

    while (!s.empty())
        s.pop();
    std::cout << "End -- size of Stack is: " << s.size() << std::endl;

    return 0;
}
