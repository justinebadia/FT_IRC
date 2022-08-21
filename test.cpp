#include <iostream>
#include <fstream>

#include <string>

std::string getFileContents (std::ifstream&);            //Gets file contents

int main(int argc, char *argv[])
{

    std::ifstream Reader ("tobastine.txt");             //Open file
    std::ifstream tanuki ("tanuki.txt");             //Open file

    std::string Art = getFileContents (Reader);       //Get file

	 Art += getFileContents (tanuki);
    
    std::cout << Art << std::endl;               //Print it to the screen

    Reader.close ();                           //Close file

    return 0;
}

std::string getFileContents (std::ifstream& File)
{
    std::string Lines = "";        //All lines
    
    if (File)                      //Check if everything is good
    {
	while (File.good ())
	{
	    std::string TempLine;                  //Temp line
	    std::getline (File , TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character
	    
	    Lines += TempLine;                     //Add newline
	}
	return Lines;
    }
    else                           //Return error
    {
	return "ERROR File does not exist.";
    }
}