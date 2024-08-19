#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring> 
using namespace std;

// Registro para armazenar valores do arquivo
struct roupa {
    int codigo;
    char modelo[15];
    int tamanho;
    char cor[15];
    char descricao[50];
};
//função para a interface inicial, escolhe se vai ler de um arquivo csv ou binario
int interface(int &opcaoi){
    opcaoi = -1;
	while(opcaoi < 0 or opcaoi > 2){
		cout << "Escolha uma opcao:" << endl
			 << "[1] Ler de um arquivo binario" << endl
			 << "[2] Ler de um arquivo csv" << endl
			 << "Escolha:";
		cin >> opcaoi;
		if(opcaoi < 0 or opcaoi > 2){
			cout << "Opcao nao encontrada" << endl;
		}
	}
	return opcaoi;
}
// função para o menu principal, tem opções que o usuario pode escolher
int menu(int &opcaom){
    opcaom = -1;
	while(opcaom < 0 or opcaom > 15){
		cout << "Escolha uma opcao:" << endl
			 << "[1] Inserir novo elemento" << endl
			 << "[2] Remover elemento" << endl
			 << "[3] Exibir todo arquivo" << endl
			 << "[4] Exibir um trecho do arquivo" << endl
			 << "[5] Exportar para um arquivo CSV" << endl
			 << "[6] Exportar para um arquivo binario" <<endl
			 << "[7] Buscar pelo codigo da roupa" << endl
			 << "[8] Buscar pelo modelo da roupa" << endl
			 << "[9] Ordenar pelo codigo" << endl
			 << "[10] Ordenar pelo modelo" << endl
			 << "[11] Ordenar pelo tamanho" << endl
			 << "[12] Ordenar pela cor" << endl
			 << "[13] Ordenar pela descricao" << endl
			 << "[14] Salvar alteracoes" << endl
			 << "[15] Finalizar" << endl
			 << "Escolha:";
		cin >> opcaom;
		if(opcaom < 0 or opcaom > 15){
			cout << "Opcao nao encontrada" << endl;
		}
	}
	return opcaom;
}

// Função para redimensionar o vetor dinamicamente
void redimensionar(roupa*& registros, int& capacidade) {
    int novaCapacidade = capacidade + 10;
    roupa* temp = new roupa[novaCapacidade];
    for (int i = 0; i < capacidade; i++) {
        temp[i] = registros[i];
    }
    delete[] registros;
    registros = temp;
    capacidade = novaCapacidade;
}


// Função para ler registros do arquivo binário e colocar no vetor
void lerBin(const char* nomearquivobin, roupa*& registros, int& numRegistros, int& capacidade) {
    ifstream arquivoler(nomearquivobin, ios_base::in | ios_base::binary);
    if (arquivoler.is_open()) {
        // Contar o número de registros no arquivo
        arquivoler.seekg(0, ios::end);
        int totalRegistros = arquivoler.tellg() / sizeof(roupa);
        arquivoler.seekg(0, ios::beg);

        // Ler os registros do arquivo
        for (int i = 0; i < totalRegistros; i++) {
            if (numRegistros >= capacidade) {
                redimensionar(registros, capacidade);
            }
            arquivoler.read(reinterpret_cast<char*>(&registros[numRegistros]), sizeof(roupa));
            numRegistros++;
        }
        arquivoler.close();
    } else {
        cout << "Arquivo não pode ser aberto para leitura!" << endl;
    }
}

