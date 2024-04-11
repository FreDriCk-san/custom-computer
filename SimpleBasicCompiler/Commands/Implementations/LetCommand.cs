using SimpleBasicCompiler.Commands.Interface;
using System;
using System.Collections.Generic;

namespace SimpleBasicCompiler.Commands.Implementations
{
    public class LetCommand : AbstractRemContainer, IBasicCommand
    {
        string _name;
        string _formula;
        CompilerFactory _compilerFactory;
        public LetCommand(CompilerFactory compilerFactory)
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

            int spaceIndex = operand.IndexOf('=');
            if (spaceIndex == -1)
            {
                Console.WriteLine($"Can't find name parameter: {operand}");
                return false;
            }

            //Имя изменяемого параметра
            _name = operand.Substring(0, spaceIndex).Trim();

            operand = operand.Substring(spaceIndex + 1).Trim();

            //Парсим в обратную польскую запись
            _formula = FormulaParser.GetExpression(operand).Trim();

            return true;
        }

        public bool Build(Dictionary<string, int> parameters, ref int i, out string line)
        {
            line = "";

            string formula = _formula;
            var temp = new Stack<string>(); 
            for (int j = 0; j < formula.Length; j++) //Для каждого символа в строке
            {
                //Если символ - оператор
                if (FormulaParser.IsOperator(formula[j])) 
                {
                    //Берем два последних значения из стека
                    var a = temp.Pop();
                    var b = temp.Pop();

                    line += $"{i++} LOAD {parameters[b]} \n";

                    //И производим над ними действие, согласно оператору
                    switch (formula[j]) 
                    {
                        case '+':
                            line += $"{i++} ADD {parameters[a]} \n";
                            break;
                        case '-':
                            line += $"{i++} SUB {parameters[a]} \n";
                            break;
                        case '/':
                            line += $"{i++} DIVIDE {parameters[a]} \n";
                            break;
                        case '*':
                            line += $"{i++} MUL {parameters[a]} \n";
                            break;
                        default:
                            Console.WriteLine($"Unknown command: {formula[j]}");
                            return false;
                    }

                    var key = Guid.NewGuid().ToString().Replace('-', '_');
                    int row = i;

                    //Если это не последнее действие, то для каждого оператора создается своя ячейка памяти
                    if (j + 1 != formula.Length)
                    {
                        line += $"{i} = {0} \n";
                        parameters.Add(key, i++);

                        line += $"{i++} STORE {row} \n";
                        temp.Push(key);
                    }
                }
                //Проверяем, что не разделитель
                else if(!FormulaParser.IsDelimeter(formula[j]))
                {
                    string nameParam = "";
                    //Считываем параметр
                    while (!FormulaParser.IsDelimeter(formula[j]) && !FormulaParser.IsOperator(formula[j])) //Пока не разделитель
                    {
                        nameParam += formula[j]; //Добавляем
                        j++;
                        if (j == formula.Length)
                            break;
                    }

                    //Если параметра еще не было
                    if (!parameters.ContainsKey(nameParam))
                    {
                        int resValue = 0;
                        //Если параметр это число, то парсим
                        if (char.IsDigit(nameParam[0]))
                        {
                            if (!int.TryParse(nameParam, out resValue))
                            {
                                Console.WriteLine($"Parameter can't contain number: {nameParam}");
                                return false;
                            }
                        }

                        line += $"{i} = {resValue} \n";
                        parameters.Add(nameParam, i++);
                    }

                    j--;
                    temp.Push(nameParam); //Записываем в стек
                }
            }

            //Сейчас могут парсится только правильные формулы
            //Это костыль для них
            if (temp.Count > 1)
            {
                return false;
            }
            //Если что-то еще лежит в стеке, то скорее всего было
            //LET A = 1 или LET A = B
            else if (temp.Count == 1)
            {
                var a = temp.Pop();
                line += $"{i++} LOAD {parameters[a]} \n";
            }

            //Добавляем ячейку для выходного параметра
            if (!parameters.ContainsKey(_name))
            {
                parameters.Add(_name, i);
                line += $"{i++} = 0 \n";
            }

            //Записываем результат
            int indexParameter = parameters[_name];
            line += $"{i++} STORE {indexParameter} \n";

            return true;
        }
    }
}
