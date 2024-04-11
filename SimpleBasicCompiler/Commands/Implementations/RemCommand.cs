using SimpleBasicCompiler.Commands.Interface;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    internal class RemCommand : IBasicCommand
    {
        string _value = "";
        public RemCommand(CompilerFactory compilerFactory)
        {
            _ = compilerFactory;
        }

        public bool Parse(string operand)
        {
            _value = operand.Trim();
            return true;
        }

        public bool Build(Dictionary<string, int> parameters, ref int i, out string line)
        {
            line = $"; {_value}\n";
            return true;
        }
    }
}
