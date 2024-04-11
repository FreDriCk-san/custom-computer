using SimpleBasicCompiler.Commands.Interface;
using System;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    internal class OutputCommand : AbstractRemContainer, IBasicCommand
    {
        string _name;
        CompilerFactory _compilerFactory;
        public OutputCommand(CompilerFactory compilerFactory)
        {
            _compilerFactory = compilerFactory;
        }

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
            //Если пытаемся показать параметр, которого нет, то ругаемся
            if (!parameters.ContainsKey(_name))
            {
                Console.WriteLine($"Not found parameter for print: {_name}");
                line = "";
                return false;
            }

            int indexParameter = parameters[_name];

            line = $"{i++} WRITE {indexParameter}";
            if (!TrySetRem(parameters, i, ref line))
            {
                return false;
            }

            return true;
        }
    }
}
