using SimpleBasicCompiler.Commands.Interface;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    internal class EndCommand : AbstractRemContainer, IBasicCommand
    {
        CompilerFactory _compilerFactory;
        public EndCommand(CompilerFactory compilerFactory)
        {
            _compilerFactory = compilerFactory;
        }

        public bool Parse(string operand)
        {
            if (!CheckOnRem(_compilerFactory, ref operand))
            {
                return false;
            }

            return true;
        }

        //Добавляем только HALT 00
        public bool Build(Dictionary<string, int> parameters, ref int i, out string line)
        {
            line = $"{i++} HALT 00";
            if (!TrySetRem(parameters, i, ref line))
            {
                return false;
            }
            return true;
        }
    }
}
