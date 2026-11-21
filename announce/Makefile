CXX = g++-3.4
CXXFLAGS = -m32 -O0 -std=c++98 -I../source
LDFLAGS = -s

SOURCES = announce.cpp ../source/httpclient.cpp ../source/runutil.cpp

announce: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $@

clean:
	rm -f announce

.PHONY: clean
