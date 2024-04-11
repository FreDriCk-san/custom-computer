using System;
using System.IO;

namespace SimpleBasicCompiler
{
    internal class Program
    {
        static bool TryParse(string fileName, out CompilerFactory? factory)
        {
            factory = null;
            if (!File.Exists(fileName))
            {
                Console.WriteLine("File not exist");
                return false;
            }

            factory = new CompilerFactory();

            int preRowNumber = -1;
            var lines = File.ReadAllLines(fileName);
            //Парсим по строкам
            for (int i = 0; i < lines.Length; i++)
            {
                var line = lines[i];
                //Находим разделитель между номером строки и командой
                int index = line.IndexOf(' ');
                if (index == -1)
                {
                    Console.WriteLine($"Row {i + 1}. Can't parse line");
                    return false;
                }

                //Проверяем, что номер строки число
                string number = line.Substring(0, index);
                if (!int.TryParse(number, out int rowNumber))
                {
                    Console.WriteLine($"Row {i + 1}. Row number is not corrected");
                    return false;
                }

                //Проверяем,что номер строки всегда расчет
                if(preRowNumber >= rowNumber)
                {
                    Console.WriteLine($"Row {i + 1}. Row number is not corrected");
                    return false;
                }
                preRowNumber = rowNumber;

                line = line.Remove(0, index);

                //Находим разделитель между командой и операндом
                index = line.IndexOf(' ', 1);

                //Некоторые команды не имею операнд (END)
                string command;
                if (index != -1)
                {
                    command = line.Substring(0, index + 1);
                }
                else
                {
                    command = line;
                }

                //Проверяем, что все команды написаны с большом регистре
                if (command.ToUpper() != command)
                {
                    Console.WriteLine($"Row {i + 1}. Row contain lower characters in command {command}");
                    return false;
                }

                line = line.Remove(0, index + 1);
                //Парсим строку
                if (!factory.Parse(rowNumber, command, line))
                {
                    return false;
                }
            }

            return true;
        }

        public static void Main(string[] args)
        {
            //args = new string[2] { "..\\..\\..\\testBasic.txt", "..\\..\\..\\Output.txt" };
            if (args.Length != 2)
            {
                Console.WriteLine("Bruh");
                return; 
            }

            string fileName = args[0];
            if(!TryParse(fileName, out var factory))
            {
                return;
            }

            if (factory != null)
            {
                string fileSave = args[1];
                factory.Save(fileSave);
            }
        }
    }
}
