using System;
using System.Collections.Generic;
using SimpleBasicCompiler.Commands.Interface;

namespace SimpleBasicCompiler.Commands.Implementations
{
    internal class IfCommand : AbstractRemContainer, IBasicCommand
    {
        string? _left = null;
        string? _oper = null;
        string? _right = null;
        IBasicCommand? _basicCommand = null;
        CompilerFactory _compilerFactory;
        public IfCommand(CompilerFactory compilerFactory)
        {
            _compilerFactory = compilerFactory;
        }

        //Парсится только команды с одним неравенством (IF A > B)
        public bool Parse(string operand)
        {
            if (!CheckOnRem(_compilerFactory, ref operand))
            {
                return false;
            }

            operand = operand.Trim();

            //Парсим операнд
            for(int i = 0; i < operand.Length; i++)
            {
                //Проверяем на разделитель
                if (!FormulaParser.IsDelimeter(operand[i]))
                {
                    //Если это знак неравенства
                    if (FormulaParser.IsInequality(operand[i]))
                    {
                        //Собираем знаки неравенства
                        string nameParam = "";
                        while (!FormulaParser.IsDelimeter(operand[i]) && FormulaParser.IsInequality(operand[i]))
                        {
                            nameParam += operand[i];
                            i++;
                            if (i == operand.Length)
                                break;
                        }

                        //Записываем их
                        _oper = nameParam;
                    }
                    //Иначе или параметр или значение
                    else
                    {
                        //Собираем параметр или значение
                        string nameParam = "";
                        while (!FormulaParser.IsDelimeter(operand[i]) && !FormulaParser.IsInequality(operand[i]))
                        {
                            nameParam += operand[i];
                            i++;
                            if (i == operand.Length)
                                break;
                        }

                        //Записываем в левый параметр
                        if (_left == null)
                        {
                            _left = nameParam;
                        }
                        //Записываем в правый параметр
                        else if (_right == null)
                        {
                            _right = nameParam;
                            operand = operand.Substring(i);
                            break;
                        }
                    }
                    i--;
                }
            }

            //Проверяем, что все есть в неравенстве
            if(_left == null || _oper == null || _right == null)
            {
                Console.WriteLine("Can't parse IF line");
                return false;
            }
            //Проверяем, что операнд не пустой
            else if(string.IsNullOrEmpty(operand))
            {
                Console.WriteLine("Not found IF operand");
                return false;
            }

            int index = operand.IndexOf(' ', 1);
            //Парсим команду в операнде
            string command;
            if (index != -1)
            {
                command = operand.Substring(0, index + 1);
            }
            else
            {
                command = operand;
            }

            if (command.ToUpper() != command)
            {
                Console.WriteLine($"Row contain lower characters in command {command}");
                return false;
            }

            operand = operand.Remove(0, index + 1);
            //Формируем команду
            if (!CompilerFactory.GetBasicCommand(_compilerFactory, command, operand, out _basicCommand))
            {
                return false;
            }

            return true;
        }

