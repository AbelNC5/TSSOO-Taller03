#include <global.h>
#include <checkArgs.hpp>

uint64_t* arreglo = nullptr;

int main(int argc, char** argv){

	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t limInferior;
	uint32_t limSuperior;

	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads     = argumentos->getArgs().numThreads;
	limInferior    = argumentos->getArgs().limInferior;
	limSuperior    = argumentos->getArgs().limSuperior;

	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads  : " << numThreads     << std::endl;
	std::cout << "Limite inferior: " << limInferior << std::endl;
	std::cout << "Limite superior: " << limSuperior << std::endl;

	//Este comentario se uso para verificar que los hilos se generaban correctamente.
	/*uint32_t id;
	#pragma omp parallel private(id) num_threads(numThreads)
	{
	id = omp_get_thread_num();
	std::cout << "id: " << id << std::endl;
	}*/

	//En esta sección se establece la función randómica thread-safe que se utilizará 
	//para generar lo números aleatorios
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<> unif(limInferior,limSuperior);

	//Etapa de llenado

	//-------Secuencial-------
	arreglo = new uint64_t[totalElementos];

	auto start = std::chrono::high_resolution_clock::now();

	for(size_t i = 0; i < totalElementos; ++i){
		arreglo[i] = unif(rng);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto tiempoLlenadoTotal_S = elapsed.count();

	delete[] arreglo;
	//-------Paralelo-------
	arreglo = new uint64_t[totalElementos];

	start = std::chrono::high_resolution_clock::now();

	//Serán utilizados los valores de este arreglo para la etapa de suma,
	//ya que el arreglo anterior fue borrado, para hacer el llenado paralelo.

	#pragma omp parallel for num_threads(numThreads)
	for(size_t i=0; i < totalElementos; ++i){
		arreglo[i] = unif(rng);
	}

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
	auto tiempoLlenadoTotal_P = elapsed.count();

	//Etapa de suma

	//Secuencial
	uint64_t sumaSecuencial=0;

	start = std::chrono::high_resolution_clock::now();

	for(size_t i = 0; i < totalElementos; ++i){
		sumaSecuencial += arreglo[i];
	}

	end = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto tiempoTotalSuma_S = elapsed.count();

	//Paralela
	uint64_t sumaParalela=0;
	start = std::chrono::high_resolution_clock::now();

	//De forma similar al llenado

	#pragma omp parallel for reduction(+:sumaParalela) num_threads(numThreads)
	for(size_t i=0; i<totalElementos; ++i){
		sumaParalela += arreglo[i];
	}

	end     = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto tiempoTotalSuma_P = elapsed.count();

	//Etapa de Resultados

	std::cout << "Suma secuencial total: " << sumaSecuencial << std::endl;
	std::cout << "Suma en paralelo total: " << sumaParalela << std::endl;
	std::cout << "------- Tiempos de ejecución --------" << std::endl;
	std::cout << "Tiempo Llenado Secuencial: " << tiempoLlenadoTotal_S << "[ms]" << std::endl;
	std::cout << "Tiempo Llenado Paralelo: " << tiempoLlenadoTotal_P << "[ms]" << std::endl;
	std::cout << "SpeedUp Etapa de Llenado: " << (double)tiempoLlenadoTotal_S/tiempoLlenadoTotal_P << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "Tiempo Suma Secuencial: " << tiempoTotalSuma_S << "[ms]" << std::endl;
	std::cout << "Tiempo Suma Paralela: " << tiempoTotalSuma_P << "[ms]" << std::endl;
	std::cout << "SpeedUp Etapa de Suma: " << (double)tiempoTotalSuma_S/tiempoTotalSuma_P << std::endl;
	std::cout << "-------------------------------------" << std::endl;


	return(EXIT_SUCCESS);
}

