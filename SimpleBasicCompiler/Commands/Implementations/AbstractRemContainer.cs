using System;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    //Абстрактный класс для команд, которые могут содержать комментарии
    public class AbstractRemContainer
    {
        RemCommand? _remCommand;
        protected bool CheckOnRem(CompilerFactory compilerFactory, ref string operand)
        {
            //Проверяем строку, содержит ли она REM
            int remIndex = operand.IndexOf(" REM ");
            if (remIndex != -1)
            {
                var remString = operand.Substring(remIndex + 4);
                operand = operand.Substring(0, remIndex);
                _remCommand = new RemCommand(compilerFactory);
                if (!_remCommand.Parse(remString))
                {
                    Console.WriteLine($"Can't parse REM in input: {remString}");
                    return false;
                }
            }

            return true;
        }

        protected bool TrySetRem(Dictionary<string, int> parameters, int i, ref string line)
        {
            //Дополнение комментарием.
            //TODO:Не во всех командах понятно как эти комментарии поместить после команды
            if (_remCommand != null)
            {
                if (!_remCommand.Build(parameters, ref i, out var remline))
                {
                    Console.WriteLine($"Can't build rem line in other line");
                    return false;
                }
                line += remline;
            }
            else
            {
                line += "\n";
            }

            return true;
        }
    }
}
