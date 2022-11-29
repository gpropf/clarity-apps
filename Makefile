AR 		= emar
CC		= emcc

#ENV		= DEMANGLE_SUPPORT=1 EMCC_DEBUG=1
ENV		= DEMANGLE_SUPPORT=1

#CFLAGS	=  -O0 -g -gsource-map --source-map-base=http://localhost:6931/ -std=c++17
CFLAGS	=  -O0 -g -std=c++17 -I./clarity -D USETF
#CFLAGS	=  -O3 -std=c++17

JSOUT	= clarity_embind.js

FRAMEWORK_DEPS = JSProxyNode.js Util.js pixelreactor.html Makefile

# %.o : %.cpp $(FRAMEWORK_DEPS)
# 	$(ENV) $(CC) $< -o $@ -c $(CFLAGS)

%.o : %.cpp
	$(ENV) $(CC) $< -o $@ -c $(CFLAGS) 

# Pixelreactor.o: Pixelreactor.hpp
# 	$(ENV) $(CC) pixelreactor.hpp -c $(CFLAGS)

# cd clarity; make ClarityNode.o CanvasElement.o Selectables.o clarity.o

PIXELREACTOR_OBJS = pixelreactor.o clarity/ClarityNode.o clarity/CanvasElement.o clarity/Selectables.o clarity/clarity.o

pixelreactor-prod: CFLAGS = -O1 -std=c++17 -I./clarity
pixelreactor-prod: ENV =
pixelreactor-prod: $(PIXELREACTOR_OBJS)	
	$(ENV) $(CC) -lembind --pre-js jsaux/clapps-aux.js $(PIXELREACTOR_OBJS) $(CFLAGS) -o $(JSOUT)


pixelreactor: $(PIXELREACTOR_OBJS)	
	$(ENV) $(CC) -lembind --pre-js jsaux/clapps-aux.js $(PIXELREACTOR_OBJS) $(CFLAGS) -o $(JSOUT)

CLDemo.o: CLDemo.hpp
	$(ENV) $(CC) CLDemo.hpp -c $(CFLAGS)
	
cldemo: cldemo.o clarity/ClarityNode.o clarity/CanvasElement.o clarity/Selectables.o clarity/clarity.o
	cd clarity; make ClarityNode.o CanvasElement.o Selectables.o clarity.o
	$(ENV) $(CC) -lembind cldemo.o clarity/ClarityNode.o clarity/CanvasElement.o clarity/Selectables.o clarity/clarity.o $(CFLAGS) -o $(JSOUT)

docs: clarity-apps.doxyconfig
	doxygen clarity-apps.doxyconfig

docsclean:
	rm -rf html latex

clean:
	rm -f *.o 

realclean: clean
	cd clarity; make realclean
	rm -f *.wasm *.wasm.map *.a $(JSOUT)

#all: pixelreactor cldemo docs