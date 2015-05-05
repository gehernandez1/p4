#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include "command_line.h"
#include "sicengine.h"

using namespace std;

int main()
{

    //declare shell class
    shell instance;

    //initialize SIC machine simulator
    SICInit();

    //initialize shell
    instance.runShell();

    return 0;
}
