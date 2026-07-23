cmake --build build
cd python
start /B uvicorn translation_server:app --host 127.0.0.1 --port 8000 --workers 8
cd ..
timeout /t 2 /nobreak
bin\Debug\VapeCloudBot

taskkill /F /IM uvicorn.exe
