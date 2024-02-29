# CMakeList.txt: проект CMake для 2048; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("2048")

set(SFML_DIR "G:/documents/2048/external/SFML-2.6.1/lib/cmake/SFML")


find_package(SFML 2.6.1 COMPONENTS graphics audio REQUIRED)

add_executable (2048 "2048.cpp" "2048.h" "GUI/Button/Button.cpp" )

target_link_libraries(2048 sfml-graphics sfml-audio)


# TODO: Добавьте тесты и целевые объекты, если это необходимо.
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 2048 PROPERTY CXX_STANDARD 20)
endif()
