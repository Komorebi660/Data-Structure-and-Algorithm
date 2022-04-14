main: A_star IDA_star
A_star: A_star.cpp A_star.h input.txt
	g++ A_star.cpp -O2 -o A_star
IDA_star: IDA_star.cpp A_star.h input.txt
	g++ IDA_star.cpp -O2 -o IDA_star
input.txt: TestDataGen
	./TestDataGen
	rm TestDataGen
TestDataGen: TestDataGen.cpp
	g++ TestDataGen.cpp -O2 -o TestDataGen
CheckAnswer: CheckAnswer.cpp
	g++ CheckAnswer.cpp -O2 -o CheckAnswer


.PHONY: clean check
clean:
	rm A_star IDA_star input.txt target.txt 
check: A_star IDA_star CheckAnswer
	./A_star
	./CheckAnswer
	rm output.txt
	./IDA_star
	./CheckAnswer
	rm output.txt
	rm CheckAnswer
