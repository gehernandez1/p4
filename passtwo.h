
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace std;

//for helping with the variable assignments later on in the program
class symLabel
{
public:
	string code;
	string address;
};


class interlabel
{
public:
	string opcode;
	string location_counter;
	string objectcode;
};

//*******************************
string line_input;
string symbol_input;
vector<symLabel> symbol;
vector<interlabel> table;
int numbers[32];
string startingAddrs;
//********************************

//function declaration
void parseTheLine(string name, string& storage_1, string& storage_2, string& storage_3);
bool symbolTbl(string & labl, string & address, vector<symLabel> & symbol);
string binary(int x);
void objectwrite(vector<interlabel> table);

//sort labels
void tablelist(string & labl, string & address, string & object, vector<interlabel> & symbol)
{
	//if empty
	if (table.empty())
	{
		interlabel l;
		l.opcode = labl;
		l.location_counter = address;
		l.objectcode = object;
		symbol.push_back(l);
	}
	//if not empty
	else
	{
		for (unsigned int i = 0; i < table.size(); i++)
		{
			if (table[i].opcode == labl)
				break;
		}

		interlabel l;
		l.opcode = labl;
		l.location_counter = address;
		l.objectcode = object;
		symbol.push_back(l);
	}
}


void passtwo(string total_l)
{
	/*Opens the files for me*/
	ifstream interfile;
	interfile.open("intermediate.txt");

	ifstream symbolin;
	symbolin.open("symboltable.txt");

	ofstream listing;
	listing.open("listing.txt");

	ofstream objectfile;
	objectfile.open("object.txt");

	/*Local Variable assignement*/
	string param_1, param_2, param_3, param_4, param_5, param_6;
	int l_line = 5;
	unsigned int i = 0;
	string param, address, storage;

	//We load up the file beforehand so that we don't get tied up later
	while (!symbolin.eof())
	{
		getline(symbolin, symbol_input);
		parseTheLine(symbol_input, param_4, param_5, param_6);
		symbolTbl(param_4, param_5, symbol);
		param_4.clear();
		param_5.clear();
	}

	//to prevent unwanted results from messing with output
	table.clear();

	while (!interfile.eof())
	{
		//cleans up variables
		param_1.clear();
		param_2.clear();
		param_3.clear();
		param_4.clear();
		param_5.clear();
		param_6.clear();
		storage.clear();
		line_input.clear();
		symbol_input.clear();

		/*Grabs from intermediate file line by line*/
		getline(interfile, line_input);
		parseTheLine(line_input, param_1, param_2, param_3);

		if (!param_2.empty())
		{
			/*When we read in the lines and start is detected*/
			if (param_2 == "START")
			{
				/*Writes to the listing file*/
				listing << l_line << '\t' << param_3 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << endl;
				objectfile << "H";
				int x = 0;
				int y = 0;
				while (x < 6){
					if (x < param_1.length()){
						objectfile << param_1[x];
						storage += param_1[x];
					}
					else
					{
						objectfile << ' ';
					}
					x++;
				}
				/*for sorting out later*/
				tablelist(param_2, param_3, storage, table);
				/*Writes to the object file*/
				objectfile << std::setfill('0') << std::setw(6) << param_3;
				/*Start address*/
				startingAddrs = param_3;
				objectfile << std::setfill('0') << std::setw(6) << total_l << endl << endl;
				l_line += 5;
			}
			/*Handles the byte directive*/
			else if (param_2 == "BYTE")
			{
				char holder[6];
				int num = 2;
				holder[0] = ' ';
				holder[1] = ' ';
				holder[2] = ' ';
				holder[3] = ' ';
				holder[4] = ' ';
				holder[5] = ' ';
				//******************************************************
				if (param_3[0] == 'C' && param_3[1] == '\'' && param_3[(param_3.size() - 1)] == '\'')
				{
					if (param_3[2] == 'E' && param_3[3] == 'O' && param_3[4] == 'F')
					{
						holder[0] = '4';
						holder[1] = '5';
						holder[2] = '4';
						holder[3] = 'f';
						holder[4] = '4';
						holder[5] = '6';
						for (int y = 0; y < 6; y++){
							storage += holder[y];
						}
					}
					else
					{
						int i = 0;
						while (param_3[num] != '\'')
						{
							holder[i] = param_3[num];
							storage += holder[i];
							num++;
							i++;
						}
					}
				}
				else if (param_3[0] == 'X' && param_3[1] == '\'' && param_3[(param_3.size() - 1)] == '\'')
				{
					if (param_3[2] == 'E' && param_3[3] == 'O' && param_3[4] == 'F')
					{
						holder[0] = '4';
						holder[1] = '5';
						holder[2] = '4';
						holder[3] = 'f';
						holder[4] = '4';
						holder[5] = '6';
						for (int y = 0; y < 6; y++){
							storage += holder[y];
						}
					}
					else
					{
						int i = 0;
						while (param_3[num] != '\'')
						{
							holder[i] = param_3[num];
							storage += holder[i];
							num++;
							i++;
						}
					}
				}
				//*******************************************************
				/*Handles the writing and storage of the current data*/
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				listing << l_line << '\t' << param_4 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << storage << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
			}
			/*Handles the RSUB Directive due to special nature*/
			else if (param_2 == "RSUB")
			{
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				string storage;
				storage = "4C0000";
				listing << l_line << '\t' << param_4 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << storage << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
			}
			/*Handles the Word Directive*/
			else if (param_2 == "WORD")
			{
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				/*For handling the object code*/
				int temp = atoi(param_3.c_str());
				string storage = decToHex(temp);
				//*************************************
				listing << l_line << '\t' << param_4 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << "00" << std::setfill('0') << std::setw(4) << storage << '\t' << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
			}
			/*Handles the RESW directive*/
			else if (param_2 == "RESW")
			{
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				listing << l_line << '\t' << param_4 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
			}
			/*Handles the RESB directive*/
			else if (param_2 == "RESB")
			{
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				listing << l_line << '\t' << param_4 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
			}
			/*For when the program hits the end of the file*/
			else if (param_2 == "END")
			{
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				listing << l_line << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
				break;
			}
			/*Handles all other directives*/
			else
			{
				i = 0;
				bool flag = false;
				/*For handling buffer directive*/
				if (param_3[0] == 'B' && param_3[1] == 'U' && param_3[2] == 'F' && param_3[3] == 'F' && param_3[4] == 'E' && param_3[5] == 'R' && param_3[6] == ',' && param_3[7] != ' ')
				{
					param_3 = "BUFFER";
					flag = true;
				}
				/*All other directives*/
				while (i < symbol.size())
				{
					if (symbol[i].code == param_3)
					{
						param = symbol[i].code;
						address = symbol[i].address;
						i = 0;
						if (flag == true)
						{
							address = binary(hexToDec(address));
						}
						break;
					}
					else
					{
						i++;
					}
					if (i > symbol.size())
					{
						i = 0;
						break;
					}
				}
				getline(interfile, line_input);
				parseTheLine(line_input, param_4, param_5, param_6);
				string storage;
				storage = mnemonics(param_2) + address;
				listing << l_line << '\t' << param_4 << '\t' << param_1 << '\t' << param_2 << '\t' << param_3 << '\t' << '\t'<< storage << endl;
				tablelist(param_2, param_4, storage, table);
				l_line += 5;
				param.clear();
				address.clear();
			}
		}
	}

	/*After the listing file is done and completed
	Variables for the next operation*/
	int y = 0;
	int z = 1;
	int num[10];
	/*Clears the array for initial use*/
	for (int a = 0; a < 10; a++){
		num[a] = 0;
	}
	string a[10];
	string b[2];
	bool flag = false;
	bool trigger = false;

	/*Handles the writing to the object file*/
	while (true) {
		if (table[z].opcode == "END"){
			if (trigger == false){
				trigger = true;
				goto jump;
			}
			break;
		}
		/*Writes the line's start location to the object file*/
		if (flag == false) {
			objectfile << 'T' << std::setfill('0') << std::setw(6) << table[z].location_counter;
			b[0] = table[z].location_counter;
			flag = true;
		}
		/*Handles when the buffer is full and writes out the object line*/
		else if (y < 10) {
			/*Fills the buffer*/
			a[y] = table[z].objectcode;
			if (table[z].opcode == "WORD"){
				num[y] = 1;
			}
			//**************************************************
			/*Grabs the end location for the current line*/
			if (table[z + 1].opcode == "END"){
				b[1] = table[z].location_counter;
				b[1] = decToHex(hexToDec(b[1]) + 1);
			}
			else if (table[z].opcode != "RESB" && table[z].opcode != "RESW"){
				b[1] = table[z + 1].location_counter;
			}
			//***************************************************
			y++;
			z++;
		}
		else
		{
			jump:
			y = 0;
			flag = false;
			string length;
			/*Gets the lenght of the line*/
			length = decToHex(hexToDec(b[1]) - hexToDec(b[0]));
			objectfile << std::setfill('0') << std::setw(2) << length;
			/*Writes out the buffer*/
			while (y < 10) {
				if (num[y] == 1){
					objectfile << std::setfill('0') << std::setw(6) << a[y];
				}
				else{
					objectfile << a[y];
				}
				y++;
			}
			objectfile << endl;
			y = 0;
			/*Clears the buffers for next line*/
			while (y < 10){
				a[y].clear();
				if (y < 2)
				b[y].clear();
				y++;
			}
			y = 0;
			for (int a = 0; a < 10; a++){
				num[a] = 0;
			}
		}
	}

	/*Ends the writes by writing the last line with the start location of the file*/
	objectfile << endl << 'E' << std::setfill('0') << std::setw(6) << startingAddrs;

 	interfile.close();
	symbolin.close();
	listing.close();
	objectfile.close();
}

