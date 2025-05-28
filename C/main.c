#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

// Compilar com: gcc -o sce main.c

#define LIMITE_ENTRADA 128
#define MENSAGEM_DE_ERRO "Erro: Leitura de entrada não ocorreu...\n"
// ATENÇÃO: Ler o texto abaixo antes de contribuir no código

// Ordem de precedência de declaração:
// #include (libs) -> #define (macros) -> Variáveis Globais/Vetores/Constantes -> Structs/Funções/Variáveis Locais (por ordem de uso)

// Tabulação é equivalente a 4 espaços
// Seguir a convenção camelCase, respectivamente, para nome de variáveis, constantes, funções, entre outros
// Adicionar linha extra antes ou depois de estruturas como if, for, declaração de função
// Declaração de if, for, switch... deve conter um espaço entre comando e () enquanto na de funções não
// Exemplo:

// if ()
// int funcao()
// funcao()

// Não escrever mais de 127 caracteres por linha, se possível

// Deveria ser obtido de uma Query, como SELECT COUNT(*) FROM USUARIO;
int idAtual = 3;

// Em teoria o resultado de uma Query em ENTRADA não deve passar de 120 caracteres, sendo 4+80+10+10+10+5+1, com uma data sendo no padrão (YYYY-mm-dd), então sendo 10 caracteres
// As strings dessa parte, os vetores deveriam ser de tamanho dinâmico
// O consumo de memória dessa struct é maior do que o poderia ser, mas prefiro evitar vetores de tamanho dinâmico que requereriam manipulações mais avançadas, uso de malloc
typedef struct {
	int id;
	char nome[81];
	int quantidade;
	char validade[14];
	char nivelPrioridade[2];
} produto;

void limpaTela() {
	system("clear");

	printf("\033[106mv0.1 SCE por AlphaTech\033[K\n");
	printf("\033[0m\n");
}

void primeiraTela() {
	limpaTela();
	printf("Bem-vindo ao SCE!\n\n");
	printf("O que deseja fazer?\n[0] Entrar\n[1] Criar uma conta\n[c] Sair\n");
}

void criacaoTela() {
	limpaTela();
	printf("Criação de Usuário\n\n");
	printf("AVISOS: Nome deve conter pelo menos 3 letras. Não usar acentos ou cedilha.\n\n");
}

char *formataValidade(char *propriedadeValidade) {
	return strcmp(propriedadeValidade, "null") == 0 ? "Indeterminada" : propriedadeValidade;
}

void estoqueTela(produto *lista, int controle, int *controleTamanho, int numeroPagina, int *paginaPonteiro) {
	limpaTela();

	// Aqui deveria ser armazenado o resultado de um Query, SELECT (ID, NOME, QNTD, VALIDADE, NV_PRI) FROM ENTRADA WHERE ID > offset AND ID < limite;
	// E também uma checagem do total ou se uma hora é retornado menos do que as 7 linhas
	// int total = 3;
	int offset = 0;
	if (numeroPagina > 1) {
		offset = 7 * numeroPagina;

		if (offset > controle) {
			*paginaPonteiro -= 1;
			offset -= 7;
		}
	}

	for (int linha = offset; linha < (7 + offset); linha++) {
		if (lista[linha].quantidade == 0) break;

		printf("[%d] %s,\nQuantidade: %d,\nValidade: %s,\nPrioridade: %s\n",
		 linha + 1, lista[linha].nome, lista[linha].quantidade, formataValidade(lista[linha].validade), lista[linha].nivelPrioridade);
	}
}

void trocaValidade(produto *lista, int id, char *validadeTexto) {
	strcpy(lista[id].validade, validadeTexto);
}

void removeEntrada(produto *lista, int id, int controle, int *controlePonteiro) {
	int pulo = 0;

	for (int i = 0; i < controle; ++i) {
        if (lista[i].id == id) { pulo+=1; }

		lista[i] = lista[i+pulo];
    }

	*controlePonteiro -= 1;
}

void mudaQuantidade(produto *lista, int id, int controle, int *controlePonteiro, unsigned int quantidadeNova) {
	if (quantidadeNova > 99999) {
		printf("Erro: Esse número é acima do limite de quantidade...\n");
		sleep(10);
	}
	else if (quantidadeNova == 0) {
		removeEntrada(lista, lista[id].id, controle, controlePonteiro);
	}

	lista[id].quantidade = quantidadeNova;
}

