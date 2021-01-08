# DIAGRAMS is a list of all available diagrams
macro(configureDiagramLibrary FILE DIAGRAMS)

	file(WRITE ${FILE} "")

## diagram includes
foreach(DIAGRAM ${DIAGRAMS})
	file(APPEND ${FILE} "#include \"${DIAGRAM}.h\"\n")
endforeach()

## availableDiagrams()
set(PREFIX_AVAILABLE_DIAGRAMS "#include <QStringList>
#include \"diagram.h\"
extern \"C\" QStringList availableDiagrams() {
return {")
set(POSTFIX_AVAILABLE_DIAGRAMS "\t}\;\n}")


file(APPEND ${FILE} ${PREFIX_AVAILABLE_DIAGRAMS})
foreach(DIAGRAM ${DIAGRAMS})
	file(APPEND ${FILE} "\"${DIAGRAM}\",\n")
endforeach()
file(APPEND ${FILE} ${POSTFIX_AVAILABLE_DIAGRAMS} "\n\n")

## create object
set(BASECLASS "Diagram")
set(ARGS "int _cx=0, int _cy=0")
set(ARGS_FUNCTION_CALL "_cx, _cy")
foreach(DIAGRAM ${DIAGRAMS})
	file(APPEND ${FILE} "extern \"C\" ${BASECLASS}* create_${DIAGRAM}(${ARGS}) {return new ${DIAGRAM}(${ARGS_FUNCTION_CALL});}\n")
endforeach()

## destroy object
file(APPEND ${FILE} "extern \"C\" void destroy_diagram(${BASECLASS}* d) {delete d;}")

endmacro()
