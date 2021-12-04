g++ -std=c++20 -Wall -Wextra -O2 testy-zad4/"$1" -fconcepts-diagnostics-depth=10000 2>&1 | less
rm *.gch

