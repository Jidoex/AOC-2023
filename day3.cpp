#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <optional>

// DAY 3


bool isANumber(char p_character)
{
    return (p_character >= '0') && (p_character <= '9');
}

bool isASymbol(char p_character)
{
    bool itIs = (!isANumber(p_character)) && ('.' != p_character);

    static std::string knownSymbol = "*$/&@+=\\_#%-";

    if(itIs &&  (-1 == knownSymbol.find(p_character)))
    {
        std::cerr <<  "Strange char " << p_character << std::endl;
    }

    return itIs;
}

bool hasSymbolAtIndex(int p_charIndex, const std::string& p_previousLine, const std::string& p_currentLine, const std::string& p_nextLine)
{
    bool shouldTreatPreviousLine = (p_previousLine.size() != 0);
    bool shouldTreatNextLine = (p_nextLine.size() != 0);

    if (isASymbol(p_currentLine[p_charIndex]))
    {
        return true;
    }
    if (shouldTreatPreviousLine && isASymbol(p_previousLine[p_charIndex]))
    {
        return true;
    }
    if (shouldTreatNextLine && isASymbol(p_nextLine[p_charIndex]))
    {
        return true;
    }
    return false;
}

bool hasSymbolClose(int p_charIndex, const std::string& p_previousLine, const std::string& p_currentLine, const std::string& p_nextLine)
{
    // Check on the left
    if((p_charIndex>0) && (hasSymbolAtIndex(p_charIndex-1, p_previousLine, p_currentLine,p_nextLine)))
    {
        return true;
    }

    // Check on the right
    if((p_charIndex<(p_currentLine.size()-1)) && (hasSymbolAtIndex(p_charIndex+1,p_previousLine,p_currentLine,p_nextLine)))
    {
        return true;
    }

    // Check on the top
    bool shouldTreatPreviousLine = (p_previousLine.size() != 0);

    if (shouldTreatPreviousLine && isASymbol(p_previousLine[p_charIndex]))
    {
        return true;
    }

    // Check at the bottom
    bool shouldTreatNextLine = (p_nextLine.size() != 0);
    if (shouldTreatNextLine && isASymbol(p_nextLine[p_charIndex]))
    {
        return true;
    }
    return false;
}

std::optional<std::pair<int, std::pair<int, int>>> getNumber(int char_index, const std::string line)
{
    int number = 0;
    int start_index = char_index;
    int end_index = char_index;

    if((char_index < 0) || (char_index >= line.size()) || !isANumber(line[char_index]))
    {
        return std::nullopt;
    }

    while((end_index < line.size() - 1) && isANumber(line[end_index + 1]))
    {
        end_index++;
    }

    while((start_index > 0) && isANumber(line[start_index - 1]))
    {
        start_index--;
    }

    number = std::stoi(line.substr(start_index, end_index - start_index + 1));

    return {{number, {start_index, end_index}}};
}

int closeNumberResult(int p_gearIndex, const std::string& p_previousLine, const std::string& p_currentLine, const std::string& p_nextLine)
{
    // Check on the top
    bool shouldTreatPreviousLine = (p_previousLine.size() != 0);
    std::vector<int> result_vector;

    if (shouldTreatPreviousLine)
    {
        auto result1 = getNumber(p_gearIndex - 1, p_previousLine);
        auto result2 = getNumber(p_gearIndex, p_previousLine);
        auto result3 = getNumber(p_gearIndex + 1, p_previousLine);
        if(result1.has_value() && result2.has_value() && result3.has_value())
        {
            result_vector.push_back(result1.value().first);
        } 
        else if (!result2.has_value())
        {
            if(result1.has_value())
            {
                result_vector.push_back(result1.value().first);
            }
            if(result3.has_value())
            {
                result_vector.push_back(result3.value().first);
            }
        } 
        else 
        {
            result_vector.push_back(result2.value().first);
        }
    }

    // Check at the bottom
    bool shouldTreatNextLine = (p_nextLine.size() != 0);

    if (shouldTreatNextLine)
    {
        auto result1 = getNumber(p_gearIndex - 1, p_nextLine);
        auto result2 = getNumber(p_gearIndex, p_nextLine);
        auto result3 = getNumber(p_gearIndex + 1, p_nextLine);
        if(result1.has_value() && result2.has_value() && result3.has_value())
        {
            result_vector.push_back(result1.value().first);
        } 
        else if (!result2.has_value())
        {
            if(result1.has_value())
            {
                result_vector.push_back(result1.value().first);
            }
            if(result3.has_value())
            {
                result_vector.push_back(result3.value().first);
            }
        } 
        else 
        {
            result_vector.push_back(result2.value().first);
        }
    }

    // Check on the left
    if((p_gearIndex > 0) && isANumber(p_currentLine[p_gearIndex - 1]))
    {
        result_vector.push_back(getNumber(p_gearIndex - 1, p_currentLine).value().first);
    }

    // Check on the right
    if((p_gearIndex < (p_currentLine.size() - 1)) && isANumber(p_currentLine[p_gearIndex + 1]))
    {
        result_vector.push_back(getNumber(p_gearIndex + 1, p_currentLine).value().first);
    }
    
    std::for_each(result_vector.begin(), result_vector.end(), [](const auto & p_elem){std::cout << p_elem << " ,";});
    std::cout << std::endl ;

    return (result_vector.size() != 2) ? 0 : result_vector[0] * result_vector[1];
}

