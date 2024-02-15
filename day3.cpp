#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

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


int main(int p_argv, char** p_argc) 
{
    std::fstream input_file;  
    input_file.open("example3.txt", std::ios::in);

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

        std::cout << "Part 1 result: " << resultPart1Sum << std::endl;
    }
    
    
    std::cout << "Part 2 result: " << resultPart2Sum << std::endl;

    input_file.close();

    return 0;
};
