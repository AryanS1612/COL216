#include "MIPS_Pipelining.hpp"

void printVectorOfVector(const std::vector<std::vector<std::string>>& vec)
{
    for (const auto& v : vec) {  // loop over the outer vector
        for (const auto& s : v) {  // loop over the inner vector
            std::cout << s << " | ";  // print each string separated by a space
			std::cout.flush();
        }
        std::cout << std::endl;  // print a newline after each inner vector
    }
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Required argument: file_name\n./MIPS_interpreter <file name>\n";
		return 0;
	}
	std::ifstream file(argv[1]);
	MIPS_Architecture *mips;
	if (file.is_open())
		mips = new MIPS_Architecture(file);
	else
	{
		std::cerr << "File could not be opened. Terminating...\n";
		return 0;
	}

	// printVectorOfVector(mips->commands);

	mips->executeCommandsPipelined();
	return 0;
}