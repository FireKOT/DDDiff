@call parameters.bat main.cpp diff.cpp syntaxanal.cpp vec.cpp general.cpp gdump.cpp latex.cpp -o main.exe

@if not errorlevel 1 (
    main.exe %*
)