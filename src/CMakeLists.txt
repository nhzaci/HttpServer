set(LIB_NAME "HttpServer")

file(GLOB LIB_SOURCES *.cxx)
file(GLOB LIB_HEADERS *.hxx)

add_library(${LIB_NAME} ${LIB_SOURCES} ${LIB_HEADERS})

add_executable(RunServer main.cxx)
target_link_libraries(RunServer HttpServer)