#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>

std::vector<std::vector<int>> lerMatriz(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return {};
    }
    int rows, cols;
    arquivo >> rows >> cols;
    std::vector<std::vector<int>> matriz(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arquivo >> matriz[i][j];
        }
    }
    arquivo.close();
    return matriz;
}

void calcular_e_salvar_parte(int id, const std::vector<std::vector<int>>& m1, const std::vector<std::vector<int>>& m2, int P, int inicio_idx) {
    auto inicio_proc = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<std::string, int>> resultado_parcial;
    int m1_cols = m1[0].size();
    int res_rows = m1.size();
    int res_cols = m2[0].size();

    for (int k = 0; k < P; ++k) {
        int idx_global = inicio_idx + k;
        if (idx_global >= res_rows * res_cols) {
            break;
        }
        int i = idx_global / res_cols;
        int j = idx_global % res_cols;
        int valor = 0;
        for (int m = 0; m < m1_cols; ++m) {
            valor += m1[i][m] * m2[m][j];
        }
        std::string coordenada = "c" + std::to_string(i + 1) + std::to_string(j + 1);
        resultado_parcial.push_back({coordenada, valor});
    }

    auto fim_proc = std::chrono::high_resolution_clock::now();
    long long duracao_proc = std::chrono::duration_cast<std::chrono::microseconds>(fim_proc - inicio_proc).count();

    std::string nomeArquivo = "resultado_processo_" + std::to_string(id) + ".txt";
    std::ofstream arquivo(nomeArquivo);
    arquivo << res_rows << " " << res_cols << std::endl;
    for (const auto& par : resultado_parcial) {
        arquivo << par.first << " " << par.second << std::endl;
    }
    arquivo << duracao_proc << std::endl;
    arquivo.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_matriz1> <arquivo_matriz2> <P>" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> m1 = lerMatriz(argv[1]);
    std::vector<std::vector<int>> m2 = lerMatriz(argv[2]);
    int P = std::atoi(argv[3]);

    if (m1.empty() || m2.empty() || P <= 0) {
        return 1;
    }

    int n1 = m1.size();
    int m2_cols = m2[0].size();
    int total_elementos = n1 * m2_cols;
    int num_processos = std::ceil(static_cast<double>(total_elementos) / P);
    
    std::cout << "Criando " << num_processos << " processos-filhos..." << std::endl;

    for (int i = 0; i < num_processos; ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            int inicio_idx = i * P;
            calcular_e_salvar_parte(i, m1, m2, P, inicio_idx);
            exit(0);
        } else if (pid < 0) {
            std::cerr << "Erro ao criar processo-filho (fork)." << std::endl;
            return 1;
        }
    }

    for (int i = 0; i < num_processos; ++i) {
        wait(NULL);
    }

    std::cout << "Todos os processos-filhos terminaram." << std::endl;

    return 0;
}