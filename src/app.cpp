#include "app.h"
#include "mainFrame.h"
#include "textUtils.h"

int OnCmdLine(wxCmdLineParser &parser, int argc, char **argv)
{
    // Add command line options
    parser.AddOption("palindrome", "p", "Check if text is a palindrome", wxCMD_LINE_VAL_STRING);
    parser.AddOption("replace", "r", "Replace 'foo' with 'bar' in text", wxCMD_LINE_VAL_STRING);
    parser.AddOption("reverse", "v", "Reverse text", wxCMD_LINE_VAL_STRING);
    parser.AddOption("sortlines", "s", "Sort lines by length", wxCMD_LINE_VAL_STRING);
    parser.AddOption("file", "f", "Read input from a file instead of a string argument", wxCMD_LINE_VAL_STRING);
    parser.AddOption("outputfile", "o", "Write output to a file instead of stdout", wxCMD_LINE_VAL_STRING);
    parser.AddSwitch("help", "h", "Show help message"); // Added help switch

    int parseResult = parser.Parse();

    if (parseResult == 0) // Successful parsing
    {
        wxString operationValue;
        wxString operationFound;
        bool operationRequested = false;

        if (parser.Found("palindrome", &operationValue))
        {
            operationFound = "palindrome";
            operationRequested = true;
        }
        else if (parser.Found("replace", &operationValue))
        {
            operationFound = "replace";
            operationRequested = true;
        }
        else if (parser.Found("reverse", &operationValue))
        {
            operationFound = "reverse";
            operationRequested = true;
        }
        else if (parser.Found("sortlines", &operationValue))
        {
            operationFound = "sortlines";
            operationRequested = true;
        }

        wxString inputFilePath;
        bool inputFileMode = parser.Found("file", &inputFilePath);

        wxString outputFilePath;
        bool outputToFileMode = parser.Found("outputfile", &outputFilePath);

        if (parser.Found("help"))
        {
            parser.Usage();
            return 0;
        }

        if (operationRequested)
        {
            wxString inputText;
            if (inputFileMode)
            {
                if (inputFilePath.IsEmpty())
                {
                    std::cerr << "Error: --file requires a file path." << std::endl;
                    return 1;
                }
                wxTextFile file;
                if (!file.Open(inputFilePath))
                {
                    std::cerr << "Error: Could not open input file for reading: " << std::string(inputFilePath.ToUTF8()) << std::endl;
                    return 1;
                }
                for (size_t i = 0; i < file.GetLineCount(); ++i)
                {
                    inputText += file[i];
                    if (i < file.GetLineCount() - 1)
                    {
                        inputText += "\n";
                    }
                }
                file.Close();
            }
            else
            {
                if (operationValue.IsEmpty())
                {
                    std::cerr << "Error: No input text or file specified for operation." << std::endl;
                    return 1;
                }
                inputText = operationValue;
            }

            std::string result;
            bool boolResult = false;

            if (operationFound == "palindrome")
            {
                boolResult = CheckTextForPalindrome(inputText);
                result = boolResult ? "1" : "0";
            }
            else if (operationFound == "replace")
            {
                result = std::string(ProcessReplaceFooBar(inputText).ToUTF8());
            }
            else if (operationFound == "reverse")
            {
                result = std::string(ProcessReverseText(inputText).ToUTF8());
            }
            else if (operationFound == "sortlines")
            {
                result = std::string(ProcessSortLines(inputText).ToUTF8());
            }

            if (outputToFileMode)
            {
                if (outputFilePath.IsEmpty())
                {
                    std::cerr << "Error: --outputfile requires a file path." << std::endl;
                    return 1;
                }
                wxFileOutputStream outputStream(outputFilePath);
                if (!outputStream.IsOk())
                {
                    std::cerr << "Error: Could not open output file: " << std::string(outputFilePath.ToUTF8()) << std::endl;
                    return 1;
                }
                outputStream.Write(result.c_str(), result.length());
                if (!outputStream.IsOk())
                {
                    std::cerr << "Error: Could not write to output file: " << std::string(outputFilePath.ToUTF8()) << std::endl;
                    return 1;
                }
                std::cout << "Output written to file: " << std::string(outputFilePath.ToUTF8()) << std::endl;
            }
            else
            {
                std::cout << result << std::endl;
            }
            return 0;
        }
        else
        {
            parser.Usage();
            return 1;
        }
    }
    else if (parseResult == -1)
    {
        return 1;
    }

    return -1;
}

