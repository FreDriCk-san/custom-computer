mkdir Artifacts/
rm Artifacts/*.*
g++ -c MySimpleComputer/MySimpleComputer/MySimpleComputer.cpp -o Artifacts/msc.o
g++ -c MyTerm/MyTerm/MyTerm.cpp -o Artifacts/mt.o
g++ -c MyBigChars/MyBigChars/MyBigChars.cpp -o Artifacts/mbc.o
g++ -c CommonTerm/CommonTerm/CommonTerm.cpp -o Artifacts/ct.o
g++ -c MyReadKey/MyReadKey/MyReadKey.cpp -o Artifacts/mrk.o
g++ -c SimpleComputerProject/SimpleComputerProject/SimpleComputerProject.cpp -o Artifacts/scp.o
cp -R MyBigChars/MyBigChars/BigSymbolsStore.txt Artifacts/

g++ Artifacts/mt.o Artifacts/mbc.o Artifacts/msc.o  Artifacts/ct.o Artifacts/mrk.o Artifacts/scp.o -o Artifacts/SimpleComputerProject.o

g++ -c SimpleAssemblerCompiler/SimpleAssemblerCompiler/SimpleAssemblerCompiler.cpp -o Artifacts/sac.o
g++ Artifacts/mt.o Artifacts/mbc.o Artifacts/msc.o  Artifacts/ct.o Artifacts/mrk.o Artifacts/sac.o -o Artifacts/SimpleAssemblerCompiler.o

g++ -c MySimpleComputer/MySimpleComputerTests/MySimpleComputerTests.cpp -o Artifacts/msct.o
g++ Artifacts/mt.o Artifacts/mbc.o Artifacts/msc.o  Artifacts/ct.o Artifacts/mrk.o Artifacts/msct.o -o Artifacts/MySimpleComputerTests.o

dotnet build SimpleBasicCompiler/SimpleBasicCompiler.sln
cp -R SimpleBasicCompiler/bin/Debug/net8.0/SimpleBasicCompiler.dll Artifacts/SimpleBasicCompiler.dll
cp -R SimpleBasicCompiler/bin/Debug/net8.0/SimpleBasicCompiler Artifacts/SimpleBasicCompiler
cp -R SimpleBasicCompiler/bin/Debug/net8.0/SimpleBasicCompiler.runtimeconfig.json Artifacts/SimpleBasicCompiler.runtimeconfig.json
cp -R SimpleBasicCompiler/bin/Debug/net8.0/SimpleBasicCompiler.deps.json Artifacts/SimpleBasicCompiler.deps.json
