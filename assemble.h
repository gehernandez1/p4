#include <fstream>
using namespace std;

class label
{
public:
    string sym;
    string address;
};

void upper(string &input)
{

    for (int i = 0; i < static_cast<int>(input.length()); i++)
    {
        input[i] = toupper(input[i]);
    }
}

void lower(string &input)
{

    for (int i = 0; i < static_cast<int>(input.length()); i++)
    {
        input[i] = tolower(input[i]);
    }
}

//Converts a Hex number to decimal
int hexToDec(string hexnum)
{
    stringstream converter(hexnum);
    unsigned int value = 0;
    converter >> hex >> value;
    return value;
}

//converts a decimal number to string hex
string decToHex(int hexnum)
{
    stringstream ss;
    ss << hex << hexnum;

    string n;
    ss >> n;
    return n;
}

//symbol table
bool symboltable(string & labels, string & address, vector<label> & symbol)
{
    if (symbol.empty())
    {
        label lbl;
        lbl.sym = labels;
        lbl.address = address;
        symbol.push_back(lbl);

        return false;
    }
    else
    {
        for (unsigned int i = 0; i < symbol.size(); i++)
        {
            if (symbol[i].sym == labels)
                return true;
        }

        label lbl;
        lbl.sym = labels;
        lbl.address = address;
        symbol.push_back(lbl);

        return false;
    }
}

//returns the opcode value of the mnemonic
string mnemonics(string mnemonic)
{
    if (mnemonic == "ADD")
        return "18";

    if (mnemonic == "AND")
        return "58";

    if (mnemonic == "BYTE")
        return "00";

    if (mnemonic == "COMP")
        return "28";

    if (mnemonic == "DIV")
        return "24";

    if (mnemonic == "J")
        return "3C";

    if (mnemonic == "JEQ")
        return "30";

    if (mnemonic == "JGT")
        return "34";

    if (mnemonic == "JLT")
        return "38";

    if (mnemonic == "JSUB")
        return "48";

    if (mnemonic == "LDA")
        return "00";

    if (mnemonic == "LDCH")
        return "50";

    if (mnemonic == "LDL")
        return "08";

    if (mnemonic == "LDX")
        return "04";

    if (mnemonic == "MUL")
        return "20";

    if (mnemonic == "OR")
        return "44";

    if (mnemonic == "RESB")
        return "00";

    if (mnemonic == "RESW")
        return "00";

    if (mnemonic == "RD")
        return "D8";

    if (mnemonic == "STA")
        return "0C";

    if (mnemonic == "STCH")
        return "54";

    if (mnemonic == "STL")
        return "14";

    if (mnemonic == "STX")
        return "10";

    if (mnemonic == "SUB")
        return "1C";

    if (mnemonic == "TD")
        return "E0";

    if (mnemonic == "TIX")
        return "2C";

    if (mnemonic == "WD")
        return "DC";

    if (mnemonic == "WORD")
        return "00";

    else
        return "Not Found";
}

void parseLine(string name, string& str1, string& str2, string& str3)
{
    int size = 0;
    bool flag = false;
    int x = 1;
    int y = 0;
    int z = name.size();

    //compares one character at a time.
    while (true)
    {
        if (y == z)
            break;
        else if (name[y] == '.')
            break;
        else if (name[y] == '\t' || name[y] == ' ')
        {
            if (flag == true)
            {
                flag = true;
            }
            else
            {
                x++;
                flag = true;
            }
        }
        //store the commands
        else if (x == 1)
        {
            str1 += name[y];
        }
        else if (x == 2)
        {
            str2 += name[y];
            flag = false;
        }
        else if (x == 3)
        {
            str3 += name[y];
            flag = false;
        }
        y++;
    }
}

