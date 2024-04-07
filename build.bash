mkdir Artifacts/
rm Artifacts/*.*
g++ -c MySimpleComputer/MySimpleComputer/MySimpleComputer.cpp -o Artifacts/MySimpleComputer.o
g++ -c MyTerm/MyTerm/MyTerm.cpp -o Artifacts/MyTerm.o
g++ -c MyBigChars/MyBigChars/MyBigChars.cpp -o Artifacts/MyBigChars.o
g++ -c CommonTerm/CommonTerm/CommonTerm.cpp -o Artifacts/CommonTerm.o
g++ -c MyReadKey/MyReadKey/MyReadKey.cpp -o Artifacts/MyReadKey.o
cp -R MyBigChars/MyBigChars/BigSymbolsStore.txt Artifacts/
g++ Artifacts/MyTerm.o Artifacts/MyBigChars.o Artifacts/MySimpleComputer.o  Artifacts/CommonTerm.o Artifacts/MyReadKey.o -o Artifacts/CommonProject.o