#include <global.h>
#include <checkArgs.hpp>


int main(int argc, char** argv){
	
	uint64_t totalElementos;
	uint32_t numThreads;

	
	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	
	totalElementos = argumentos->getArgs().tamProblema;
	numThreads     = argumentos->getArgs().numThreads;
	
	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads  : " << numThreads     << std::endl;
	
	
	return(EXIT_SUCCESS);
}