int main(int p_argv, char** p_argc) 
{

    // std::cout << closeNumberResult(3, "467..114..", "...*......", "..35..633.") << std::endl;
    // return 0;

    std::fstream input_file;  
    input_file.open("input_day3.txt", std::ios::in);

    std::cout << std::boolalpha;
    std::cout << "Day 3, lets go!" << std::endl;

    int resultPart1Sum = 0;
    int resultPart2Sum = 0;
    // part 1
    {
        std::string previousLine;
        std::string currentLine;
        std::string nextLine;
        std::getline(input_file, currentLine);
        std::getline(input_file, nextLine);

        bool lastLineWasParsed =false;
        bool shouldExit =false;

        while(true)
        {   
            // Process only current line
            {
                bool wasInNumber =false;
                bool hasCloseSymbol =false;
                std::string numberText;

                for(int index=0; index < currentLine.size(); index++)
                {
                    if(isANumber(currentLine[index]))
                    {
                        if(!wasInNumber)
                        {
                            wasInNumber = true;
                        }
                        hasCloseSymbol|=hasSymbolClose(index,previousLine,currentLine,nextLine);
                        numberText+=currentLine[index];
                        if(index == (currentLine.size()-1))
                        {
                            resultPart1Sum+= hasCloseSymbol? std::stoi(numberText) : 0;;
                        }
                    }
                    else if (wasInNumber )
                    {
                        if(hasCloseSymbol)
                        {
                            resultPart1Sum+= std::stoi(numberText);
                        }
                        
                        hasCloseSymbol=false;
                        wasInNumber=false;
                        numberText="";
                    }
                    else
                    {
                        // Previous char was not a number
                    }
                }
            }

            previousLine=currentLine;
            currentLine=nextLine;

            if(shouldExit)
            {
                break;
            }


            if(!std::getline(input_file, nextLine))
            {
                shouldExit=true;
            }
        } 
    }
    
    input_file.close();
    input_file.open("input_day3.txt", std::ios::in);
    std::cout << "Part 2, lets go!" << std::endl;

    // part 2
    {
        std::string previousLine;
        std::string currentLine;
        std::string nextLine;
        std::getline(input_file, currentLine);
        std::getline(input_file, nextLine);

        bool lastLineWasParsed =false;
        bool shouldExit =false;

        int i = 0;
        while(true)
        {   
            std::cout << "line " << i++ << " : ";

            // Process only current line
            {
                std::string numberText;

                for(int index=0; index < currentLine.size(); index++)
                {
                    if(currentLine[index] == '*')
                    {
                        resultPart2Sum += closeNumberResult(index, previousLine, currentLine, nextLine);
                    }
                }
            }

            previousLine=currentLine;
            currentLine=nextLine;

            if(shouldExit)
            {
                break;
            }


            if(!std::getline(input_file, nextLine))
            {
                shouldExit=true;
            }

            std::cout << std::endl;
        }

        input_file.close();


    }
    
    std::cout << "Part 1 result: " << resultPart1Sum << std::endl;
    std::cout << "Part 2 result: " << resultPart2Sum << std::endl;


    return 0;
};
