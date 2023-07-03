#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <set>
#include <random>

using namespace std;

int heuristica_vmc(int depositos, int vendedores, vector<vector<int> >& sol_inicial_h_vmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades){
    auto start = chrono::steady_clock::now();

    // me creo el archivo de salida
    string solucion = "solucion_heuristica_vmc";
    ofstream output_file(solucion);

    // me creo el vector solucion
    vector<vector<int> > vec_sol(depositos, vector<int>(vendedores, -1));

    // me creo mi vector de capacidades restantes
    vector<int> capacidades_restantes = capacidades;

    // me creo una variable para guardar la distancia total recorrida
    int dist_total = 0;
    
    // iterar por cada vendedor
    for(int a = 0; a < vendedores; a++){
        // itero por cada deposito
        int min = 99999999;
        int deposito_mas_cerca;
        for(int b = 0; b < depositos; b++){
            if (distancias[b][a] < min && (capacidades_restantes[b] >= demandas[b][a])){
                min = distancias[b][a];
                deposito_mas_cerca = b;
            }
        }
        // escribo la asignacion en el archivo solucion
        if(min != 99999999){
            capacidades_restantes[deposito_mas_cerca] = capacidades_restantes[deposito_mas_cerca] - demandas[deposito_mas_cerca][a];
            dist_total = dist_total + min;
            vec_sol[deposito_mas_cerca].push_back(a);
        }
        else{
            // busco la distancia mas grande del vendedor a que no pudo ser asignado
            int max = 0;
            for(int c = 0; c < depositos; c++){
                if (distancias[c][a] > max){
                    max = distancias[c][a];
                }
            }
            dist_total = dist_total + (3 * max);
            cout << "Se agrego una penalizacion de" << 3 * max << " por el vendedor " << a << " que no fue asignado." << endl;
        }
    }
    // escribo las asignaciones en el archivo de salida
    for (int i = 0; i < vec_sol.size(); i++) {
        for(int s = 0; s < vec_sol[i].size(); s++){
            if (vec_sol[i][s] != -1){
                sol_inicial_h_vmc[i].push_back(vec_sol[i][s]);
                output_file << vec_sol[i][s] << " ";
            }
        }
        output_file << endl;
    }

    output_file.close();

    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;

    return dist_total;
}