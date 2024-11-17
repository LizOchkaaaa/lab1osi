.\run_benchmark.ps1 -OutputLogDir "bin" -BinSearchProgram "..\..\cmake-build-debug\source\Bin.exe" -BinSearchParam "1000"
.\run_benchmark.ps1 -OutputLogDir "io" -IoThptWriteProgram "..\..\cmake-build-debug\source\IO.exe" -AdditionalArguments @("10", "..\..\cmake-build-debug\source\testfile.txt", "4", "1024")
.\run_benchmark.ps1 -OutputLogDir "combined" -BinSearchProgram "..\..\cmake-build-debug\source\Bin.exe" -BinSearchParam "1000" -IoThptWriteProgram "..\..\cmake-build-debug\source\IO.exe" -AdditionalArguments @("1000", "..\..\cmake-build-debug\source\testfile.txt", "4", "1024")

