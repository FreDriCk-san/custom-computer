using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Interface
{
    public interface IBasicCommand
    {
        public bool Parse(string operand);
        bool Build(Dictionary<string, int> parameters, ref int index, out string text);
    }
}
