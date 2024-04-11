#include <iostream>
#include <cstring>
#include <fstream>
#include <map>

#include "../../MySimpleComputer/MySimpleComputer/MySimpleComputer.h"

using namespace std;

//Проверка строки, число это или нет
bool is_number(const std::string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && (isdigit(*it) || *it == '-'))
    {
        ++it;
    }
    return !s.empty() && it == s.end();
}

//Создание словаря для команд
map<string, int> getPossibleCommand()
{
    map<string, int> map;
    map["READ"]= 10;
    map["WRITE"]= 11;
    map["LOAD"]= 20;
    map["STORE"]= 21;
    map["ADD"]= 30;
    map["SUB"]= 31;
    map["DIVIDE"]= 32;
    map["MUL"]= 33;
    map["JUMP"]= 40;
    map["JNEG"]= 41;
    map["JZ"]= 42;
    map["HALT"]= 43;
    map["NOT"]= 51;
    map["AND"]= 52;
    map["OR"]= 53;
    map["XOR"]= 54;
    map["JNS"]= 55;
    map["JC"]= 56;
    map["JNC"]= 57;
    map["JP"]= 58;
    map["JNP"]= 59;
    map["CHL"]= 60;
    map["SHR"]= 61;
    map["RCL"]= 62;
    map["RCR"]= 63;
    map["NEG"]= 64;
    map["ADDC"]= 65;
    map["SUBC"]= 66;
    map["LOGLC"]= 67;
    map["LOGRC"]= 68;
    map["RCCL"]= 69;
    map["RCCR"]= 70;
    map["MOVA"]= 71;
    map["MOVR"]= 72;
    map["MOVCA"]= 73;
    map["MOVCR"]= 74;
    map["EADDC"]= 75;
    map["ESUBC"]= 76;
    map["="]= 99;
    return map;
} 

//Парсим строку
int tryParseSplit(string split, int index, map<string, int> map, int * address, int * command, int * opperand)
{
    //Заполнение адрема ячейки
    if(*address == -1)
    {
        if(!is_number(split))
        {
            cout << "Row " << index << ". Address contains invalid characters: " << split << endl;
            return -1;
        }
        *address = atoi(split.data());
    }
    //Заполнение команды
    else if(*command == -1)
    {
        if(is_number(split))
        {
            cout << "Row " << index << ". Command contains invalid characters: " << split << endl;
            return -1;
        }

        //Поиск команды в словаре
        const char* item = split.data();
        if(map.find(item) == map.end())
        {
            cout << "Row " << index << ". Unknown command: " << item << endl;
            return -1;
        }

        *command = map.at(item);
    }
    //Поиск операнда
    else if(*opperand == -1)
    {
        if(!is_number(split))
        {
            cout << "Row " << index << ". Opperand contains invalid characters: " << split << endl;
            return -1;
        }
        *opperand = atoi(split.data());
    }
    //Слишком много параметров в строке
    else
    {
        cout << "Row " << index << ". Too many arguments on line: " << split << endl;
        return -1;
    }
}

int tryParse(char* fileName)
{
    //Ресетаем память
    sc_reset();

    //Загружаем файл
    string filePath(fileName);
    fstream myfile;
	myfile.open(filePath, ios::in);

	if (!myfile) 
    {
        cout << "Can't open file" << endl;
		return -1;
	}

    //Создаем словарь
    map<string, int> map = getPossibleCommand();

    string line;
    //Проверяем по строчкам
    int index = 1;

    //Парсим по строкам
    int flag = 0;
    while (getline(myfile, line))
    {
        size_t pos = 0;
        //Удаляем комментарии. Все что после знака ; отбрасывается
        if((pos = line.find(";")) != std::string::npos)
        {
            line = line.substr(0, pos);
        }

        //Скорее всего на строке был только комментарий
        if(line.length() == 0)
        {
            index++;
            continue;
        }

        int address = -1;
        int command = -1;
        int opperand = -1;
        //Парсим строку по разделителю
        while ((pos = line.find(" ")) != std::string::npos)
        {
            //Если несколько разделителей, то смещаем
            string split = line.substr(0, pos);
            if(split.length() == 1 && split[0] == ' ')
            {
                line.erase(0, pos + 1);
                continue;
            }

            //Парсинг части строки
            int res = tryParseSplit(split, index, map, &address, &command, &opperand);
            if(res == -1)
            {
                flag = -1;
                goto skip;
            }
            
            line.erase(0, pos + 1);
        }

        //Если в строке еще что-то осталось
        if(line.length() > 0)
        {
            int res = tryParseSplit(line, index, map, &address, &command, &opperand);
            if(res == -1)
            {
                flag = -1;
                goto skip;
            }
        }

        if(address == -1)
        {
            cout << "Row " << index << ". Not found address" << endl;
            flag = -1;
        }
        else if(command == -1)
        {
            cout << "Row " << index << ". Not found command" << endl;
            flag = -1;
        }
        else if(opperand == -1)
        {
            cout << "Row " << index << ". Not found opperand or value" << endl;
            flag = -1;
        }
        else
        {
            //Если команда 99, то это =
            int code;
            if(command == 99)
            {
                int resCode = sc_memorySetAndEncode(address, opperand, &code);
                if(resCode < 0)
                {
                    cout << "Row " << index << ". Can't set value " << opperand << " in " << address << ". Res code " << resCode << endl;
                    return -1;
                }
            }
            //Иначе команда
            else
            {
                int resCode = sc_commandSetAndEncode(address, command, opperand, &code);
                if(resCode < 0)
                {
                    cout << "Row " << index << ". Can't command " << command << " opperand " << opperand << " in " << address << ". Res code " << resCode << endl;
                    return -1;
                }
            }
        }

skip:
        index++;
    }

    return flag;
}

//Запись в файл
int trySaveNewFile(char* fileName)
{
    return sc_memorySave(fileName);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "Invalid number of parameters specified" << endl;
        return -1;
    }

    char* fileName = argv[1];
    char* outputFileName = argv[2];
    //Парсинг входного файла
    if(tryParse(fileName) == -1)
    {
        cout << "Can't parse file" << endl;
        return -1;
    }

    //Сохранение файла
    if(trySaveNewFile(outputFileName) == -1)
    {
        cout << "Can't save file" << endl;
        return -1;
    }

    cout << "File " << fileName << " converted and saved to " << outputFileName << endl;

    return 0;
}