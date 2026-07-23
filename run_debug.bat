cmake --build build
cd python
call .venv\Scripts\activate.bat
timeout /t 1 /nobreak
start "Translation Server" uv run python translation_server.py
cd ..
timeout /t 2 /nobreak
bin\Debug\VapeCloudBot

