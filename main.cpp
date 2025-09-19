#include<bits/stdc++.h>
using namespace std;

optional<int>stringToNumber(const string &s)
{
    if(s.empty())
        return nullopt;
    // stoi can throw errors, because it can throw error, that needs to be handled gracefully
    try {
        return stoi(s);
    } catch (...) {//basically catch any error
        return nullopt;
    }
}



optional<int> evaluateLeftToRight( const vector<string>&tokens)
{
    if(tokens.size()==0 )
        return nullopt;


    // I handle the case where the first number is a "-" sign, rest cases of +, * , / are invalid
    if(tokens.size() >= 2 && tokens[0] == "-") {
        
        auto num = stringToNumber(tokens[1]);
        if(num && tokens.size() == 2) {
            return -(*num);  
        }
        
        
        if(num) {
            vector<string> newTokens;
            newTokens.push_back(to_string(-(*num)));  
            for(int i = 2; i < tokens.size(); i++) {
                newTokens.push_back(tokens[i]);
            }
            return evaluateLeftToRight(newTokens);  
        }
    }




    auto result = stringToNumber(tokens[0]);
    if(!result)
        return nullopt;


    // now we have the format of ( "2" +"3"-"2"), here basically the ith position is the number and the i+1th is the operation that i need to do
    for(int i =1;i<tokens.size()-1;i+=2)
    {
        auto num = stringToNumber(tokens[i+1]);
        string operation = tokens[i];

        if(!num)
        return nullopt;

        if(operation=="+") *result += *num;
        else if(operation == "-") *result -= *num;
        else if(operation == "*") *result *= *num;
        else if(operation == "/")
        {
            if(*num == 0)
                return nullopt;
            *result /=*num;

        }

        else return nullopt; // This is important as there might be invalid operations;
    }
    return result;

}

// One assumption I had made was that the expression inside the bracket would also be evaluated left ot right, so i just find out he inner most brackets and evaluate
optional<int>evaluateBrackets(const vector<string> &expression)
{
    vector<string>tokens = expression;
    while(true)
    {
        int openPosition = -1, closePosition = -1;
        bool foundFirst = false;
        for(int i =0;i<tokens.size();i++)
        {
            if(tokens[i]=="(")
            {
                openPosition = i;
                foundFirst = true;
            }
                
            if(tokens[i]==")")
            {
                if(foundFirst==false)
                    return nullopt;
                
                closePosition = i;
                break;
            }
        }
        if(openPosition == -1 && closePosition == -1 && foundFirst ==false)
            break;
        
        // these cases check if the brackets are misplaced or have closed before opening or have opened and not closed at all 

        if(openPosition == -1 && closePosition!=-1)
            return nullopt;
        if(openPosition !=-1 && closePosition == -1)
            return nullopt;
        

        vector<string>innerTokens;

        for(int i = openPosition+1 ; i<closePosition;i++)
            innerTokens.push_back(tokens[i]);
        
        if(innerTokens.size()==0)
            return nullopt;


        
        auto innerResult = evaluateLeftToRight(innerTokens);
        
 
        if(!innerResult)
            return nullopt;

        vector<string>newTokens;


        // I just replace the innermost bracket with its result like (2+(3+4))=(2+7), continuing till there are no brackets
        for(int i =0;i<openPosition;i++)
            newTokens.push_back(tokens[i]);
        
        newTokens.push_back(to_string(*innerResult));
        
        for(int i = closePosition+1;i<tokens.size();i++)
            newTokens.push_back(tokens[i]);

        tokens  = newTokens;


    }
    // this is the final evalution , as I might be left with something like 2+8+10, which as no brackets but needs to be evaluated
    return evaluateLeftToRight(tokens);
}

vector<string> tokenize(string const &expression)
{
    vector<string>tokens;

    string currrentCharacter = "";
    for(int i =0;i<expression.size();i++)
    {
        // if its a number just add it 
        if(expression[i] >= '0' && expression[i] <= '9')
        {
            currrentCharacter+=expression[i];
        }// if its anything but a - sign, just again add the current character and the operation
        else if(expression[i] == '('|| expression[i] == ')'|| expression[i] == '+'|| expression[i] == '*'||expression[i] == '/')
        {
            if(currrentCharacter.size()>0)
            {
                tokens.push_back(currrentCharacter);
                currrentCharacter="";
            }
            string operation = "";
            operation+=expression[i];
            tokens.push_back(operation);
        }
        // this is the tricketst case because - and that too without a space can be very very tricky, it could either mean a subsctraciton or that tis a negative sign
        else if(expression[i] == '-')
        {
            bool isSubstraction = false;
            
            // if there is a currencharacter , that has a length, that means its a substraction sign , simple enough
            if(currrentCharacter.size()>0)
            {
                isSubstraction = true;
            }// lets say no characters in current character, now we check the tokens that we are creating, if the last digit is a ) or a number that means its a substraction 
            else if(!tokens.empty()){
                string lastToken = tokens.back();
                if(lastToken == ")" || lastToken[0] >= '0' && lastToken[0] <= '9' )
                {
                    isSubstraction=true;
                }
            } // now we just push the character that is forming, "890-", we will push the currrentCharacter that is in there and then if by our calcuation its a negative sign just put the next character's first sign as -, else push hte - in the tokens
            if(currrentCharacter.size()>0)
            {
                tokens.push_back(currrentCharacter);
                currrentCharacter="";
            }
            if(isSubstraction == false)
            {
                currrentCharacter="-";
            }
            else{
                tokens.push_back("-");
            }


        }// just ignore the spaces, if its ' ', push the currenct crhacter if it exists
        else if(expression[i]== ' '){
            if(currrentCharacter.size()>0)
            {
                tokens.push_back(currrentCharacter);
                currrentCharacter="";
            }
        }
        else{
            return {};
        }
    }
    // now this is intersting, lets say i reach the end, "12+923", we have reached the end, so, if there is a currentchracter, just push it in , to not lose it
    if(currrentCharacter.size()>0)
    {
        tokens.push_back(currrentCharacter);
        currrentCharacter="";
    }
    return tokens;
}

