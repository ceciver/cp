set -e
g++ -std=c++17 code.cpp -o code
g++ -std=c++17 gen.cpp -o gen
g++ -std=c++17 brute.cpp -o brute

for((i = 1; ; ++i)); do
    ./gen $i > input_file
    echo input_file
    ./code < input_file > myAnswer
    ./brute < input_file > correctAnswer
    diff myAnswer correctAnswer > /dev/null || break
    echo "Passed test: "  $i
done
echo "WA on the following test:"
cat input_file
echo "Your answer is:"
cat myAnswer
echo "Correct answer is:"
cat correctAnswer
