using SimpleBasicCompiler.Commands.Interface;
using System;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    internal class GotoCommand : AbstractRemContainer, IBasicCommand
    {
        CompilerFactory _compilerFactory;
        public GotoCommand(CompilerFactory compilerFactory)
        {
            _compilerFactory = compilerFactory;
        }

        internal int InnerRefRow { get; private set; } = -1;
        internal int RefRow { get; set; } = -1;
        public bool Parse(string operand)
        {
            if (!CheckOnRem(_compilerFactory, ref operand))
            {
                return false;
            }

            //Проверяем, что операнд - это число
            if (!int.TryParse(operand.Trim(), out var res))
            {
                Console.WriteLine($"Can't parse ref line in goto: {operand}");
                return false;
            }

            InnerRefRow = res;

            return true;
        }

        public bool Build(Dictionary<string, int> parameters, ref int i, out string line)
        {
            //Команда может ссылаться на несуществующую строку
            if (!_compilerFactory._commandLines.ContainsKey(InnerRefRow))
            {
                Console.WriteLine($"Goto contain reference to a non-existent line number: {InnerRefRow}");
            }

            RefRow = _compilerFactory._commandLines[InnerRefRow].Item2;
            line = $"{i++} JUMP {RefRow} \n";

            //Если ссылка идет на выше лежащую строку, то ссылка будет
            //Если ссылка идет на ниже лежащую строку, то ее скорее всего нет, и обработать нужно будет позже
            if (RefRow == -1)
            {
                _compilerFactory.AddSkippedGoto(this, line);
            }

            return true;
        }
    }
}
