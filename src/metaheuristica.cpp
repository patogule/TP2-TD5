#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <set>
#include <random>

using namespace std;

// METAHEURISTICA

vector<vector<int> > greedy_randomized(int& dist_total_greedy, int depositos, int vendedores, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades, int rcl_size){

    // me creo el vector solucion
    vector<vector<int> > vec_sol(depositos);

    // me creo mi vector de capacidades restantes
    vector<int> capacidades_restantes = capacidades;

    // me creo una variable para guardar la distancia total recorrida
    int dist_total = 0;
    
    // iterar por cada vendedor
    for(int a = 0; a < vendedores; a++){
        // itero por cada deposito
        int min = 99999999;
        // buscamos la cantidad de depositos mas cercanos que nos pide rcl_size
        vector<int> depositos_mas_cercanos;
        set<int> depositos_mas_cercanos_set;
        vector<int> vector_min;
        for (int q = 0; q < rcl_size; q++){
            int min = 99999999;
            int indice;
            for(int b = 0; b < depositos; b++){
                if (distancias[b][a] < min && (capacidades_restantes[b] >= demandas[b][a]) && (depositos_mas_cercanos_set.count(b) == 0)){
                    min = distancias[b][a];
                    indice = b;
                }
            }
            vector_min.push_back(distancias[indice][a]);
            depositos_mas_cercanos.push_back(indice);
            depositos_mas_cercanos_set.insert(indice);
        }  

        // elegir un deposito random dentro de los rcl_zise depositos mas cercanos
        random_device rd;
        mt19937 generador(rd());
        uniform_int_distribution<> distribucion(0, depositos_mas_cercanos.size() - 1);
        int indiceAleatorio = distribucion(generador);
        int deposito_elegido = depositos_mas_cercanos[indiceAleatorio];
        min = vector_min[indiceAleatorio];

        // escribo la asignacion en el archivo solucion
        if(depositos_mas_cercanos.size() != 0){
            capacidades_restantes[deposito_elegido] = capacidades_restantes[deposito_elegido] - demandas[deposito_elegido][a];
            dist_total = dist_total + min;
            vec_sol[deposito_elegido].push_back(a);
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
    dist_total_greedy = dist_total;
    return vec_sol;
}

int grasp(double& tiempo, int depositos, int vendedores, vector<vector<int> >& sol_grasp, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades, int n_iters, int rcl_size){
    auto start = chrono::steady_clock::now();

    // inicializo el vector solucion y una distancia recorrida best en infinito
    vector<vector<int> > vector_best(depositos);
    int best = 999999999;

    // hago n_iters iteraciones
    for(int r = 0; r < n_iters; r++){
        // calculo un greedy randomizado
        int dist_total_greedy;
        vector<vector<int> > vector_current = greedy_randomized(dist_total_greedy, depositos, vendedores, distancias, demandas, capacidades, rcl_size);
        // aplico la busqueda local swap a nuestro greedy randomizado
        vector<vector<int> > sol_swap(depositos);
        double tiempo;
        int dist_total = swap(tiempo, dist_total_greedy, sol_swap, vector_current, depositos, capacidades, demandas, distancias);

        // si esta nueva distancia total es menor a la best global esta va a ser mi nueva distancia best global
        //cout << "dist total best: " << best << endl << "dist de esta ronda: "  << dist_total << endl << endl;
        if (dist_total < best){
            best = dist_total;
            vector_best = sol_swap;
        }
    }
    sol_grasp = vector_best;

    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;
    tiempo = tiempo_transcurrido.count();

    return best;