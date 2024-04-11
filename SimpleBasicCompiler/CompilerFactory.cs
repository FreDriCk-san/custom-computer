using SimpleBasicCompiler.Commands.Implementations;
using SimpleBasicCompiler.Commands.Interface;
using System;
using System.Collections.Generic;
using System.IO;

namespace SimpleBasicCompiler
{
    public class CompilerFactory
    {
        //Коллекция распарсенных команд.
        //Ключ номер строки из файла
        //Значение картеж из команды и реального адреса
        internal Dictionary<int, (IBasicCommand, int)> _commandLines = new Dictionary<int, (IBasicCommand, int)>();

        //Парсер строк в команды
        static internal bool GetBasicCommand(CompilerFactory compilerFactory,
                                             string command,
                                             string line,
                                             out IBasicCommand? basicCommand)
        {
            basicCommand = null;
            switch (command)
            {
                case " REM ":
                    basicCommand = new RemCommand(compilerFactory);
                    break;
                case " INPUT ":
                    basicCommand = new InputCommand(compilerFactory);
                    break;
                //Почему то в документе написано, что необходимо поддерживать OUTPUT,
                //но в примере используется PRINT
                case " OUTPUT ":
                case " PRINT ":
                    basicCommand = new OutputCommand(compilerFactory);
                    break;
                case " GOTO ":
                    basicCommand = new GotoCommand(compilerFactory);
                    break;
                case " IF ":
                    basicCommand = new IfCommand(compilerFactory);
                    break;
                case " LET ":
                    basicCommand = new LetCommand(compilerFactory);
                    break;
                case " END ":
                case " END":
                    basicCommand = new EndCommand(compilerFactory);
                    break;
                default:
                    Console.WriteLine($"Unknown command {command}");
                    return false;
            }

            if (!basicCommand.Parse(line))
            {
                return false;
            }

            return true;
        }

        internal bool Parse(int rowNumber, string command, string line)
        {
            if(!GetBasicCommand(this, command, line, out var basicCommand))
            {
                return false;
            }

            //Добавляем распарсенную команду в список команд
            _commandLines.Add(rowNumber, (basicCommand!, -1));
            return true;
        }

        //Коллекция goto команд, которые ссылаются еще не на созданные строки
        Dictionary<GotoCommand, string> skippedJumps = new Dictionary<GotoCommand, string>();
        //Сборка и сохранение строк из команд в файл 
        internal bool Save(string fileSave)
        {
            using var stream = File.CreateText(fileSave);
            int i = 0;
            var parameters = new Dictionary<string, int>();
            
            var lines = new List<string>();
            foreach((var key, (var command, int index)) in _commandLines)
            {
                int startIndex = i;
                //Сборка строки по команде
                if(!command.Build(parameters, ref i, out var line))
                {
                    return false;
                }

                _commandLines[key] = (command, startIndex);
                lines.Add(line);
            }

            //Заполняем пропущенные GOTO команды
            foreach((var gotoCommand, var oldLine) in skippedJumps)
            {
                //Команда может ссылаться на несуществующую строку
                if (!_commandLines.ContainsKey(gotoCommand.InnerRefRow))
                {
                    Console.WriteLine($"Goto contain reference to a non-existent line number: {gotoCommand.InnerRefRow}");
                }

                //Новый адрес строки
                var newRow = _commandLines[gotoCommand.InnerRefRow].Item2;
                for(int j = 0; j < lines.Count; j++)
                {
                    var line = lines[j];
                    //Поиск подстроки в строке 
                    if (line.Contains(oldLine))
                    {
                        //Замена
                        var newLine = oldLine.Replace("-1", newRow.ToString());
                        lines[j] = line.Replace(oldLine, newLine);
                        break;
                    }
                }
            }

            //Запись строк в файл
            foreach (var line in lines)
            {
                stream.Write(line);
            }

            return true;
        }

        //Функция для доступа к словарю пропущенных GOTO команд
        internal void AddSkippedGoto(GotoCommand gotoCommand, string line)
        {
            if (gotoCommand.RefRow == -1)
            {
                skippedJumps.Add(gotoCommand, line);
            }
        }
    }
}