bool App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    if (argc > 1)
    { // Check if there are any command-line arguments
        // Manual parsing logic

        wxString manualOperationType = wxEmptyString;
        wxString manualInputValue = wxEmptyString;
        bool manualInputIsFile = false;
        wxString manualOutputFilePath = wxEmptyString;
        bool manualOutputToFile = false;
        bool helpRequested = false;

        if (!wxApp::OnInit())
            return false;

        for (int i = 1; i < argc; ++i)
        {
            wxString arg = argv[i];

            if (arg == "--palindrome" || arg == "-p")
            {
                manualOperationType = "palindrome";
                if (i + 1 < argc && !(wxString(argv[i + 1]) == "--file" || wxString(argv[i + 1]) == "-f"))
                {
                    manualInputValue = argv[++i];
                }
            }
            else if (arg == "--replace" || arg == "-r")
            {
                manualOperationType = "replace";
                if (i + 1 < argc && !(wxString(argv[i + 1]) == "--file" || wxString(argv[i + 1]) == "-f"))
                {
                    manualInputValue = argv[++i];
                }
            }
            else if (arg == "--reverse" || arg == "-v")
            {
                manualOperationType = "reverse";
                if (i + 1 < argc && !(wxString(argv[i + 1]) == "--file" || wxString(argv[i + 1]) == "-f"))
                {
                    manualInputValue = argv[++i];
                }
            }
            else if (arg == "--sortlines" || arg == "-s")
            {
                manualOperationType = "sortlines";
                if (i + 1 < argc && !(wxString(argv[i + 1]) == "--file" || wxString(argv[i + 1]) == "-f"))
                {
                    manualInputValue = argv[++i];
                }
            }
            else if (arg == "--file" || arg == "-f")
            {
                manualInputIsFile = true;
                if (i + 1 < argc)
                {
                    manualInputValue = argv[++i];
                }
                else
                {
                    std::cerr << "Error: --file requires a file path." << std::endl;
                    return false;
                }
            }
            else if (arg == "--outputfile" || arg == "-o")
            {
                manualOutputToFile = true;
                if (i + 1 < argc)
                {
                    manualOutputFilePath = argv[++i];
                }
                else
                {
                    std::cerr << "Error: --outputfile requires a file path." << std::endl;
                    return false;
                }
            }
            else if (arg == "--help" || arg == "-h")
            {
                helpRequested = true;
            }
        }

        if (!manualOperationType.IsEmpty() || helpRequested)
        {
            if (helpRequested)
            {
                std::cout << "Usage: UNotePad <operation> [input_value] [--file <path>] [--outputfile <path>]" << std::endl;
                // ... (rest of help message)
                return false;
            }

            wxString processedInputText;
            if (manualInputIsFile)
            {

                if (manualInputValue.IsEmpty())
                {
                    std::cerr << "Error: --file flag requires a file path." << std::endl;
                    return false;
                }
                wxTextFile file;
                if (!file.Open(manualInputValue))
                {
                    std::cerr << "Error: Could not open input file for reading: " << std::string(manualInputValue.ToUTF8()) << std::endl;
                    return false;
                }
                for (size_t i = 0; i < file.GetLineCount(); ++i)
                {
                    processedInputText += file[i];
                    if (i < file.GetLineCount() - 1)
                    {
                        processedInputText += "\n";
                    }
                }
                file.Close();
            }
            else
            {
                if (manualInputValue.IsEmpty())
                {
                    std::cerr << "Error: No input text provided for the operation." << std::endl;
                    return false;
                }
                processedInputText = manualInputValue;
            }

            std::string result;
            if (manualOperationType == "palindrome")
            {
                result = CheckTextForPalindrome(processedInputText) ? "1" : "0";
            }
            else if (manualOperationType == "replace")
            {
                result = std::string(ProcessReplaceFooBar(processedInputText).ToUTF8());
            }
            else if (manualOperationType == "reverse")
            {
                result = std::string(ProcessReverseText(processedInputText).ToUTF8());
            }
            else if (manualOperationType == "sortlines")
            {
                result = std::string(ProcessSortLines(processedInputText).ToUTF8());
            }

            if (manualOutputToFile)
            {
                if (manualOutputFilePath.IsEmpty())
                {
                    std::cerr << "Error: --outputfile requires a file path." << std::endl;
                    return false;
                }
                wxFileOutputStream outputStream(manualOutputFilePath);
                if (!outputStream.IsOk())
                {
                    std::cerr << "Error: Could not open output file for writing: " << std::string(manualOutputFilePath.ToUTF8()) << std::endl;
                    return false;
                }
                outputStream.Write(result.c_str(), result.length());
                if (!outputStream.IsOk())
                {
                    std::cerr << "Error: Could not write to output file: " << std::string(manualOutputFilePath.ToUTF8()) << std::endl;
                    return false;
                }
                std::cout << "Output written to file: " << std::string(manualOutputFilePath.ToUTF8()) << std::endl;
            }
            else
            {
                std::cout << result << std::endl;
            }
            return false;
        }
    }

    MainFrame *frame = new MainFrame("UNotePad");
    frame->Show(true);
    return true;
}