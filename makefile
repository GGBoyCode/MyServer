src=$(wildcard ./*.cpp ./src/*.cpp)
objs=$(patsubst %.cpp, %.o, $(src))
target=app
$(target):$(objs)
	g++ -o $@ $^
	rm -f $^

%.o:%.cpp
	g++ -c $< -o $@ -I ./include
