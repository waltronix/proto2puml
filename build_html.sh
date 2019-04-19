mkdir public

em++ --std=c++17 \
    -O3 \
    -s "DISABLE_EXCEPTION_CATCHING=0"  \
    -s "EXPORTED_FUNCTIONS=['_proto_to_puml']" \
    -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap']" \
    -o public/proto2puml.js \
    -I include \
    -I 3rdParty/PEGTL/include \
    src/proto2puml.cpp \
    src/puml_printer.cpp

cp html/index.html public
cp 3rdParty/plantuml-encoder/dist/plantuml-encoder.min.js public
