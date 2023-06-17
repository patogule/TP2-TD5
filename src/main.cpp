#include <string>
#include <iostream>
#include <fstream>

int heuristica_vmc(int m){

    // iterar por cada vecino
    //for(int a, a < )
}


int relocate(std::string filename){
    

}


int main(int argc, char** argv) {
    std::string filename = "instances/gap/gap_a/a05100";
    std::cout << "Reading file " << filename << std::endl;

    // Abrir el archivo
    std::ifstream file(filename);
    if (file.is_open()) {
        // Leer la primera línea
        std::string firstLine;
        std::getline(file, firstLine);

        // Imprimir la primera línea
        std::cout << "First line: " << firstLine << std::endl;

        // Cerrar el archivo
        file.close();
    } else {
        std::cout << "Error opening file" << std::endl;
    }

    return 0;
}