// lê registros do arquivo CSV
void lerCSV(const string& nomeArquivo, roupa*& registros, int& numRegistros, int& capacidade) {
    ifstream arquivo(nomeArquivo);
    string linha, leitura;
    int contador = 0;
    int tamanho_csv = 0;

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }

    // Conta o número de linhas no arquivo
    while (getline(arquivo, leitura)) {
        tamanho_csv++;
    }
    arquivo.clear();
    arquivo.seekg(0, ios::beg);

    registros = new roupa[tamanho_csv];

    while (contador < tamanho_csv) {
        getline(arquivo, linha);
        stringstream ss(linha);

        ss >> registros[contador].codigo;
        ss.ignore(); // Ignora a vírgula

        string temp;
        getline(ss, temp, ',');
        strncpy(registros[contador].modelo, temp.c_str(), 14);
        registros[contador].modelo[14] = '\0'; // Garante terminação nula

        ss >> registros[contador].tamanho;
        ss.ignore(); // Ignora a vírgula

        getline(ss, temp, ',');
        strncpy(registros[contador].cor, temp.c_str(), 14);
        registros[contador].cor[14] = '\0'; // Garante terminação nula

        getline(ss, temp);
        strncpy(registros[contador].descricao, temp.c_str(), 49);
        registros[contador].descricao[49] = '\0'; // Garante terminação nula

        // Verifica se a descrição não está vazia antes de remover aspas
        size_t len = strlen(registros[contador].descricao);
        if (len > 0) {
            for (size_t i = 0; i < len - 1; i++) {
                registros[contador].descricao[i] = registros[contador].descricao[i + 1];
            }
            registros[contador].descricao[len - 2] = '\0'; // Remove a última aspa
        }

        contador++;
    }
    numRegistros = contador;
    arquivo.close();
}
//função para inserir um novo item, mas não salva
void inserir(int &numRegistros, roupa *registros){
		cout << "\nCodigo do produto: ";
		cin >> registros[numRegistros].codigo;
		cin.ignore(); // Ignorar o caractere de nova linha deixado pelo cin
		cout << "Modelo do produto: ";
		cin.getline(registros[numRegistros].modelo, 15);
		cout << "Tamanho do produto: ";
		cin >> registros[numRegistros].tamanho;
		cin.ignore(); // Ignorar o caractere de nova linha deixado pelo cin
		cout << "Cor do produto: ";
		cin.getline(registros[numRegistros].cor, 15);
		cout << "Descricao do produto: ";
		cin.getline(registros[numRegistros].descricao, 50);
		cout << endl;
		numRegistros++;
}
//função para ler um trecho do arquivo
void trecho(roupa *registros, int numRegistros, int inicio, int fim){
	for(int i=inicio; i<fim; i++){
		cout << registros[i].codigo << " "
                << registros[i].modelo << " "
                << registros[i].tamanho << " "
                << registros[i].cor << " "
                << registros[i].descricao << endl;
	}
}
//função para ordenar o arquivo de acordo com o codigo
void shell_sort_codigo(roupa *registros, int numRegistros) {
    int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    int pos_gap = 8;
    while (gaps[pos_gap] > numRegistros) {
        pos_gap--;
    }    
    roupa aux;
    int j;
    while (pos_gap >= 0) {
        int gap = gaps[pos_gap];

        for (int i = gap; i < numRegistros; i++) {
            aux = registros[i];
            j = i;
            while ((j >= gap) and (aux.codigo < registros[j - gap].codigo)) {
                registros[j] = registros[j - gap];
                j = j - gap;
            }
            registros[j] = aux;
        }

        pos_gap--;
    }
}
//função para ordenar o arquivo de acordo com o tamanho
void shell_sort_tamanho(roupa *registros, int numRegistros) {
    int gaps[9] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    int pos_gap = 8;
    while (gaps[pos_gap] > numRegistros) {
        pos_gap--;
    }    
    roupa aux;
    int j;
    while (pos_gap >= 0) {
        int gap = gaps[pos_gap];

        for (int i = gap; i < numRegistros; i++) {
            aux = registros[i];
            j = i;
            while ((j >= gap) and (aux.tamanho < registros[j - gap].tamanho)) {
                registros[j] = registros[j - gap];
                j = j - gap;
            }
            registros[j] = aux;
        }

        pos_gap--;
    }
}
// função para remover algum elemento, fornecendo a sua posição(pode ser vista em busca por código)
void remover(roupa *&registros, int elemento, int &numRegistros){
    roupa *vetaux = new roupa[numRegistros-1];
    for(int i = 0; i < elemento; i++){
        vetaux[i] = registros[i];
    }
    for(int i = elemento+1; i < numRegistros; i++){
        vetaux[i-1] = registros[i];
    }
    delete[] registros;
    registros = vetaux;
    numRegistros--;
}
// função para buscar um elemento pelo seu codigo, usa busca binaria
int buscaBinCodigo(roupa *registros,int elementoProcura, int inicio, int fim){
	int meio = (inicio + fim)/2;
	if(registros[meio].codigo == elementoProcura){
		return meio;
	}
	else if(inicio < fim){
		if(elementoProcura < registros[meio].codigo){
			return buscaBinCodigo(registros, elementoProcura, inicio, fim = meio-1);
			}
		else{
			return buscaBinCodigo(registros, elementoProcura, inicio = meio+1 , fim);
		}
	}
	else{
		return -1;
	}
}
//função para ordenar o arquivo de acordo com o modelo
void ordenaModelo(roupa *registros, int numRegistros) {
    roupa aux;
    for (int x = 0; x < numRegistros; x++) {
        for (int y = x + 1; y < numRegistros; y++) {
            if (strcmp(registros[x].modelo, registros[y].modelo) > 0) {
                aux = registros[x];
                registros[x] = registros[y];
                registros[y] = aux;
            }
        }
    }
}
//função para ordenar o arquivo de acordo com a cor
void ordenaCor(roupa *registros, int numRegistros) {
    roupa aux;
    for (int x = 0; x < numRegistros; x++) {
        for (int y = x + 1; y < numRegistros; y++) {
            if (strcmp(registros[x].cor, registros[y].cor) > 0) {
                aux = registros[x];
                registros[x] = registros[y];
                registros[y] = aux;
            }
        }
    }
}
//função para ordenar o arquivo de acordo com a descricao
void ordenaDescricao(roupa *registros, int numRegistros) {
    roupa aux;
    for (int x = 0; x < numRegistros; x++) {
        for (int y = x + 1; y < numRegistros; y++) {
            if (strcmp(registros[x].descricao, registros[y].descricao) > 0) {
                aux = registros[x];
                registros[x] = registros[y];
                registros[y] = aux;
            }
        }
    }
}
//função para copiar um regitro, fazendo um buckup dele
void copiarRegistros(roupa* registros, roupa*& registrosbuckup, int numRegistros) {
    // Libera a memória do vetor de backup se já estiver alocada
    if (registrosbuckup != nullptr) {
        delete[] registrosbuckup;
    }

    // Aloca memória para o vetor de backup
    registrosbuckup = new roupa[numRegistros];

    // Copia os dados de cada registro
    for (int i = 0; i < numRegistros; i++) {
        registrosbuckup[i] = registros[i];
    }
}
//função para exportar os valores do vetor para um arquivo binario
void exportarBin(const char* nomeArquivoBin, roupa* registrosbuckup, int numRegistros) {
    ofstream arquivo(nomeArquivoBin, ios_base::out | ios_base::binary);
    if (!arquivo.is_open()) {
        cout << "Arquivo não pode ser aberto!" << endl;
        return;
    }
    for (int i = 0; i < numRegistros; i++) {
        arquivo.write(reinterpret_cast<char*>(&registrosbuckup[i]), sizeof(roupa));
    }
    arquivo.close();
    cout << "Arquivo binário exportado com sucesso!" << endl;
}
//função para exportar os valores do vetor para um arquivo CSV
void exportarCSV(const char* nomearquivocsv, roupa* registrosbuckup, int &numRegistros) {
    ofstream arquivo(nomearquivocsv);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV para escrita." << endl;
        return;
    }

    for (int i = 0; i < numRegistros; i++) {
        arquivo << registrosbuckup[i].codigo << ","
                << registrosbuckup[i].modelo << ","
                << registrosbuckup[i].tamanho << ","
                << registrosbuckup[i].cor << ","
                << registrosbuckup[i].descricao << endl;
    }

    arquivo.close();
}
// função para buscar um arquivo de acordo com o seu modelo, usa busca binaria
int buscaBinModelo(roupa *registros, int inicio, int fim, char procura[15]) {
    if (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int resultado = strcmp(registros[meio].modelo, procura);

        // Verifica se a chave está presente no meio
        if (resultado == 0) {
            return meio;
        }

        // Se a chave for maior, ignore a metade esquerda
        if (resultado < 0) {
            return buscaBinModelo(registros, meio + 1, fim, procura);
        }
        // Se a chave for menor, ignore a metade direita
        else {
            return buscaBinModelo(registros, inicio, meio - 1, procura);
        }
	}
    // Retorna -1 se a chave não for encontrada
    return -1;
}


