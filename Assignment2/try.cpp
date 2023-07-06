#include "MIPS_Processor.hpp"
using namespace std;

void printVectorOfVector(const vector<vector<string>>& vec)
{
    for (const auto& v : vec) {  // loop over the outer vector
        for (const auto& s : v) {  // loop over the inner vector
            cout << s << " | ";  // print each string separated by a space
        }
        cout << endl;  // print a newline after each inner vector
    }
}

int main(){
    std::ifstream file("sample.asm");
	MIPS_Architecture *mips;
	if (file.is_open())
		mips = new MIPS_Architecture(file);
	else
	{
		std::cerr << "File could not be opened. Terminating...\n";
		return 0;
	}

	mips->constructCommands(file);
    printVectorOfVector(mips->commands);
	return 0;
}