void geradorSenha(char *buffer) {
	char senha[17];
    char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";

	srand(time(NULL));

    for (int i = 0; i < 16; i++) {
        senha[i] = caracteres[rand() % 70];
    }

    senha[16] = '\0';

    printf("\033[36mSenha Nova:\033[m %s\n", senha);

    strcpy(buffer, senha);
}

// ATENÇÃO: Os códigos comentados abaixo implementam um cache por arquivo local,
// não foram homologados a tempo, use por sua conta e risco
// void salvarUsuarioLogado(const char *nome) {
//     FILE *arquivo = fopen("usuario_logado.txt", "w");
//     if (arquivo != NULL) {
//         fprintf(arquivo, "%s\n", nome);
//         fclose(arquivo);
//     }
// }


int verificarCredenciais(char *nome, char *senha, char *tempNome, char *tempSenha) {
    // FILE *arquivo = fopen("usuarios.txt", "r");
    // if (arquivo == NULL) {
    //     return 0;
    // }

    // char linha[128];
    // char nomeArq[50], senhaArq[50];

    // while (fgets(linha, sizeof(linha), arquivo)) {
    //     sscanf(linha, "%49[^;];%49[^\n]", nomeArq, senhaArq);

    //     if (strcmp(nome, nomeArq) == 0 && strcmp(senha, senhaArq) == 0) {
    //         fclose(arquivo);
    //         return 1;
    //     }
    // }

    // fclose(arquivo);

	// O que esta parte deveria fazer é uma Query como 'SELECT NOME, SENHA FROM USUARIO WHERE NOME = nome AND SENHA = hash(senha);'
	if (strcmp(nome, "joão") == 0 || strcmp(nome, tempNome) == 0) {
		if (strcmp(senha, "*28t5LkSJd1acW9l") == 0 || strcmp(senha, tempSenha) == 0) {
			return 0;
		}
	}

	return 1;
}

void removeQuebraLinha(char *buffer) {
	strtok(buffer, "\n");
}

int validaLetraNome(int l) {
	return isalnum(l) ? 0 : 1;
}

int validaNumero(int l) {
	return isdigit(l) ? 0 : 1;
}

int validaEmail(int l) {
	return (isalnum(l) || l == '@' || ispunct(l)) ? 0 : 1;
}

int validaSenha(int l) {
	return (isalnum(l) || strchr("!@#$%^&*", l) != NULL) ? 0 : 1;
}

int validaId(int l) {
	return (isdigit(l) && l != '0' && l != '9') ? 0 : 1;
}

int validaData(int l) {
	return (isdigit(l) || l == '-') ? 0 : 1;
}

int validaNomeProduto(int l) {
	return (isalnum(l) || strchr("!.#$%&", l) != NULL) ? 0 : 1;
}

int validador(char *bufferTeste, int comprimento, int (*callback)(int)) {
	for (int letra = 0; letra < comprimento; letra++) {
		if (!isprint(bufferTeste[letra]) ||  bufferTeste[letra] == '\v' || callback(bufferTeste[letra]) == 1) {
			return 1;
		}
	}

	return 0;
}

void geraNomeQuery(char *nome, char *senha, int nv_acesso, char *email) {
	// O que deveria ser guardado na verdade é o hash da senha, algo como HASH_SH512(senha)
	printf("\nINSERT INTO USUARIO VALUES (%d, \'%s\', \'%s\', %d, \'%s\');\n", idAtual, nome, senha, nv_acesso, email);
}

void geraEntrada(produto *lista, int novoId, int *controle, char *nome, int quantidade, char *validade, char *nivelPrioridade) {
	if (novoId == 0) {
		lista[novoId].id = 1;
	}
	else {
		lista[novoId].id = lista[novoId - 1].id + 1;
	}
	strcpy(lista[novoId].nome, nome);
	lista[novoId].quantidade = quantidade;
	strcpy(lista[novoId].validade, validade);
	strcpy(lista[novoId].nivelPrioridade, nivelPrioridade);

	*controle += 1;
}