void parseTheLine(string name, string& storage_1, string& storage_2, string& storage_3)
{
	int size = 0;
	bool flag = false;
	int x = 1;
	int y = 0;
	int z = name.size();
	//This part gets the string and compares it one character at a time.
	while (true)
	{
		//I have the size of the input to ensure I don't read in garbage
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
		//This is where I store my commands
		else if (x == 1)
		{
			storage_1 += name[y];
		}
		else if (x == 2)
		{
			storage_2 += name[y];
			flag = false;
		}
		else if (x == 3)
		{
			storage_3 += name[y];
			flag = false;
		}
		y++;
	}
}

bool symbolTbl(string & labl, string & address, vector<symLabel> & symbol)
{
	if (symbol.empty())
	{
		symLabel l;
		l.code = labl;
		l.address = address;
		symbol.push_back(l);

		return false;
	}
	else
	{
		for (unsigned int i = 0; i < symbol.size(); i++)
		{
			if (symbol[i].code == labl)
				return true;
		}

		symLabel l;
		l.code = labl;
		l.address = address;
		symbol.push_back(l);

		return false;
	}
}

string binary(int x) {
	int a[16];
	a[0] = 1;
	int z = 1;
	while (z < 16){
		a[z] = 0;
		z++;
	}
	z--;
	while (x>0) {
		a[z] = x % 2;
		z--;
		x = x / 2;
	}
	int b = 0;
	z = 0;
	while (z < 16) {
		if (a[z] == 1) {
			b = b + pow(2, (15 - z));
		}
		z++;
	}
	return decToHex(b);
}
