#include "jmp.hpp"

template <typename T> class Stack
{
    private:
        T *stack;
        long long int top;
        unsigned long long int capacity;

    public:
        Stack (unsigned long long int c) : capacity{c}
        {
            stack = new T[capacity];
            top = -1;
        }

        ~Stack()
        {
            delete[] stack;
        }

        bool is_empty() const
        {
            return top == -1;
        }

        bool is_full() const
        {
            return top == capacity;
        }

        T push(T val)
        {
            return stack[++top] = val;
        }

        T pull()
        {
            T NullVal;
            if (!is_empty())
                return stack[top--];
            return NullVal;
        }

        T at(unsigned long long int i) const
        {
            return stack[i];
        }

        T at_top() const
        {
            return stack[top];
        }

        unsigned long long int size() const
        {
            return capacity;
        }

        void clear()
        {
            top = -1;
            for (int i=0; i<capacity; i++)
                stack[i] = 0;
        }

        long long int top_val()
        {
            return top;
        }

};

namespace stack
{
    int prec(char ch)
    {
        switch (ch)
        {
            case '+': case '-': return 1; break;
            case '*': case '/': case '%': return 2; break;
            case '^': case 's': case 'a': return 3;
        }
        return -1;
    }

    std::string infix2postfix(std::string& s)
    {
        Stack<char> st(s.size());
        std::string ans = "";

        for (int i=0; i<s.length(); i++)
        {
            char ch = s[i];

            if ((ch == '+' || ch == '-') && i == 0)
                ans += ch;

            if (ch >= '0' && ch <= '9' || ch == '.')
            {
                while (ch >= '0' && ch <= '9' || ch == '.')
                {
                    ans += ch;
                    if (i<s.length())
                        ch = s[++i];
                    else
                        break;
                }
                ans += '$';
            }

            if (ch == '(')
                st.push('(');
            else if (ch == ')') {
                while (st.at_top() != '(')
                {
                    ans += st.at_top();
                    st.pull();
                }
                st.pull();
            } else {
                while (!st.is_empty() && prec(s[i]) <= prec(st.at_top())) {
                    ans += st.at_top();
                    st.pull();
                }
                st.push(ch);
            }
        }

        while (!st.is_empty())
        {
            ans += st.at_top();
            st.pull();
        }
        return ans;
    }

    jmp calculate_postfix(std::string& s)
    {
        jmp a, b;
        Stack<jmp> st(s.size());

        for (unsigned long long int i=0; s[i]; ++i)
        {
            std::string number;
            while (s[i] != '$' && s[i] != '+' && s[i] != '-' && s[i] != '*' && s[i] != '/' && s[i] != '%' && s[i] != '^' && s[i] != 's' && s[i] != 'a')
                number.push_back(s[i++]);
            if (!number.empty())
                st.push(number);

            if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%' || s[i] == '^' || s[i] == 's' || s[i] == 'a')
            {
                if (s[i] == 's' || s[i] == 'a')
                    a = st.pull();
                else {
                    a = st.pull();
                    b = st.pull();
                }
                b.set_division_precision(20);
                switch (s[i])
                {
                    case '+': st.push(b + a); break;
                    case '-': st.push(b - a); break;
                    case '*': st.push(b * a); break;
                    case '/': st.push(b / a); break;
                    case '%': st.push(b % a); break;
                    case '^': st.push(JMP::pow(b, a)); break;
                    case 's': st.push(JMP::sqrt(a)); break;
                    case 'a': st.push(JMP::abs(a)); break;
                }
            }
        }
        return st.pull();
    }
}
