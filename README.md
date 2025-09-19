## Made this to document my thoughts on the problem

1. We can assume that there can be spaces in between the numbers and the characters, like 1 + -3 or 1   + -3, or, there might not be spaces (1+3)*2 is a valid expression and hence, it should be processed. Its important to not that only "-" before an integer makes an sense else, if there is something like 1 + /3 , or 1 + 9- that makes no mathematical sense and we can throw an error,
2. The first obvious step is to extract all the characters, no matter how many whitespaces are there in between, they are to be ignored, then to have this parsed in an array, once i have this array, the program becomes much simpler, once something like {"1", "+", "(" , "3", "*" ,"5" , ")"} is obtained, we can safely asssume, its a question of evaluating the brackets and then evaluting the rest of the expression from left to right. 
3. Second step is to get the brackets evaluated , (again, the elements inside the brackets have to evaluted from left right) like 1 + (-5 * (3 + 4)), we can make it as 1 + (-5 * 7) and 1 + -35 and -34, if there is an uneven number of brackets, we can safely assume it a mathematical error, even a statement such as (()), throws an error because, we cannot have empty brackets
4. Once we have dealt with brackets, we have to just evaluate it left to right, 
5. For now the clear steps is , evaluate the expression starting with teh inner most bracket first, till there are no brackets
6. Evaluate it from left to right 
7. One thought is we can break it into three functions, 

    a. Coverting the string to the number with error handling
    b. Search for brackets (and keep on evaluatin from left to right)
    c. Evluation from left to right

# Actual implementation 

## Parsing:
The tokenizer in my opion is the most cruicial part, I have to consider that there might be spaces, in between, there might be no spaces in between, 
I need to form the array with all the tokens in it, side by side, 
Now the part is , it might be "1234+(234+245)"

1. I ll mantian a currentDigit to keep count of the number that am forming , If I encounter a digit, I would just add it to the current digit and keep on forming.
2. If I encounter an opertaion , "+" , "/" , "("  , ")" or "*" ,  I just push the current number that I have formed and the operation in the array
3. One more case is the '-' operator, which i think is the trickest, 10-3 and 10+-3 are both logically correct expressions, one thing is, lets say we encounter a "-" sign, now, we check the last token , if its a number 1 through 9 or a closing bracket (like 10-3 or (10+3)-4), its quite clear that its a substraction, in which case, we push the formed number and the - sign, 
4. If not, if there is an operation before the - sign, that means, its a negative number, thus, just push the number, and then start the currentDigit with a "-", that shows a negative sign. (10+-3
)
5. There is another edge case , -(5+4) should be -9

5. If we have a blank space that means, we can ignore it, just i ll push in the currentDigit and initialize it to ""


## StringToNumber
1. We already have everything parsed in, a simple way to convert from string to integer is the stoi function in the C++ libaray.  I will encase this in try catch so that it does not throw errors.

## LeftToRight
1. The next problem is lets say we have an array of numbers and operations, like {"1", "+", "2","*","3"}, evaluate this from left to right, this is quite simple, I have convert the first number to an integer with the function I just made, its noticable that every odd index will have operations, like index 1, index 2, index 3, 
so, I chose the following idea, 
3. Make a variable result and have the first integer's value in it, (if it returns a nullopt, propagate the error, to the main for error handling)
4. Start a loop from 1 to lenght of array -1 and increment +2, and then the current index will have the operation, (extract and store in a string) and index+1 will have all numbers (in the form of strings), convert them to numbers using the stringtonumber function, and then use a simple if statement (we know there can only be 4 operations to generate the ans)
{"1"+"2"}, we can see that the 0th index contains the number and then the 1st is the operation, similarly we can think that ith position will be the integer and the i+1th position will be a number.
Note: I ran into a bug with this which has been discussed later


## BracketEvaluation

1. Now, the problem is simpler, if I have something like {"1", "+", "(", "9", "+", "(" "5" "+" "9", ")")}, we should find out the inner most brackts and use the left to right to evaluate 
this should be then 
2. {"1", "+", "(", "9", "+", "(" "5" "+" "9", ")")}, this should become {"1", "+", "(", "9", "+", "14")} and then {"1", "+", "23"} 
3. The inner working is quite simple, you take the index of the last opneing bracket, (a simple for loop) and then the poisition of the first closing bracket, there are. a few edgecases, i have to consider, like if there is an opneing bracket but no closing bracket, or if htere is a closing bracket no opening bracket. all cases return a nullopt
4. I chose to conitnue doing step 1 and then evaluating, until there are no opneing brackets, once that is sorted, the only thing that is left is an entire array without brackets and i just need to evalute it and we are done 

## Bug report 
One bug that I ran into was with an expression like (-(5)), the bracket evalutor evaluted it into {"(", "-", "5",")"}, now, the left to right, goes on to evaluate {"-", "5"}, now the expression will thow an error, this is because the evaluator tries to think that "-" tries to parse it to a number and thus fails
Now, the fix is in the evaluator ihwould have to check that if the first nuumber is a "-", then  I would just merge it with the next number, even for something like {(-(5+9)*2)}, now I would  have to merge it with the number adjacent the -, 
and make it become {-14*2}. On my initial glance this problem seemed to be simple , however there are several sensitive test cases  that should be handled 

## Challengs
Writing the unit test suite , typing it out was the toughest part, also the parsing was tough, intially i wanted to use strinstream to calculate, however, there could be one or more whitespaces or no whitespaces at all, also , there is no space between () and the numbers, hence the parsing should be able to cover that properly , I also used using namespace std to make the program easier to read 
Also, nullopt returns a garabge value, so if its a grabage value just return it correclty 

## Steps to run
g++ -o main main.cpp
./main

## Assumptions I made
1. the function signature returns an integer, that means, that the expression returns a value that fits in integer, if it does not using long long is a viable way, or including double to include floating point numbers
2. The length of the expression is about 10power8, this is because most CPU architectures can support 10power8 operations per second and this program thus runs in one second 
3. I use GCC 14 compiler , I used using namespace std and using bits/stdc++.h to acces the inbuilt c++ libaries, I don't use any evalution or inuilt functions
