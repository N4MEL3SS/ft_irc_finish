cmake_minimum_required(VERSION 3.21)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
#add_compile_options(-Wall -Wextra -Werror)

set(SERVER Server.cpp Server.hpp)
set(USER User.cpp User.hpp)
set(CHANNEL Channel.cpp Channel.hpp)
set(MESSAGE Message.cpp Message.hpp)
set(COMMANDS Commands.cpp)
set(UTILS Utils.cpp Utils.hpp)
set(OTHER main.cpp numeric_replies.hpp)

add_executable(ft_irc ${SERVER}  ${USER} ${CHANNEL} ${MESSAGE} ${COMMANDS} ${UTILS} ${OTHER})
