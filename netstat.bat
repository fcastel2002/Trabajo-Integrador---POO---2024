@echo off
:loop

netstat -ano | findstr 9000
timeout /t 2 /nobreak >nul
goto loop