        public bool Build(Dictionary<string, int> parameters, ref int i, out string line)
        {
            line = "";
            //Проверяем, создали ли уже ячейку для параметра
            if (!parameters.ContainsKey(_left))
            {
                int resValue = 0;
                //Параметр может быть числом
                if (char.IsDigit(_left[0]))
                {
                    if (!int.TryParse(_left, out resValue))
                    {
                        Console.WriteLine($"Parameter can't contain number: {_left}");
                        return false;
                    }
                }

                //Добавляем ячейку для параметра
                parameters.Add(_left, i);
                line += $"{i++} = {resValue} \n";
            }

            //Проверяем, создали ли уже ячейку для параметра
            if (!parameters.ContainsKey(_right))
            {
                int resValue = 0;
                //Параметр может быть числом
                if (char.IsDigit(_right[0]))
                {
                    if (!int.TryParse(_right, out resValue))
                    {
                        Console.WriteLine($"Parameter can't contain number: {_right}");
                        return false;
                    }
                }

                //Добавляем ячейку для параметра
                parameters.Add(_right, i);
                line += $"{i++} = {resValue} \n";
            }

            string trueLine;
            int falseIndex;
            //IF A ? B
            switch (_oper)
            {
                //Загружаем A и отнимаем B
                //Проверяем JNEG и JZ -> отправляем на следующий блок
                //Иначе выполняем True
                //Следующий блок
                case ">":
                    line += $"{i++} LOAD {parameters[_left]} \n";
                    line += $"{i++} SUB {parameters[_right]} \n";

                    falseIndex = i + 2;
                    if (!_basicCommand!.Build(parameters, ref falseIndex, out trueLine))
                    {
                        return false;
                    }
                    line += $"{i} JNEG {falseIndex} \n";
                    line += $"{i + 1} JZ {falseIndex} \n";
                    line += trueLine;
                    i = falseIndex;
                    break;
                //Загружаем A и отнимаем B
                //Проверяем JNEG -> отправляем на следующий блок
                //Иначе выполняем True
                //Следующий блок
                case ">=":
                    line += $"{i++} LOAD {parameters[_left]} \n";
                    line += $"{i++} SUB {parameters[_right]} \n";

                    falseIndex = i + 1;
                    if (!_basicCommand!.Build(parameters, ref falseIndex, out trueLine))
                    {
                        return false;
                    }
                    line += $"{i} JNEG {falseIndex} \n";
                    line += trueLine;
                    i = falseIndex;
                    break;
                //Загружаем A и отнимаем B
                //Проверяем JZ -> отправляем блок True
                //Иначе GOTO на блок следующий блок
                //Блок True
                //Следующий блок
                case "==":
                    line += $"{i++} LOAD {parameters[_left]} \n";
                    line += $"{i++} SUB {parameters[_right]} \n";
                    line += $"{i} JZ {i + 2} \n";
                    i++;

                    falseIndex = i + 1;
                    if (!_basicCommand!.Build(parameters, ref falseIndex, out trueLine))
                    {
                        return false;
                    }
                    line += $"{i} JUMP {falseIndex} \n";
                    line += trueLine;
                    i = falseIndex;
                    break;
                //Загружаем A и отнимаем B
                //Проверяем JNEG и JZ -> отправляем блок True
                //Иначе GOTO на блок следующий блок
                //Блок True
                //Следующий блок
                case "<=":
                    line += $"{i++} LOAD {parameters[_left]} \n";
                    line += $"{i++} SUB {parameters[_right]} \n";
                    line += $"{i} JNEG {i + 3} \n";
                    line += $"{i + 1} JZ {i + 3} \n";
                    i += 2;

                    falseIndex = i + 1;
                    if (!_basicCommand!.Build(parameters, ref falseIndex, out trueLine))
                    {
                        return false;
                    }
                    line += $"{i} JUMP {falseIndex} \n";
                    line += trueLine;
                    i = falseIndex;
                    break;
                //Загружаем A и отнимаем B
                //Проверяем JNEG -> отправляем блок True
                //Иначе GOTO на блок следующий блок
                //Блок True
                //Следующий блок
                case "<":
                    line += $"{i++} LOAD {parameters[_left]} \n";
                    line += $"{i++} SUB {parameters[_right]} \n";
                    line += $"{i} JNEG {i + 2} \n";
                    i++;

                    falseIndex = i + 1;
                    if(!_basicCommand!.Build(parameters, ref falseIndex, out trueLine))
                    {
                        return false;
                    }
                    line += $"{i} JUMP {falseIndex} \n";
                    line += trueLine;
                    i = falseIndex;
                    break;
                default:
                    line = "";
                    Console.WriteLine($"Inequality is not found: {_oper}");
                    return false;
            }
            return true;
        }
    }
}