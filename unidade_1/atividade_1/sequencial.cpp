#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

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

void salvarResultado(const std::vector<std::vector<int>>& matriz, long long tempo_us, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao criar o arquivo de resultado." << std::endl;
        return;
    }

    int rows = matriz.size();
    int cols = matriz[0].size();

    arquivo << rows << " " << cols << std::endl;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arquivo << "c" << i+1 << j+1 << " " << matriz[i][j] << std::endl;
        }
    }
    
    arquivo << tempo_us << std::endl;

    arquivo.close();
    std::cout << "Resultado salvo com sucesso em " << nomeArquivo << std::endl;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_matriz1> <arquivo_matriz2>" << std::endl;
        return 1;
    }

    std::string arquivo1 = argv[1];
    std::string arquivo2 = argv[2];

    std::cout << "Lendo matrizes..." << std::endl;
    std::vector<std::vector<int>> m1 = lerMatriz(arquivo1);
    std::vector<std::vector<int>> m2 = lerMatriz(arquivo2);

    if (m1.empty() || m2.empty()) {
        std::cerr << "Falha ao ler uma ou mais matrizes." << std::endl;
        return 1;
    }

    if (m1[0].size() != m2.size()) {
        std::cerr << "Erro: As matrizes nao podem ser multiplicadas (colunas de M1 != linhas de M2)." << std::endl;
        return 1;
    }

    int n1 = m1.size();
    int m1_cols = m1[0].size();
    int m2_cols = m2[0].size();
    
    std::vector<std::vector<int>> resultado(n1, std::vector<int>(m2_cols, 0));

    std::cout << "Iniciando multiplicacao sequencial..." << std::endl;
    
    auto inicio = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2_cols; ++j) {
            for (int k = 0; k < m1_cols; ++k) {
                resultado[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    auto fim = std::chrono::high_resolution_clock::now();

    long long duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio).count();
    
    std::cout << "Multiplicacao concluida em " << duracao << " microssegundos." << std::endl;

    salvarResultado(resultado, duracao, "resultado_sequencial.txt");

    return 0;
}