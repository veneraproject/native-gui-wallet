 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

find_path(Readline_ROOT_DIR
    NAMES include/readline/readline.h
    PATHS /usr/local/opt/readline/ /opt/homebrew/opt/readline/ /opt/local/ /usr/local/ /usr/
    NO_DEFAULT_PATH
)

find_path(Readline_INCLUDE_DIR
    NAMES readline/readline.h
    PATHS ${Readline_ROOT_DIR}/include
    NO_DEFAULT_PATH
)

find_library(Readline_LIBRARY
    NAMES readline
    PATHS ${Readline_ROOT_DIR}/lib
    NO_DEFAULT_PATH
)

find_library(Termcap_LIBRARY
  NAMES tinfo termcap ncursesw ncurses cursesw curses
)

if(Readline_INCLUDE_DIR AND Readline_LIBRARY)
  set(READLINE_FOUND TRUE)
else(Readline_INCLUDE_DIR AND Readline_LIBRARY)
  FIND_LIBRARY(Readline_LIBRARY NAMES readline PATHS Readline_ROOT_DIR)
  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Readline DEFAULT_MSG Readline_INCLUDE_DIR Readline_LIBRARY )
  MARK_AS_ADVANCED(Readline_INCLUDE_DIR Readline_LIBRARY)
endif(Readline_INCLUDE_DIR AND Readline_LIBRARY)

mark_as_advanced(
    Readline_ROOT_DIR
    Readline_INCLUDE_DIR
    Readline_LIBRARY
)

set(CMAKE_REQUIRED_INCLUDES ${Readline_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES ${Readline_LIBRARY})

include(CheckFunctionExists)
check_function_exists(rl_copy_text HAVE_COPY_TEXT)
check_function_exists(rl_filename_completion_function HAVE_COMPLETION_FUNCTION)

if(NOT HAVE_COMPLETION_FUNCTION)
  if (Readline_LIBRARY)
    set(CMAKE_REQUIRED_LIBRARIES ${Readline_LIBRARY} ${Termcap_LIBRARY})
  endif(Readline_LIBRARY)
  check_function_exists(rl_copy_text HAVE_COPY_TEXT_TC)
  check_function_exists(rl_filename_completion_function HAVE_COMPLETION_FUNCTION_TC)
  set(HAVE_COMPLETION_FUNCTION ${HAVE_COMPLETION_FUNCTION_TC})
  set(HAVE_COPY_TEXT ${HAVE_COPY_TEXT_TC})
  if(HAVE_COMPLETION_FUNCTION)
    set(Readline_LIBRARY ${Readline_LIBRARY} ${Termcap_LIBRARY})
  endif(HAVE_COMPLETION_FUNCTION)
endif(NOT HAVE_COMPLETION_FUNCTION)

set(LIBEDIT_LIBRARY "")
set(GNU_READLINE_LIBRARY "")

if(HAVE_COMPLETION_FUNCTION AND HAVE_COPY_TEXT)
  set(GNU_READLINE_FOUND TRUE)
  set(GNU_READLINE_LIBRARY ${Readline_LIBRARY})
elseif(READLINE_FOUND AND NOT HAVE_COPY_TEXT)
  set(LIBEDIT_FOUND TRUE)
  set(LIBEDIT_LIBRARY ${Readline_LIBRARY})
endif(HAVE_COMPLETION_FUNCTION AND HAVE_COPY_TEXT)

