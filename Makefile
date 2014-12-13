CXXFLAGS=-U__STRICT_ANSI__ -D_WIN32_IE=0x0500 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -std=c++11
INCLUDE=-I../../libraries/win32pp -I../../libraries/lua52 -I../../libraries/boost_1_57_0/include
LIBRARY=-L../../libraries/lua52 -L../../libraries/boost_1_57_0/lib
CPPS=main.cpp lua_controle_script.cpp test_main.cpp test/controle.cpp
OBJS=$(CPPS:.cpp=.o)

ash: $(OBJS)
	g++ -o ash $(OBJS) $(LIBRARY) -lcomctl32 -lgdi32 -llua52 -lboost_test_exec_monitor-mgw48-mt-1_57

.cpp.o:
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $(<:.cpp=.o)

clean:
	@rm $(OBJS)

