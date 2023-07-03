#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <set>
#include <random>


using namespace std;

int distancia_total(vector<vector<int> > vec, vector<vector<int> > distancias){
    int resultado = 0;
    for (int l = 0; l < vec.size(); l++){
        for (int m = 0; m < vec[l].size(); m++){
            resultado = resultado + distancias[l][vec[l][m]];
        }
    }
    return resultado;
} 

int relocate(vector<vector<int> > solucion_inicial, int depositos, vector<int> capacidades, vector<vector<int> > demandas, vector<vector<int> > distancias) {
    auto start = chrono::steady_clock::now();

    // calculamos la capacidad disponible de cada deposito en la solucion original
    vector<int> capacidad_disponible(depositos);
    for (size_t i = 0; i < solucion_inicial.size(); ++i) {
        capacidad_disponible[i] = capacidades[i];
        for (int pp = 0; pp < solucion_inicial[i].size(); pp++) {
            capacidad_disponible[i] = capacidad_disponible[i] - demandas[i][solucion_inicial[i][pp]];
        }
    }

    // distancia total original
    int dist_total = distancia_total(solucion_inicial, distancias);
    int dist_parcial_aux = dist_total;

    vector<vector<int> > solucion_parcial = solucion_inicial;

    // iteramos por los depositos
    for (int z = 0; z < solucion_inicial.size(); z++){
        // iteramos por los vendedores
        for(int k = 0; k < solucion_inicial[z].size(); k++){
            // cada vendedor lo cambiamos de dposito
            for(int q = 0; q < solucion_inicial.size(); q++){
                // calcular la distancia total de la nueva solucion
                if (q != z){
                    if( capacidad_disponible[q] >= demandas[q][solucion_inicial[z][k]]){
                        int dist_parcial = dist_total - distancias[z][solucion_inicial[z][k]] + distancias[q][solucion_inicial[z][k]];
                        if (dist_parcial < dist_parcial_aux){
                            dist_parcial_aux = dist_parcial;
                            solucion_parcial = solucion_inicial;
                            // escribimos la nueva solucion parcial en nuestro vector de solucion parcial
                            solucion_parcial[q].push_back(solucion_parcial[z][k]);
                            solucion_parcial[z][k] = -1;
                        }
                    }
                }
            }
        }
    }

    // escribir en el txt
    // nos creamos el archivo de salida
    string solucion_rel = "solucion_busqueda_local_relocate";
    ofstream output_file_rel(solucion_rel);

    if (output_file_rel.is_open()) {
        for (int u = 0; u < solucion_parcial.size(); u++){
            for (int y = 0; y < solucion_parcial[u].size(); y++){
                if (solucion_parcial[u][y] != -1){
                    output_file_rel << solucion_parcial[u][y] << " ";
                }
            }
            output_file_rel << "\n"; // Nueva línea después de cada vector interno
        }
        output_file_rel.close();
    } else {
        std::cout << "No se pudo abrir el archivo." << std::endl;
    }
    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;

    return dist_parcial_aux;
    
}