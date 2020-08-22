# opencvproj
чтобы собрать проект используйте
cmake -B . -S .
cmake --build . --clean-first

запуск rtsp клиента(для выхода нажмите 'ESC'):
./gvc rtsp://your-url.com



чтобы тестировать (с двумя трансляциями из интернета)
ctest .
