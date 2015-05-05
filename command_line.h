#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <fstream>
#include "assemble.h"
#include "passtwo.h"

using namespace std;


class shell
{

    string command;
    int size;
    string param_1;
    string param_2;
    string param_3;
public:

    void runShell()
    {
        int flag = 0 ;
        //shell executes while flag is not equal to 1

        while(flag != 1 )
        {
            size = 1;
            cout << ">> ";

            getline(cin, command);
	    upper(command);

            int x = 1;
            int y = 0;
            int z = command.size();


            while (command[y] == ' ')
            {
                y++;
            }

            //This part gets the string and compares it one character at a time.
            while (true)
            {
                //I have the size of the input to ensure I don't read in garbage
                if (y == z)
                    break;
                //If I detect a space, I go to the next storage space
                else if (command[y] == ' ')
                {
                    x++;
                    size++;
                }
                //This is where I store my commands
                else if (x == 1)
                {
                    param_1 += command[y];
                }
                else if (x == 2)
                {
                    param_2 += command[y];
                }
                else if (x == 3)
                {
                    param_3 += command[y];
                }
                y++;
            }

            //checks to ensure that all conditions are met
            if (param_1 == "LOAD")
            {
                if (size != 2)
                    cout<<"Invalid Parameters"<<endl<<endl;
                else
                {
                    load(param_2);
                    flag = 0;
                }
            }

            //reaches execute stub
            else if (param_1 == "EXECUTE")
            {
                execute();
                flag = 0;
            }

            //reaches debug stub
            else if (param_1 == "DEBUG")
            {
                debug();
                flag = 0;
            }

            //reaches dump stub
            else if (param_1 == "DUMP")
            {
                //Checks to ensure valid params are met
                if (size != 3)
                    cout<<"Invalid Parameters:"<<endl<<endl;
                else
                {
                    dump(param_2, param_3);
                    flag = 0;
                }
            }
            else if (param_1 == "HELP")
            {
                if (size < 2)
                {
                    //helps the user to navigate the system
                    cout << "Please type:" << endl << endl;
                    cout << "load:		loads progam into memory" << endl
                         << "execute:	executes program from memory" << endl
                         << "debug:		executes program in debug mode" << endl
                         << "dump:		dumps memory" << endl
                         << "assemble:	assembles program using SIC machine language" << endl
                         << "directory:	list directory" << endl
                         << "exit:		exit interpreter" << endl;
                }
                else if (size > 2)
                    cout<<"Invalid Parameters"<<endl<<endl;
                else
                {
                    //after user inputs for help, systems outputs proper help file
                    help(param_2);
                    flag = 0;
                }
            }
            else if (param_1 == "ASSEMBLE")
            {
                //reaches assemble stub
                if (size != 2)
                    cout<<"Invalid Parameters"<<endl<<endl;
                else
                {
                    string total_l = assemble(param_2);
                    if (!total_l.empty())
                        passtwo(total_l);
                        flag = 0;
                }
            }
            //Essentially all the same Dir, ls, and directory
            else if (param_1 == "DIRECTORY")
            {
                //displays directory
                directory();
                flag = 0;
            }
            else if (param_1 == "DIR")
            {
                //displays directory
                directory();
                flag = 0;
            }
            else if (param_1 == "LS")
            {
                //displays directory
                directory();
                flag = 0;
            }
            else if (param_1 == "CLEAR")
            {
                //clears the screen
                system("clear");
                flag = 0;
            }
            else if (param_1 == "EXIT")
            {
                //exits shell
                flag = 1;
            }
            else
            {
                cout<<"Command does not exist" << endl ;
                flag = 0;
            }
            
            param_1.clear();
            param_2.clear();
            param_3.clear();
        }
    }

    //most of this next section is simply stubs or they don't work......yet

    void load(string file)
    {
        //load file
        cout<<"Now loading "<< file <<endl<<endl;

        //loading(param_2);
    }

    void execute()
    {
        //executes command
        cout<<"You have reached the execute command"<<endl ;

    }

    void debug()
    {
        //for now, just a stub but debugs
        cout<<"You have reached the debug command"<<endl<<endl;
    }

    void dump(string start, string end)
    {
        //for now, just a stub but dumps
        cout<<"Dumping from "<< start << " to " << end <<endl<<endl;

    }

    void help(string file)
    {
        //this function handles the help for whatever the user inputs
        cout<<"Available Commands: " <<endl;

        if (file == "LOAD")
        {
            cout<<"Loads file into memory"<<endl<<endl;
        }
        else if (file == "EXECUTE")
        {
            cout<<"Executes file in memory"<<endl<<endl;
        }
        else if (file == "DEBUG")
        {
            cout<<"Executes program in debug mode"<<endl<<endl;
        }
        else if (file == "DUMP")
        {
            cout<<"Dumps contents of specified memory" << endl ;
        }
        else if (file == "ASSEMBLE")
        {
            cout<<"Assembles source code"<<endl<<endl;
        }
        else if (file == "DIRECTORY")
        {
            cout<<"Display contents of current directory"<<endl<<endl;
        }
        else if (file == "EXIT")
        {
            cout<<"Exits program"<<endl<<endl;
        }
        else
        {
            cout<<"Command does not exist"<<endl<<endl;
        }
    }

    void directory()
    {

        system("ls");
    }

};
