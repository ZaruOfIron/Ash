CXXFLAGS=-U__STRICT_ANSI__ -D_WIN32_IE=0x0500 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 -std=c++11
INCLUDE=-I../../../libraries/win32pp -I../../../libraries/lua52 -I../../../libraries/boost_1_57_0/include
LIBRARY=-L../../../libraries/lua52 -L../../../libraries/boost_1_57_0/lib
CPPS=main.cpp lua_controle_script.cpp ash.cpp controle_window.cpp copydata_view.cpp
OBJS=$(CPPS:.cpp=.o)

ash: $(OBJS)
	g++ -o ash $(OBJS) $(LIBRARY) -lcomctl32 -lgdi32 -llua52

.cpp.o:
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $(<:.cpp=.o)

clean:
	@rm $(OBJS)