string assemble(string file)
{

    lower(file);
    cout << "The following file will be assembled: " << file << endl << endl;

    //read in file
    ifstream infile;
    infile.open(file.c_str());
    //open or create intermediate
    ofstream intermediate;
    intermediate.open("intermediate.txt");
    //open or create symbol table
    ofstream symbolTbl;
    symbolTbl.open("labels.txt");

    //-------------------------------------------------------------------------------

    //Store lines
    string line_input;

    //keeps track of counter
    string counter;
    int deccounter = 0;

    //symbol table
    vector<string> list;
    vector<label> symbol;

    //--------------------------------------------------------------------------------

    int start = 0;
    int length = 0;
    string hexlength;
    string param1, param2, param3;


    //while not end of file loop through each line
    while (!infile.eof())
    {
        if (!infile)
        {

            break;
        }

        param1.clear();
        param2.clear();
        param3.clear();
        line_input.clear();

        //gets the line from the file
        getline(infile, line_input);

        //break line into tokens
        parseLine(line_input, param1, param2, param3);

        //ignore comments or empty lines
        if (!param2.empty())
        {
            //writes line to the intermeditate file
            intermediate << param1 << '\t' << param2 << '\t' << param3 << endl;

            if (param2 != "START" && param2 != "RSUB" && param2 != "END")
            {
                intermediate << counter << endl << mnemonics(param2) << endl << param3 << endl;

                //Adds label to symbol table
                if (symboltable(param1, counter, symbol) == false && !param1.empty())
                {
                    symbolTbl << param1 << '\t' << counter << '\t' << endl;
                }
            }
            //is symbol
            if (!param1.empty())
            {
                //if directive is START
                if (param2 == "START")
                {
                    start = hexToDec(param3);
                    deccounter = hexToDec(param3);
                    counter = decToHex(deccounter);

                    //add lable to symtbl
                    if (symboltable(param1, counter, symbol) == false)
                    {
                        symbolTbl << param1 << '\t' << counter << '\t' << endl;
                    }
                }
                else
                {
                    //checks the mnemonics to assign addresses and ensure it is valid
                    string mnemonic, mnAddrs;
                    mnemonic = param2;
                    mnAddrs = mnemonics(mnemonic);

                    //if found
                    if (mnemonic != "ERROR!")
                    {
                        //increment deccounter according to directive
                        if (mnemonic == "WORD")
                        {
                            deccounter = hexToDec(counter);
                            deccounter += 3;
                            counter = decToHex(deccounter);
                        }

                        else if (mnemonic == "RESB")
                        {
                            char x[100];
                            int y = 0;
                            int z = 0;
                            y = param3.size();
                            while (z <= y)
                            {
                                x[z] = param3[z];
                                z++;
                            }
                            deccounter = hexToDec(counter);
                            deccounter += atoi(x);
                            counter = decToHex(deccounter);
                        }

                        else if (mnemonic == "RESW")
                        {
                            char x[100];
                            int y = 0;
                            int z = 0;
                            y = param3.size();
                            while (z <= y)
                            {
                                x[z] = param3[z];
                                z++;
                            }
                            deccounter = hexToDec(counter);
                            deccounter += (3 * atoi(x));
                            counter = decToHex(deccounter);
                        }

                        else if (mnemonic == "BYTE")
                        {
                            int y = 0;
                            y = param3.size() - 3;
                            if (param3[0] == 'C' && param3[1] == '\'' && param3[(param3.size() - 1)] == '\'')
                            {
                                deccounter = hexToDec(counter);
                                deccounter += y;
                                counter = decToHex(deccounter);
                            }
                            else if (param3[0] == 'X' && param3[1] == '\'' && param3[(param3.size() - 1)] == '\'')
                            {
                                deccounter = hexToDec(counter);
                                deccounter += y / 2;
                                counter = decToHex(deccounter);
                            }
                            else
                            {
                                intermediate << "Invalid Parameter: " << param3 << endl;
                            }
                        }

                        else
                        {
                            deccounter = hexToDec(counter);
                            deccounter += 3;
                            counter = decToHex(deccounter);
                        }
                    }
                    else
                    {
                        intermediate << "Illegal Operand Detected: " << mnemonic << endl;
                    }
                }
            }


            //breaks out of the loop since END signals the stop of the program
            else if (param2 == "END")
            {
                length = deccounter - start;
                hexlength = decToHex(length);
                intermediate << hexlength << endl;
                break;
            }
            //check third column
            else
            {
                string mnemonic, mnAddrs;
                mnemonic = param2;
                mnAddrs = mnemonics(mnemonic);

                //if found
                if (mnemonic != "ERROR!")
                {
                    if (mnemonic == "WORD")
                    {
                        deccounter = hexToDec(counter);
                        deccounter += 3;
                        counter = decToHex(deccounter);
                    }

                    else if (mnemonic == "RESB")
                    {
                        char x[100];
                        int y = 0;
                        int z = 0;
                        y = param3.size();
                        while (z <= y)
                        {
                            x[z] = param3[z];
                            z++;
                        }
                        deccounter = hexToDec(counter);
                        deccounter += atoi(x);
                        counter = decToHex(deccounter);
                    }

                    else if (mnemonic == "RESW")
                    {
                        char x[100];
                        int y = 0;
                        int z = 0;
                        y = param3.size();
                        while (z <= y)
                        {
                            x[z] = param3[z];
                            z++;
                        }
                        deccounter = hexToDec(counter);
                        deccounter += (3 * atoi(x));
                        counter = decToHex(deccounter);
                    }

                    else if (mnemonic == "BYTE")
                    {
                        int y = 0;
                        y = param3.size() - 3;
                        if (param3[0] == 'C' && param3[1] == '\'' && param3[(y - 1)] == '\'')
                        {
                            deccounter = hexToDec(counter);
                            deccounter += y;
                            counter = decToHex(deccounter);
                        }
                        else if (param3[0] == 'X' && param3[1] == '\'' && param3[(y - 1)] == '\'')
                        {
                            deccounter = hexToDec(counter);
                            deccounter += y / 2;
                            counter = decToHex(deccounter);
                        }
                        else
                        {
                            intermediate << "Invalid Parameter: " << param3 << endl;
                        }
                    }

                    else
                    {
                        deccounter = hexToDec(counter);
                        deccounter += 3;
                        counter = decToHex(deccounter);
                    }
                }
                else
                {
                    intermediate << "Illegal Operand Detected: " << mnemonic << endl;
                }

            }
        }
    }
    //symbol table length
    symbolTbl << endl << "Length: " << hexlength;

    //close files
    infile.close();
    intermediate.close();
    symbolTbl.close();

    return hexlength;
}