int main() {
    const char* nomearquivobin = "roupas.bin";// define o nome do arquivo binario para exportar e importar
    const char* nomearquivocsv = "roupa5.csv";// define o nome do arquivo CSV para exportar e importar
    int capacidade = 40;// valor inicial do vetor
    roupa* registros = new roupa[capacidade];
    roupa* registrosbuckup = new roupa[capacidade];
    int numRegistros = 0;// variavel que pega o valor do numero de registros feitos
    
    int opcaoi=-1, opcaom=-1;// variavel que pega o valor escolhido pelo usuario da interface e do menu
    
    interface(opcaoi);
    if(opcaoi == 1){
        lerBin(nomearquivobin, registros, numRegistros, capacidade);
    }
    if(opcaoi == 2){
        lerCSV(nomearquivocsv, registros, numRegistros, capacidade);
    }
    copiarRegistros(registros, registrosbuckup, numRegistros);
    while(opcaom != 15){
        menu(opcaom);
        if(opcaom == 1){
            inserir(numRegistros, registros);
        }
        if(opcaom == 2){
            int elemento = -1;
            while(elemento < 1 or elemento > numRegistros){ 
                cout << "Digite a posicao do elemento:";
                cin >> elemento;
            }
            remover(registros, elemento-1, numRegistros);
            cout << "Elemento " << elemento << " excluido" << endl;
        }
        if(opcaom == 3){
            int inicio = 0, fim = numRegistros;
            trecho(registros, numRegistros, inicio, fim);
        }
        if(opcaom == 4){
            int inicio = 0, fim = numRegistros;
            do{
                cout << "Defina a posicao inicial do trecho:";
                cin >> inicio;
            } while(inicio < 0);
            do{
                cout << "Defina a posicao final do trecho:";
                cin >> fim;
            } while(fim > numRegistros);
            trecho(registros, numRegistros, inicio, fim);
        }
        if(opcaom == 5){
			int opcc=0;
			while(opcc < 1 or opcc >2){
				cout << "[1] salvar e exportar" << endl
					 << "[2] exportar buckup" << endl
					 << "Escolha: ";
				cin >> opcc;
			}
			if (opcc == 1){
				copiarRegistros(registros, registrosbuckup, numRegistros);
			}
			exportarCSV(nomearquivocsv, registrosbuckup, numRegistros);
		}
		if(opcaom == 6){
			int opcb=0;
			while(opcb < 1 or opcb >2){
				cout << "[1] salvar e exportar" << endl
					 << "[2] exportar buckup" << endl
					 << "Escolha: ";
				cin >> opcb;
			}
			if (opcb == 1){
				copiarRegistros(registros, registrosbuckup, numRegistros);
			}
			exportarBin(nomearquivobin, registrosbuckup, numRegistros);
		}
		if(opcaom == 7){
			int elementoProcuraC, posicaoC;
			cout << "Ordenando por codigo..." << endl;
			shell_sort_codigo(registros,numRegistros);
			cout << "Ordenado!" << endl;
			cout << "Qual o codigo da roupa?" << endl;
			cin >> elementoProcuraC;
			int inicioC = 0, fimC = numRegistros;
			posicaoC = buscaBinCodigo(registros, elementoProcuraC, inicioC, fimC);
			if (posicaoC < 0){
				cout << "O Codigo nao foi achado" << endl; 
			}
			else{
				cout << "O elemento esta na posicao " << posicaoC+1 << endl;
				cout << "Codigo: ";
				cout << registros[posicaoC].codigo << endl;
				cout << "Modelo: ";
				cout << registros[posicaoC].modelo << endl;
				cout << "Tamanho: ";
				cout << registros[posicaoC].tamanho << endl;
				cout << "Cor: ";
				cout << registros[posicaoC].cor << endl;
				cout << "Descricao: ";
				cout << registros[posicaoC].descricao << endl;
				cout << endl;
					}
		}
		if(opcaom == 8){
			int posicaoM;
			char procura[15];
			cout << "Ordenando pelo Modelo..." << endl;
			ordenaModelo(registros, numRegistros);
			cout << "Ordenado" << endl;
			cout << "Qual o modelo da roupa?";
			cin >> procura;
			int inicioM = 0, fimM = numRegistros;
			posicaoM = buscaBinModelo(registros, inicioM, fimM, procura);
			if (posicaoM < 0){
				cout << "O Modelo nao foi achado" << endl; 
			}
			else{
				cout << "Codigo: ";
				cout << registros[posicaoM].codigo << endl;
				cout << "Modelo: ";
				cout << registros[posicaoM].modelo << endl;
				cout << "Tamanho: ";
				cout << registros[posicaoM].tamanho << endl;
				cout << "Cor: ";
				cout << registros[posicaoM].cor << endl;
				cout << "Descricao: ";
				cout << registros[posicaoM].descricao << endl;
				cout << endl;
					}
		}
		if(opcaom == 9){
			shell_sort_codigo(registros, numRegistros);
			cout <<"\nOrdenado"<< endl;
		}
		if(opcaom == 10){
			ordenaModelo(registros, numRegistros);
			cout <<"\nOrdenado"<< endl;
		}
		if(opcaom == 11){
			shell_sort_tamanho(registros, numRegistros);
			cout <<"\nOrdenado"<< endl;
		}
		if(opcaom == 12){
			ordenaCor(registrosbuckup, numRegistros);
			cout <<"\nOrdenado"<< endl;
		}
		if(opcaom == 13){
			ordenaDescricao(registrosbuckup, numRegistros);
			cout <<"\nOrdenado"  << endl;
		}
		if(opcaom == 14){
			copiarRegistros(registros, registrosbuckup, numRegistros);
			cout << "Dados salvos" << endl;
		}
	}
    // Liberar a memória alocada
    delete [] registros;
    delete [] registrosbuckup;

    return 0;
}



