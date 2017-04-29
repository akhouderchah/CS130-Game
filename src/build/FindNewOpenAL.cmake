FIND_PATH(OPENAL_INCLUDE_DIR al.h
  PATHS
  $ENV{OPENALDIR}
  NO_DEFAULT_PATH
  PATH_SUFFIXES include/AL include/OpenAL include
)

FIND_PATH(OPENAL_INCLUDE_DIR al.h
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\3.05;InstallDir]
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\3.05;InstallDir]
  PATH_SUFFIXES include/AL include/OpenAL include
)

FIND_LIBRARY(OPENAL_LIBRARY 
  NAMES OpenAL al openal OpenAL32
  PATHS
  $ENV{OPENALDIR}
  NO_DEFAULT_PATH
    PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
)

FIND_LIBRARY(OPENAL_LIBRARY 
  NAMES OpenAL al openal OpenAL32
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\1.00.0000;InstallDir]
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\3.05;InstallDir]
  [HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Creative\ Labs\\OpenAL\ 1.1\ Software\ Development\ Kit\\3.05;InstallDir]
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win64 libs/Win32
)


SET(OPENAL_FOUND "NO")
IF(OPENAL_LIBRARY AND OPENAL_INCLUDE_DIR)
  SET(OPENAL_FOUND "YES")
ENDIF(OPENAL_LIBRARY AND OPENAL_INCLUDE_DIR)