optional<int> evaluate(string const &expression)
{
    if(expression.size()==0)
        return nullopt;

    //if the expressison starts with these, its wrong altogether
    if(expression[0]=='/' || expression[0]=='*' )
        return nullopt;
   
    // first parse
    vector<string>tokens = tokenize(expression);

    //next steps are tokenize and see if there are same number of brackets ,
    int openBracketCount = 0, closeBracketCount =0;

    for(const string& t : tokens)
    {
        if(t=="(")
            openBracketCount++;
        else if(t== ")")
            closeBracketCount++;
    }

    if(openBracketCount!=closeBracketCount)
    return nullopt;

    auto result = evaluateBrackets(tokens);
    
    return result;
}





int main()
{
    
    assert(stringToNumber("123").value() == 123);
    assert(stringToNumber("-456").value() == -456);
    assert(stringToNumber("0").value() == 0);
    // the try catch handles the error 
    assert(!stringToNumber("").has_value());
    assert(!stringToNumber("abc").has_value());
    
    assert(evaluateLeftToRight({"5"}).value() == 5);
    assert(evaluateLeftToRight({"1", "+", "2"}).value() == 3);
    assert(evaluateLeftToRight({"10", "-", "5"}).value() == 5);
    assert(evaluateLeftToRight({"3", "*", "4"}).value() == 12);
    assert(evaluateLeftToRight({"8", "/", "2"}).value() == 4);
    assert(evaluateLeftToRight({"1", "+", "2", "*", "3"}).value() == 9);
    assert(evaluateLeftToRight({"-5"}).value() == -5);
    assert(evaluateLeftToRight({"-5", "+", "3"}).value() == -2);
    assert(evaluateLeftToRight({"-", "5"}).value() == -5);
    assert(evaluateLeftToRight({"-","5", "+", "3"}).value() == -2);
    assert(evaluateLeftToRight({"7", "/", "3"}).value() == 2);


    assert(!evaluateLeftToRight({}).has_value());
    assert(!evaluateLeftToRight({"5", "/", "0"}).has_value());
    
    assert(evaluateBrackets({"(", "1", "+", "2", ")"}).value() == 3);
    assert(evaluateBrackets({"(", "5", ")"}).value() == 5);
    assert(evaluateBrackets({"(", "1", "+", "2", ")", "*", "3"}).value() == 9);
    assert(evaluateBrackets({"(", "(", "1", "+", "2", ")", ")"}).value() == 3);
    assert(evaluateBrackets({"(", "1", "+", "2", ")", "*", "(", "3", "+", "4", ")"}).value() == 21);
    assert(evaluateBrackets({"5"}).value() == 5);


    assert(!evaluateBrackets({}).has_value());
    assert(!evaluateBrackets({"(", ")"}).has_value());
    assert(!evaluateBrackets({"(", "1", "+", "2"}).has_value());
    
    assert(evaluate("1 + 3").value() == 4);
    assert(evaluate("10 - 5").value() == 5);
    assert(evaluate("3 * 4").value() == 12);
    assert(evaluate("1+2").value() == 3);
    assert(evaluate("(1 + 3) * 2").value() == 8);   
    assert(evaluate("(1 + -3) * 2").value() == -4);
    assert(evaluate("(4 / 2) + 6").value() == 8);
    assert(evaluate("4 + (12 / (1 * 2))").value() == 10);


    assert(evaluate("-5").value() == -5);
    assert(evaluate("-5 + 3").value() == -2);
    assert(evaluate("(-5)").value() == -5);
    assert(evaluate("1 + 2 * 3").value() == 9);
    assert(evaluate("10 - 3 - 2").value() == 5);
    assert(evaluate("-(10 - 3)").value() == -7);
    assert(evaluate("0").value() == 0);
    assert(evaluate("((((5))))").value() == 5);

    // i found my bug with this test case
    assert(evaluate("((-((-5))))").value() == 5);

   
    //the cases where nullopt is returned it won't acutally have a value

    assert(!evaluate("").has_value());
    assert(!evaluate("*5").has_value());
    assert(!evaluate("5/0").has_value());
    assert(!evaluate("( 1 + 2").has_value());
    assert(!evaluate("1 + 2 )").has_value());
    assert(!evaluate("( )").has_value());
    assert(!evaluate("abc").has_value());
    assert(!evaluate("(3/ (2-(2+0))").has_value());
    
    cout<<"Test passed"<<endl;
    
}