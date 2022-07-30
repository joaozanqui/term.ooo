#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define esq 75
#define dir 77
#define esc 27
#define enter 13
#define BS 8
#define F1 59
#define F2 60

typedef struct
{
    char letra;
    int cor;
    int pos;
} word;

word text[20][5];
word letras[26];

typedef struct reg *no;

struct reg
{
    char palavra[6];
    no prox, ant;
};

typedef struct
{
    no prim, ult;
    int qte;
} Descritor;

/* --------------------------------------------------------------------------------------------------------- */

void gotoxy(int x, int y)
{ // 80 / 24
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int color(char color)
{
    HANDLE h;
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    return SetConsoleTextAttribute(h, color);
}

void texto(int x, int y, char texto[], char cor)
{
    color(cor);
    gotoxy(x, y);
    printf("%s", texto);
}

void selecao(int x, int y, char texto[], char cor)
{
    color(cor);
    gotoxy(x, y);
    printf("%s", texto);
    gotoxy(x + 2, y + 1);
    printf("^");
}

void colocarLetra(int x, int y, char letra, char cor)
{
    color(cor);
    gotoxy(x, y);
    printf("%c", letra);
}

void selecaoLetra(int x, int y, char letra, char cor, word text)
{
    color(text.cor);
    gotoxy(x, y);
    printf("%c", letra);
    color(cor);
    gotoxy(x, y + 1);
    printf("^");
}

/* --------------------------------------------------------------------------------------------------------- */

void colocaPalavras(Descritor *lista, char palavra[])
{
    no p;
    p = (no)malloc(sizeof(struct reg));
    strcpy(p->palavra, palavra);
    p->prox = NULL;
    p->ant = NULL;
    if ((*lista).qte == 0)
        (*lista).prim = p;
    else
    {
        p->ant = (*lista).ult;
        (*lista).ult->prox = p;
    }

    (*lista).ult = p;
    (*lista).qte++;
}

Descritor criaLista()
{
    FILE *file;
    char palavra[6];
    int i;

    Descritor lista;

    lista.prim = lista.ult = NULL;
    lista.qte = 0;

    if (!(file = fopen("Palavras5.txt", "r")))
    {
        printf("NAO FOI POSSIVEL ACESSAR O ARQUIVO COM AS PALAVRAS...\n");
        return lista;
    }

    while (fscanf(file, "%s", palavra) != EOF)
    { // Coloca as palavras do arquivo na lista
        for (i = 0; i < 5; i++)
            palavra[i] = toupper(palavra[i]);
        colocaPalavras(&lista, palavra);
    }
    fclose(file);

    return lista;
}

void menu(int opc, int n, int cor, int ntext)
{
    int i, j = 0, k;

    char x[6][1000] = {"TERMO - Joao Zanqui - jpzanqui@hotmail.com", "-> tente acertar a palavra com 10 tentativas <-", "ENTER - Enviar a palavra", "F2 - Reiniciar",
                       "ESC - SAIR", "_____"};

    for (i = 0; i < n; i++)
    {
        if (i < 5)
        {
            texto(40, 3 + i, x[i], 15);
        }
        else
        {
            for (j = 0; j <= ntext; j++)
            {
                if (opc + 5 == i && j == ntext)
                {
                    if (text[j][i - 5].letra != NULL)
                    {
                        selecaoLetra(12 + 6 * i, 9 + j * 2, text[j][i - 5].letra, cor, text[j][i - 5]);
                    }
                    else
                    {
                        selecao(10 + 6 * i, 9 + j * 2, x[5], cor);
                    }
                }
                else
                {
                    if (text[j][i - 5].letra == NULL)
                        texto(10 + 6 * i, 9 + j * 2, x[5], 4);
                    else
                        colocarLetra(12 + 6 * i, 9 + j * 2, text[j][i - 5].letra, text[j][i - 5].cor);
                }
            }
        }
    }
    color(15);
    gotoxy(6, 10);
    printf("Letras disponiveis:");
    for (i = 0, j = 0, k = 0; i < 26; i++, k++)
    {
        gotoxy(2 + k * 2, j + 12);
        if (i % 13 == 0 && i != 0)
            j++;
        if (k % 13 == 0)
            k = 0;
        color(letras[i].cor);
        printf("%c", letras[i].letra);
        color(15);
    }

    gotoxy(0, 0);
}

void restart(Descritor *lista, int *opc, int *cor, int *ntext, char palavra[])
{
    int i, j;
    for (i = 0; i < 20; i++)
    { // Limpa as palavras e cores que ser�o escritas
        for (j = 0; j < 5; j++)
        {
            text[i][j].letra = NULL;
            text[i][j].cor = 15;
        }
    }

    for (i = 0; i < 26; i++)
    { // Adiciona as letras ao menu
        letras[i].letra = (char)i + 65;
        letras[i].cor = 15;
    }

    *lista = criaLista();
    *opc = 0;
    *cor = 15;
    *ntext = 0;

    int numAleatorio = 1 + rand() % ((*lista).qte + 1);
    no contLista = (*lista).prim;
    for (i = 0; i < numAleatorio; i++)
        contLista = contLista->prox;
    strcpy(palavra, contLista->palavra);
}

int main()
{

    int i, j, k;
    int apag = 0, opc = 0, cor = 15, ntext = 0, n = 10, cont = 0, repete, repeteResposta;
    char t, plv[6];

    srand((unsigned)time(NULL));

    Descritor lista = criaLista();

    char palavra[6];
    restart(&lista, &opc, &cor, &ntext, palavra);
    // strcpy(palavra, "RAMPA");

    do
    {
        system("cls");
        menu(opc, n, cor, ntext);

        if (text[ntext - 1][0].cor == 2 && text[ntext - 1][1].cor == 2 && text[ntext - 1][2].cor == 2 && text[ntext - 1][3].cor == 2 && text[ntext - 1][4].cor == 2)
        {
            color(15);
            gotoxy(40, 9 + ntext * 2);
            printf("PARABENS VOCE ACERTOU!!! A palavra era %s...\n", palavra);
            restart(&lista, &opc, &cor, &ntext, palavra);
        }

        else if (ntext >= 10) // se ganhar nao pode entrar aqui
        {
            color(15);
            gotoxy(40, 9 + ntext * 2);
            printf("BURRO!!! A palavra era %s...\n", palavra);
            restart(&lista, &opc, &cor, &ntext, palavra);
        }

        t = getch();

        if (t < 0)
        {
            apag = 0;
            t = getch();
            system("cls");

            if (t == dir)
            {
                if (opc < 4)
                    opc++;
                menu(opc, n, cor, ntext);
            }

            else if (t == esq)
            {
                if (opc > 0)
                    opc--;
                menu(opc, n, cor, ntext);
            }
        }

        else if (toupper(t) >= 65 && toupper(t) <= 90)
        {
            apag = 0;
            int vazio = 0;
            if (text[ntext][opc].letra == NULL)
                vazio = 1;
            text[ntext][opc].letra = toupper(t);
            text[ntext][opc].cor = cor;
            text[ntext][opc].pos = opc;
            if (vazio == 1)
            {
                cont++;
                if (cont < 5)
                    opc++;
                if (opc > 4)
                    opc = 0;
                while (text[ntext][opc].letra != NULL && cont < 5)
                {
                    opc++;
                    if (opc > 4)
                        opc = 0;
                }
            }
            menu(opc, n, cor, ntext);
        }

        else if (t == enter)
        {
            int existe = 0;
            int incompleto = 0;
            for (i = 0; i < 5; i++)
            {
                if (text[ntext][i].letra == NULL)
                {
                    incompleto = 1;
                    break;
                }
            }

            if (!incompleto)
            {
                int existe = 0;

                // Armazenar a palavra em uma variavel
                no q = lista.prim;

                for (i = 0; i < 5; i++)
                    plv[i] = text[ntext][i].letra;
                for (i = 5; i < strlen(plv); i++)
                    plv[i] = NULL;

                // Descobrir se a palavra existe

                while (strcmp(q->palavra, plv) != 0 && q->prox != NULL)
                    q = q->prox;

                if (q->prox == NULL && strcmp(q->palavra, plv) != 0)
                    existe = 0;
                else
                    existe = 1;
                // -------------------------------------- //

                if (existe)
                {
                    for (i = 0; i < 5; i++)
                    {
                        repete = repeteResposta = 0;

                        // Ver se a letra repete na palavra escrita
                        for (j = i - 1; j >= 0; j--)
                            if (text[ntext][i].letra == text[ntext][j].letra)
                                repete++;

                        // Ver se a letra repete na resposta
                        for (j = 0; j < 5; j++)
                            if (text[ntext][i].letra == palavra[j])
                                repeteResposta++;

                        for (j = 0; j < 5; j++)
                        {
                            if (text[ntext][i].letra == palavra[j])
                            {
                                if (i == j)
                                {
                                    text[ntext][i].cor = 2;
                                    if (repete && repeteResposta == 1)
                                    {
                                        for (k = i - 1; k >= 0; k--)
                                            if (text[ntext][i].letra == text[ntext][k].letra)
                                            {
                                                if (repeteResposta == 1)
                                                    text[ntext][k].cor = 15;
                                                else
                                                    text[ntext][k].cor = 6;
                                            }
                                    }
                                    break;
                                }
                                else
                                {
                                    if (repete != repeteResposta)
                                        text[ntext][i].cor = 6;
                                }
                                // printf("%d %d - %c", i, j, text[ntext][i].letra);
                                // printf(" - COR:%d - repete:%d - repResp:%d\n", text[ntext][i].cor, repete, repeteResposta);
                                // getch();
                                if (!repeteResposta)
                                    break;
                            }
                        }
                    }

                    for (i = 0; i < 5; i++)
                    { // Colorir as letras do menu...
                        if (text[ntext][i].cor == 15 && letras[(int)text[ntext][i].letra - 65].cor == 15)
                            letras[(int)text[ntext][i].letra - 65].cor = 4;
                        else if (text[ntext][i].cor != 15 && letras[(int)text[ntext][i].letra - 65].cor != 2)
                            letras[(int)text[ntext][i].letra - 65].cor = text[ntext][i].cor;
                    }

                    ntext++;
                    opc = 0;
                    cor = 15;
                    cont = 0;
                    apag = 0;
                }
                else
                {
                    gotoxy(40, 11 + ntext * 2);
                    printf("Essa Palavra nao eh aceita...");
                    getch();
                }
            }
        }

        else if (t == esc)
        {
            color(15);
            system("cls");
            return 0;
        }

        else if (t == BS)
        {
            if (text[ntext][opc].letra != NULL)
            {
                text[ntext][opc].letra = NULL;
                text[ntext][opc].cor = 15;
                menu(opc, n, cor, ntext);
                cont--;
                if (apag == 1 && opc != 0)
                    opc--;
            }
            else if (opc != 0)
                opc--;
            apag = 1;
        }

        else if (t == F2)
            restart(&lista, &opc, &cor, &ntext, palavra);

        // else if (t == F1)
        // {
        //     gotoxy(0, 0);
        //     color(15);
        //     printf("palavra %s\n", palavra);
        //     getch();
        // }

        else
            menu(opc, n, cor, ntext);
    } while (t != esc);

    return 0;
}