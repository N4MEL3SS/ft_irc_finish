cmake_minimum_required(VERSION 3.21)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
#add_compile_options(-Wall -Wextra -Werror)

include_directories("./")

set(SERVER Server.cpp Server.hpp)
set(USER User.cpp User.hpp)
set(CHANNEL Channel.cpp Channel.hpp)
set(MESSAGE Message.cpp Message.hpp)
set(COMMANDS commands/registration_commands.cpp commands/channel_commands.cpp commands/other_commands.cpp commands/user_commands.cpp commands/irc_operatror_commands.cpp)
set(UTILS Utils.cpp Utils.hpp)
set(OTHER main.cpp numeric_replies.hpp commands/init_commands.cpp)
set(BOT Bot/Client.cpp Bot/Client.hpp Bot/main.cpp)

add_executable(ft_irc ${SERVER}  ${USER} ${CHANNEL} ${MESSAGE} ${COMMANDS} ${UTILS} ${OTHER})
add_executable(bot ${BOT})
