using SimpleBasicCompiler.Commands.Interface;
using System;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    internal class InputCommand : AbstractRemContainer, IBasicCommand
    {
        CompilerFactory _compilerFactory;
        public InputCommand(CompilerFactory compilerFactory)
        {
            _compilerFactory = compilerFactory;
        }

        string _name;
        public bool Parse(string operand)
        {
            if (!CheckOnRem(_compilerFactory, ref operand))
            {
                return false;
            }

            operand = operand.Trim();

            int spaceIndex = operand.IndexOf(' ');
            if (spaceIndex != -1)
            {
                Console.WriteLine($"Parameter can't contain space characters: {operand}");
                return false;
            }

            _name = operand;

            if (string.IsNullOrEmpty(_name))
            {
                Console.WriteLine($"Parameter not contain any characters");
                return false;
            }

            return true;
        }

        public bool Build(Dictionary<string, int> parameters, ref int i, out string line)
        {
            bool addedNewParam = false;
            if (!parameters.ContainsKey(_name))
            {
                parameters.Add(_name, i + 1);
                addedNewParam = true;
            }

            int indexParameter = parameters[_name];

            //Добавляем READ
            line = $"{i++} READ {indexParameter}";
            if (!TrySetRem(parameters, i, ref line))
            {
                return false;
            }

            //Если параметра еще не было, добавляем для него ячейку
            if (addedNewParam)
            {
                line += $"{i++} = 0 \n";
            }

            return true;
        }
    }
}
