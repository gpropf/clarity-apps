AR 		= emar
CC		= emcc

#ENV		= DEMANGLE_SUPPORT=1 EMCC_DEBUG=1
ENV		= DEMANGLE_SUPPORT=1

#CFLAGS	=  -O0 -g -gsource-map --source-map-base=http://localhost:6931/ -std=c++17
CFLAGS	=  -O0 -g -std=c++17 -I./clarity
#CFLAGS	=  -O3 -std=c++17

JSOUT	= clarity_embind.js

FRAMEWORK_DEPS = Clarity.js Util.js pixelreactor.html Makefile

# %.o : %.cpp $(FRAMEWORK_DEPS)
# 	$(ENV) $(CC) $< -o $@ -c $(CFLAGS)

%.o : %.cpp
	$(ENV) $(CC) $< -o $@ -c $(CFLAGS)

Pixelreactor.o: Pixelreactor.hpp
	$(ENV) $(CC) pixelreactor.hpp -c $(CFLAGS)
	
pixelreactor: pixelreactor.o
	cd clarity; make realclean; make ClarityNode.o
	$(ENV) $(CC) -lembind pixelreactor.o clarity/ClarityNode.o $(CFLAGS) -o $(JSOUT)

# foo: pixelreactor.o
#  	#
	
	
# $(CC) -lembind pixelreactor.o ClarityNode.o CanvasElement.o $(CFLAGS) -o $(JSOUT)


docs: clarity.doxyconfig
	doxygen clarity.doxyconfig

clean:
	rm -f *.o 

realclean: clean
	rm -f *.wasm *.wasm.map *.a $(JSOUT)

#all: testbed speedtest docs