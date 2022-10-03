#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define newDBG_NEW


#include "Tokenizer.h"
#include "Expression.h"
#include "ExpressionBuilder.h"

#include <iostream>

int main()
{
    //const auto lines = ReadFile("Input.txt");
    //const auto lines = ReadFile("Custom_test_case.txt"); 
    const auto lines = ReadFile("Debug_test_case.txt"); 
    for (const auto& line : lines)
    {
        try
        {
            const auto tokens = Tokenize(line);
            if (tokens.size() == 0) continue; 
            const auto expression = BuildExpression(tokens);
            const auto tmp = expression->Compute(); 
             if (std::isnan(tmp) || std::isinf(tmp))
            {
                throw std::runtime_error("Result can not be nan or infinite"); // my add
            }
            //const auto result = expression->Compute();
            const auto result = tmp; 
            std::cout << "> " << line << std::endl;
            std::cout << "  = " << result << std::endl;
            std::cout << std::endl;
        }
        catch (const SyntaxError& e)
        {
            const std::string padding(e.GetOffset() + 2, ' ');
            std::cout << "> " << line << std::endl;
            std::cout << padding << "^ Error: " << e.what() << std::endl;
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "> " << line << std::endl;
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << std::endl;
        }
    }

    _CrtDumpMemoryLeaks();

    return 0;
}
