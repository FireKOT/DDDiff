@call parameters.bat main.cpp diff.cpp syntaxanal.cpp -o main.exe

@if not errorlevel 1 (
    main.exe %*
)