int main() {
	// LEIAME: Declare variáveis abaixo já inicializadas
	int status = 0;
	int admFlag = 0;
	int flagErro = 0;
	int demoMode = 0;
	int nvAcessoLocal = 0;
	char nomeUsuario[LIMITE_ENTRADA + 1];
	char senhaNova[17];
	char emailUsuario[LIMITE_ENTRADA + 1];

	primeiraTela();

	char *bufferEntrada = NULL;
	size_t tamanhoAlocado = 0;
	ssize_t comprimentoTexto = 0;
	
	// Repetiçao principal
	do {
		primeiraTela();

		do {
			printf("Digite uma das operações acima: ");

			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 1;
				bufferEntrada[0] = 'c';
				break;
			}
			if (comprimentoTexto > 2) {
				bufferEntrada[0] = '\0';
			}
			if (bufferEntrada[0] == 'a') {
				admFlag = 1;
				primeiraTela();
			}
			else if (bufferEntrada[0] == 'd') {
				demoMode = 1;
				primeiraTela();
			}
		} while (bufferEntrada[0] != '0' && bufferEntrada[0] != '1' && bufferEntrada[0] != 'c');

		if (bufferEntrada[0] == 'c') continue;

		if (bufferEntrada[0] == '1') {
			criacaoTela();
			printf("Digite o nome do novo usuário: ");
			
			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 2;
				break;
			}
			if (comprimentoTexto >= 3 && comprimentoTexto <= LIMITE_ENTRADA) {
				if (validador(bufferEntrada, comprimentoTexto, validaLetraNome)) {
					removeQuebraLinha(bufferEntrada);
					strcpy(nomeUsuario, bufferEntrada);
				}
				else {
					flagErro = 1;
				}
			}
			else {
				puts("Por favor, abrevie o nome...\n");
				status = 3;
				break;
			}

			printf("Digite o nível de acesso do novo usuário (1-2): ");

			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 4;
				break;
			}
			if (comprimentoTexto == 2) {
				if (bufferEntrada[0] == '1') {
					nvAcessoLocal = 1;
				}
				else if (bufferEntrada[0] == '2') {
					nvAcessoLocal = 2;
				}
				else {
					flagErro = 1;
				}
			}
			else {
				puts("Por favor, digite um nível de acesso válido...\n");
				status = 5;
				break;
			}

			printf("Digite o e-mail do novo usuário (joao@vocesa.com): ");
			
			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 6;
				break;
			}
			if (comprimentoTexto >= 5 && comprimentoTexto <= LIMITE_ENTRADA) {
				if (validador(bufferEntrada, comprimentoTexto, validaEmail)) {
					removeQuebraLinha(bufferEntrada);
					strcpy(emailUsuario, bufferEntrada);
				}
				else {
					flagErro = 1;
				}
			}
			else {
				puts("Por favor, digite um e-mail válido...\n");
				status = 7;
				break;
			}

			printf("Confira as informações acima.\nDeseja continuar? (S/n) ");

			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 8;
				break;
			}
			if (comprimentoTexto == 2) {
				if (bufferEntrada[0] == 'S' || bufferEntrada[0] == 's') {
					if (flagErro == 1) {
						puts("Alguma das informações acima está incorreta.\n");
						status = 9;
						break;
					}
					else {
						idAtual++;
						geradorSenha(senhaNova);

						if (admFlag == 1) {
							geraNomeQuery(nomeUsuario, senhaNova, nvAcessoLocal, emailUsuario);
							sleep(10);
						}
					}

					printf("Usuário instanciado com sucesso.\n");
					printf("Deseja prosseguir para login? (S/n) ");

					if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
						puts(MENSAGEM_DE_ERRO);
						status = 10;
						break;
					}
					if (comprimentoTexto == 2) {
						if (bufferEntrada[0] == 'S' || bufferEntrada[0] == 's') {
							bufferEntrada[0] = '0';
						}
					}
				}
			}
		}

		if (bufferEntrada[0] == '0') {
			char testeUsuario[LIMITE_ENTRADA + 1];
			char senhaTeste[17];
			limpaTela();
			printf("Login\n\n");
			printf("Nome: ");

			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 11;
				break;
			}
			if (comprimentoTexto >= 3 && comprimentoTexto <= LIMITE_ENTRADA) {
				if (validador(bufferEntrada, comprimentoTexto, validaLetraNome)) {
					removeQuebraLinha(bufferEntrada);
					strcpy(testeUsuario, bufferEntrada);
				}
				else {
					flagErro = 1;
				}
			}

			printf ("Senha: ");

			if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
				puts(MENSAGEM_DE_ERRO);
				status = 12;
				break;
			}

			if (comprimentoTexto == 17) {
				if (validador(bufferEntrada, comprimentoTexto, validaSenha)) {
					removeQuebraLinha(bufferEntrada);
					strcpy(senhaTeste, bufferEntrada);
				}
				else {
					flagErro = 1;
				}
			}
			if (flagErro == 1) {
				puts("Por favor, digite apenas informações inválidas...\n");
				sleep(10);
			}
			else {
				if (demoMode != 1) {
					// Essas variáveis abaixo ainda são buffers, não possuem terminador nulo
					if (verificarCredenciais(testeUsuario, senhaTeste, nomeUsuario, senhaNova) == 1) {
						puts("Acesso negado...\n");
						sleep(10);
						continue;
					}
					// else {
						// salvarUsuarioLogado(nome);
					// }
				}
				if (admFlag == 1) {
					printf("\nPara checar usuário:\nSELECT NOME, SENHA FROM USUARIO WHERE NOME = \'%s\' AND SENHA = hash(\'%s\');\n", testeUsuario, senhaTeste);
					sleep(10);
				}

				// Aqui deveria ocorrer a conexão com o usuário passado no banco
				int controleTela = 1;

				produto lista[100] = {
					{ .id = 1001, .nome = "Chegada de servidores", .quantidade = 5, .validade = "2035-05-28", .nivelPrioridade = "1" },
					{ .id = 1002, .nome = "Entrega de notebooks", .quantidade = 10, .validade = "2028-02-10", .nivelPrioridade = "2" },
					{ .id = 1003, .nome = "Instalação de switches", .quantidade = 7, .validade = "2033-04-30", .nivelPrioridade = "3" }
				};
				int tamanhoLista = 3;
				for (int item = 2; item < 7; item++) {
					lista[item].id = 0;
				}

				do {
					int idItem = 0;

					estoqueTela(lista, tamanhoLista, &tamanhoLista, controleTela, &controleTela);

					printf("\n");
					printf("\033[107;94m[s] selecionar um item [a] adicionar item [v] voltar uma página [p] passar uma página [c] sair\033[K ");

					if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
						puts(MENSAGEM_DE_ERRO);
						status = 13;
						bufferEntrada[0] = 'c';
						break;
					}
					
					printf("\033[0m");
					
					if (comprimentoTexto > 2) {
						bufferEntrada[0] = '\0';
					}
					if (bufferEntrada[0] == 'v' && controleTela > 1) {
						controleTela--;
						printf("\n\033[0m");
						continue;
					}
					else if (bufferEntrada[0] == 'p' && tamanhoLista > 7) {
						controleTela++;
						printf("\n\033[0m");
						continue;
					}
					else if (bufferEntrada[0] == 'a') {
						char nomeProduto[81];
						int quantidadeProduto;
						char validadeProduto[11];
						char nivelPrioridadeProduto[2];
						char numeroLote[11];

						printf("Digite o nome do novo item: ");

						if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
							puts(MENSAGEM_DE_ERRO);
							status = 14;
							bufferEntrada[0] = 'c';
							break;
						}
						if (comprimentoTexto <= 81) {
							if (validador(bufferEntrada, comprimentoTexto, validaNomeProduto)) {
								removeQuebraLinha(bufferEntrada);
								strcpy(nomeProduto, bufferEntrada);
							}
							else {
								flagErro = 1;
							}
						}
						else {
							flagErro = 2;
						}

						printf("Digite a quantidade do item: ");

						if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
							puts(MENSAGEM_DE_ERRO);
							status = 15;
							bufferEntrada[0] = 'c';
							break;
						}
						if (comprimentoTexto <= 6) {
							if (validador(bufferEntrada, comprimentoTexto, validaNumero)) {
								removeQuebraLinha(bufferEntrada);
								quantidadeProduto = atoi(bufferEntrada);
							}
							else {
								flagErro = 3;
							}
						}
						else {
							flagErro = 4;
						}

						printf("Digite a validade do item no formato padrão ou [i] para indeterminada: ");
						
						if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
							puts(MENSAGEM_DE_ERRO);
							status = 16;
							bufferEntrada[0] = 'c';
							break;
						}
						if (comprimentoTexto <= 11) {
							if (comprimentoTexto == 2 && bufferEntrada[0] == 'i') {
								strcpy(validadeProduto, "null");
							}
							else if (validador(bufferEntrada, comprimentoTexto, validaData)) {
								removeQuebraLinha(bufferEntrada);
								struct tm testeTimestamp = {0};

								if (strptime(bufferEntrada, "%Y-%m-%d", &testeTimestamp) != NULL) {
									strcpy(validadeProduto, bufferEntrada);
								}
								else {
									flagErro = 5;
								}
							}
							else {
								flagErro = 6;
							}
						}
						else {
							flagErro = 7;
						}

						printf("Digite o nível de prioridade (1-2-3): ");

						if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
							puts(MENSAGEM_DE_ERRO);
							status = 17;
							bufferEntrada[0] = 'c';
							break;
						}
						if (comprimentoTexto == 2) {
							if (bufferEntrada[0] == '1' || bufferEntrada[0] == '2' || bufferEntrada[0] == '3') {
								nivelPrioridadeProduto[0] = bufferEntrada[0];
							}
							else {
								flagErro = 8;
							}
						}
						else {
							flagErro = 9;
						}

						printf("Digite o número de lote (opcional): ");

						if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
							puts(MENSAGEM_DE_ERRO);
							status = 18;
							bufferEntrada[0] = 'c';
							break;
						}
						if (comprimentoTexto <= 11) {
							if (comprimentoTexto != 1) {
								if (validador(bufferEntrada, comprimentoTexto, validaNumero)) {
									strcpy(numeroLote, bufferEntrada);
								}
								else {
									flagErro = 10;
								}
							}
							else {
								strcpy(numeroLote, "null");
							}
						}
						else {
							flagErro = 11;
						}

						if (flagErro > 0) {
							switch (flagErro) {
								case 1:
								printf("Erro: Nome de produto inválido...\n");
								break;
								case 2:
								printf("Erro: Nome de produto muito grande...\n");
								break;
								case 3:
								printf("Erro: Quantidade inválido...\n");
								break;
								case 4:
								printf("Erro: Quantidade superior ao limite...\n");
								break;
								case 5:
								printf("Erro: Validade inválida...\n");
								break;
								case 6:
								printf("Erro: Parâmetro inválido no campo da validade...\n");
								break;
								case 7:
								printf("Erro: Validade fora do range...\n");
								break;
								case 8:
								printf("Erro: Nível de Prioridade inválida...\n");
								break;
								case 9:
								printf("Erro: Por favor, digite um nível de prioridade apropriado (1 ou 2 ou 3)...\n");
								break;
								case 10:
								printf("Erro: N° de lote inválido...\n");
								break;
								case 11:
								printf("Erro: N° de lote muito grande...\n");
								break;
							}

							sleep(10);
						}
						else {
							geraEntrada(lista, tamanhoLista, &tamanhoLista, nomeProduto, quantidadeProduto, validadeProduto, nivelPrioridadeProduto);
							if (admFlag == 1) {
								printf("\nINSERT INTO ENTRADA (NOME, QNTD, VALIDADE, NV_PRI, N_LOTE) VALUES (\'%s\', %d, \'%s\', %s, %s);\n",
								 nomeProduto, quantidadeProduto, validadeProduto, nivelPrioridadeProduto, numeroLote);
								sleep(10);
							}

							printf("\033[0m\n");
							continue;
						}
					}
					else if (bufferEntrada[0] == 's') {
						printf("\033[106mDigite o número entre [] de um item para fazer uma operação ou [c] para cancelar: \033[K");

						if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
							puts(MENSAGEM_DE_ERRO);
							status = 19;
							bufferEntrada[0] = 'c';
							break;
						}

						printf("\033[0m");
						
						if (comprimentoTexto == 2) {
							if (bufferEntrada[0] == 'c') {
								bufferEntrada[0] = '\0';
								printf("\033[0m\n");
								continue;
							}
							if (validador(bufferEntrada, comprimentoTexto, validaId)) {
								removeQuebraLinha(bufferEntrada);
								if (controleTela == 1) {
									idItem = atoi(bufferEntrada) - 1;
								}
								else {
									idItem = atoi(bufferEntrada) + (7 * (controleTela - 1));
								}

								printf("\033[46m[i] incrementar [d] decrementar [m] mudar quantidade [t] trocar validade [r] remover item [c] cancelar \033[K");

								if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
									puts(MENSAGEM_DE_ERRO);
									status = 20;
									bufferEntrada[0] = 'c';
									break;
								}

								printf("\033[0m");
								
								if (comprimentoTexto == 2) {
									if (bufferEntrada[0] == 'c') {
										bufferEntrada[0] = '\0';
										printf("\033[0m\n");
										continue;
									}
									if (bufferEntrada[0] == 'i') {
										mudaQuantidade(lista, idItem, tamanhoLista, &tamanhoLista, ++lista[idItem].quantidade);
										if (admFlag == 1) {
											printf("\nUPDATE ENTRADA SET QNTD = %d WHERE ID = %d;\n", lista[idItem].quantidade, lista[idItem].id);
											sleep(10);
										}
										printf("\033[0m\n");
										continue;
									}
									else if (bufferEntrada[0] == 'd') {
										mudaQuantidade(lista, idItem, tamanhoLista, &tamanhoLista, --lista[idItem].quantidade);
										if (admFlag == 1) {
											printf("\nUPDATE ENTRADA SET QNTD = %d WHERE ID = %d;\n", lista[idItem].quantidade, lista[idItem].id);
											sleep(10);
										}
										printf("\033[0m\n");
										continue;
									}
									else if (bufferEntrada[0] ==  'm') {
										printf("\033[104mDigite a nova quantidade ou [c] para cancelar: \033[K");

										if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
											puts(MENSAGEM_DE_ERRO);
											status = 21;
											bufferEntrada[0] = 'c';
											break;
										}

										printf("\033[0m");

										if (comprimentoTexto <= 6) {
											if (comprimentoTexto == 2) {
												if (bufferEntrada[0] == 'c') {
													bufferEntrada[0] = '\0';
													printf("\n\033[0m");
													continue;
												}
											}
											if (validador(bufferEntrada, comprimentoTexto, validaNumero)) {
												removeQuebraLinha(bufferEntrada);
												mudaQuantidade(lista, idItem, tamanhoLista, &tamanhoLista, atoi(bufferEntrada));
												if (admFlag == 1) {
													printf("\nUPDATE ENTRADA SET QNTD = %d WHERE ID = %d;\n", atoi(bufferEntrada), lista[idItem].id);
													sleep(10);
												}
												printf("\033[0m\n");
												continue;
											}
										}
									}
									else if (bufferEntrada[0] == 't') {
										printf("\033[104mDigite a nova validade no formato (yyyy-mm-dd) ou [i] para indeterminada ou [c] para cancelar \033[K");

										if ((comprimentoTexto = getline(&bufferEntrada, &tamanhoAlocado, stdin)) == -1) {
											puts(MENSAGEM_DE_ERRO);
											status = 22;
											bufferEntrada[0] = 'c';
											break;
										}

										printf("\033[0m");

										if (comprimentoTexto <= 11) {
											if (comprimentoTexto == 2) {
												if (bufferEntrada[0] == 'c') {
													bufferEntrada[0] = '\0';
												}
												else if (bufferEntrada[0] == 'i') {
													trocaValidade(lista, idItem, "null");
													if (admFlag == 1) {
														printf("\nUPDATE ENTRADA SET VALIDADE = NULL WHERE ID = %d;\n", lista[idItem].id);
														sleep(10);
													}
												}
											}
											else if (validador(bufferEntrada, comprimentoTexto, validaData)) {
												removeQuebraLinha(bufferEntrada);
												struct tm testeTimestamp = {0};
												
												if (strptime(bufferEntrada, "%Y-%m-%d", &testeTimestamp) != NULL) {
													trocaValidade(lista, idItem, bufferEntrada);
													if (admFlag == 1) {
														printf("\nUPDATE ENTRADA SET VALIDADE = \'%s\' WHERE ID = %d;\n", bufferEntrada, lista[idItem].id);
														sleep(10);
													}
												}
											}

											printf("\033[0m\n");
											continue;
										}
									}
									else if (bufferEntrada[0] == 'r') {
										if (admFlag == 1) {
											printf("\nDELETE FROM ENTRADA WHERE ID = %d;\n", lista[idItem].id);
											sleep(10);
										}
										removeEntrada(lista, lista[idItem].id, tamanhoLista, &tamanhoLista);

										printf("\033[0m\n");
										continue;
									}
								}
							}
						}
						else {
							puts("Por favor, digite um número de ID válido...\n");
							status = 23;
							bufferEntrada[0] = 'c';
							break;
						}
					}
				} while (bufferEntrada[0] != 'c');
				if (bufferEntrada[0] == 'c') bufferEntrada[0] = '\0';
				printf("\033[0m\n");
			}
		}
	} while (bufferEntrada[0] != 'c');
	
	// ATENÇÃO: Não esquecer de limpar um buffer após usar
	free(bufferEntrada);
	
	return status;
}
