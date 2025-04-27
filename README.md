SERVER RELEASE


Перед запуском:\
  1 - скачать cmake (от 3.10): https://cmake.org/#:~:text=Enter%20Keyword-,DOWNLOAD,-CMake%3A%20A%20Powerful \
  2 - скачать и поместить в server/includes openssl: https://slproweb.com/products/Win32OpenSSL.html#:~:text=of%20the%20installation.-,Win64%20OpenSSL%20v3.5.0,-EXE%20%7C%20MSI \
  3 - скачать и поместить в server/includes sqlite3.dll: https://www.sqlite.org/download.html#:~:text=Precompiled%20Binaries%20for%20Windows \
  4 - установить системную переменную среды на путь к server/files:\ 
      setx SERVER_FILES "PATH" /M (путь оканчивается на "files" без '/')

Для запуска:\
  1 - запустить _build.cmd\
  2 - запустить _start.cmd

главная страница сайта находится по адресу: localhost:8080


