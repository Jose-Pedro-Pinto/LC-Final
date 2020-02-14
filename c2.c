#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char username[20];
    char password[18];
    char nome[40];
    int contacto;
    float saldo;
    float lC; // Preço da lista de compras
    int comprado; // Numero de produtos comprados
    float gasto; // Montante gasto
} Utilizador; // A classe que mantem as informações do Cliente que está activo

typedef struct
{
    int codigo;
    char nome[30];
    char descricao[100];
    int qE;
    float preco;
    float custo;
    int qV;
    float TotalFeito; // Montante ganho no produto
    float TotalGasto; // Lucro
} Produto; // Mantem as informaçoes dos produtos existentes

typedef struct
{
    int op;
    Utilizador cliente;
} bool;

typedef struct
{
    int u;
    int p;
    int t;
    float g;
    float f;
} stats; // A classe que mantem as estatisticas do Cliente que está activo


void Csave(Utilizador g)
{
    char cod[100];
    strcpy(cod,"grep -vw ");
    strcat(cod,g.username);
    strcat(cod," user.txt > f");
    system(cod);
    FILE *fu = fopen("f","a"); // Abre o fixeiro das informações do cliente
    fprintf(fu,"%s;%s;%s;%d;%.2f;%d;%.2f\n",g.username,g.password,g.nome,g.contacto,g.saldo,g.comprado,g.gasto); // Guarda as informações
    fclose(fu);
    system("sort f > user.txt");
    system("rm -f f");
}  // Guarda as informações do cliente num fixeiro de texto

void Psave(Produto p)
{
    char d[5],cod[80];
    strcpy(cod,"grep -vw '");
    strcat(cod,p.descricao);
    strcat(cod,"' product.txt > f");
    system(cod);
    FILE *fp = fopen("f","a"); // Abre o fixeiro das informações dos produtos
    fprintf(fp,"%d;%s;%s;%d;%.2f;%.2f;%d;%.2f;%.2f\n",p.codigo,p.nome,p.descricao,p.qE,p.preco,p.custo,p.qV,p.TotalFeito,p.TotalGasto); // Guarda os novos dados
    fclose(fp);
    system("sort f > product.txt");
    system("rm -f f");
}  // Guarda as novas informaçoes de um produto no fixeiro dos produtos após uma compra

void saveStats(char s[],stats a)
{
    char cod[60];
    strcpy(cod,s);
    strcat(cod,"-stats.txt");
    FILE *fs = fopen(cod,"w");
    fprintf(fs,"%d;%d;%d;%.2f;%.2f\n",a.u,a.p,a.t,a.g,a.f);
    fclose(fs);
} // Guarda as estatisticas do utilizador num fixeiro

void lCsave(char u[],Produto lC[],int c)
{
		char cod[80];
		strcpy(cod,u);strcat(cod,"-lC.txt");
		FILE *fx = fopen(cod,"w");
		for(int i=0; i<c; i++)
			fprintf(fx,"%d;%s;%d;%.2f\n",lC[i].codigo,lC[i].nome,lC[i].qE,lC[i].preco);
		fclose(fx);
		strcpy(cod,"sort ");strcat(cod,u);strcat(cod,"-lC.txt > f");
		system(cod);
		strcpy(cod,"cat f > ");strcat(cod,u);strcat(cod,"-lC.txt");
		system(cod);
		system("rm f");
}

int Uexiste(char u[])
{
    char cod[60],d[5];
    strcpy(cod,"grep -w ");
    strcat(cod,u);
    strcat(cod," user.txt | wc -l > f");
    system(cod);
    FILE *f = fopen("f","r");
    fgets(d,5,f);
    fclose(f);
    system("rm f");
    return atoi(d);
} // verifica se o utilizador existe

Utilizador loadUser(char u[])
{
    Utilizador g;
    char cod[120],d[30];
    int i,j;

    strcpy(cod,"grep -w ");
    strcat(cod,u);
    strcat(cod," user.txt > f");
    system(cod);

    FILE *fx = fopen("f","r");
    fgets(cod,120,fx);
    fclose(fx);

    for(i=0; cod[i]!=';'; i++) g.username[i] = cod[i];
    g.username[i]='\0';
    i++; // Guarda o username
    for(j=0; cod[i]!=';'; i++) g.password[j++] = cod[i];
    g.password[j]='\0';
    i++; // Guarda a password
    for(j=0; cod[i]!=';'; i++) g.nome[j++] = cod[i];
    g.nome[j]='\0';
    i++; // Guarda o nome
    for(j=0; cod[i]!=';'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    g.contacto = atoi(d); // Guarda o contacto
    for(j=0; cod[i]!=';'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    g.saldo = atof(d); // Guarda o saldo
    for(j=0; cod[i]!=';'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    g.comprado = atoi(d); // Guarda o total comprado
    for(j=0; cod[i]!='\n'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    g.gasto = atof(d); // Guarda o total gasto
    system("rm f");
    return g;
} // Carrega as informações do cliente para o programa a partir do fixeiro onde estão guardadas

Produto loadProduto(char d[])
{
    Produto p;
    char cod[250];
    strcpy(cod,"grep -w ");
    strcat(cod,d);
    strcat(cod," product.txt > f");
    system(cod);
    FILE *fx = fopen("f","r");
    fscanf(fx,"%d;%s;%s;%d;%f;%f;%d;%f;%f\n",&p.codigo,p.nome,p.descricao,&p.qE,&p.preco,&p.custo,&p.qV,&p.TotalFeito,&p.TotalGasto);
    fclose(fx);
    system("rm f");
    return p;
} // Carrega as informações do produto para o programa a partir do fixeiro onde estão guardadas

stats loadStats(char s[])
{
    char d[10],cod[80];
    stats a;
    strcpy(cod,s);
    strcat(cod,"-stats.txt");

    FILE *fs = fopen(cod,"r");
    fgets(cod,60,fs);
    fclose(fs);

    int i,j;
    for(i=0; cod[i]!=';'; i++) d[i] = cod[i];
    d[i]='\0';
    i++;
    a.u = atoi(d);

    for(j=0; cod[i]!=';'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    a.p = atoi(d);

    for(j=0; cod[i]!=';'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    a.t = atoi(d);

    for(j=0; cod[i]!=';'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    a.g = atof(d);

    for(j=0; cod[i]!='\n'; i++) d[j++] = cod[i];
    d[j]='\0';
    i++;
    a.f = atof(d);

    return a;
} // Carrega as diversas estatisticas para o programa a partir do fixeiro onde estão guardadas

bool login()
{
    bool a;
    char u[20],p[18];
    printf("** LOGIN **\n");
    printf("Username: ");
    scanf(" %s",u);
    printf("Password: ");
    scanf(" %s",p);

    if(Uexiste(u)!=1)  // Verifica se o utilizador existe
    {
        a.op = 1;
        return a;
    }

    a.cliente = loadUser(u);

    // Comparar password do ficheiro com a lida do stdin
    long int c = strlen(a.cliente.password);
    if(strlen(p) != c)
    {
        a.op = 3;
        return a;
    }
    if(strncmp(p,a.cliente.password,c)!=0)  // Verifica se a password está correta
    {
        a.op = 4;
        return a;
    }
    a.op = 0;
    return a;
} // Faz as operações para o login,verifica se o utilizador existe e se a passe corresponde a esse utilizador

void menu()
{
    printf("\n$ SUPCliente\n");
    printf("** MENU **\n");
    printf("1) Gerir Saldo\n");
    printf("2) Gerir Lista de Compras\n");
    printf("3) Ver Estatísticas\n");
    printf("4) Alterar password\n");
    printf("5) Logout\n");
}   // Escreve o menu de entrada

int Gsaldo(Utilizador g)
{
    printf("\n$ SUPCliente\n");
    printf("O seu saldo atual é %.2f €\n",g.saldo);
    printf("\n1) Adicionar saldo\n");
    printf("2) Voltar a trás\n");
    float op;
    scanf(" %f",&op);
    if(op==1)  // Adiciona um valor ao saldo
    {
        printf("Montante a adicionar: ");
        scanf(" %f",&op);
        if(op>=0)
        {
            g.saldo += op;
            Csave(g);
            return 0;
        }
        else
        {
            printf("Montante impróprio.\n");
            return 1;
        }
    }
    return 2;
}   // Permite ao utilizador adicionar um montande ao seu saldo

void le_base_dados(Produto DB[], int c)
{
    char cod[250],k[15];
    int n=0,i,j;
    FILE *fp = fopen("product.txt","r");
    while(c-->0)
    {
        fgets(cod,250,fp);
        for(i=0; cod[i]!=';'; i++)	k[i]=cod[i];
        k[i]='\0';
        i++;
        DB[n].codigo = atoi(k); // Lê o codigo dos produtos
        for(j=0; cod[i]!=';'; i++) DB[n].nome[j++] = cod[i];
        DB[n].nome[j] = '\0';
        i++;
        for(j=0; cod[i]!=';'; i++) DB[n].descricao[j++] = cod[i];
        DB[n].descricao[j] = '\0';
        i++; // Lê a descrição dos produtos
        for(j=0; cod[i]!=';'; i++)	k[j++]=cod[i];
        k[j]='\0';
        i++;
        DB[n].qE = atoi(k);
        for(j=0; cod[i]!=';'; i++)	k[j++]=cod[i];
        k[j]='\0';
        i++;
        DB[n].preco = atof(k); // lê o preço dos produtos
        for(j=0; cod[i]!=';'; i++)	k[j++]=cod[i];
        k[j]='\0';
        i++;
        DB[n].custo = atof(k); // Lê o custo dos produtos
        for(j=0; cod[i]!=';'; i++)	k[j++]=cod[i];
        k[j]='\0';
        i++;
        DB[n].qV = atoi(k);
        for(j=0; cod[i]!=';'; i++)	k[j++]=cod[i];
        k[j]='\0';
        i++;
        DB[n].TotalFeito = atof(k); // Lê o lucro dos produtos
        for(j=0; cod[i]!='\n'; i++)	k[j++]=cod[i];
        k[j]='\0';
        i++;
        DB[n].TotalGasto = atof(k); // Lê a total gasto nos produtos
        n++;
    }
    fclose(fp);
} // Carrega os produtos existentes para o programa a partir do fixeiro onde estão guardados

void le_lista_compras(Produto DB[],char u[],int c)
{
    char cod[80],k[15];
    int i,j,p=0;
    strcpy(cod,u);
    strcat(cod,"-lC.txt");
    FILE *fp = fopen(cod,"r");
    while(p<c)
    {
				fgets(cod,250,fp);
				for(i=0; cod[i]!=';';i++)
					k[i] = cod[i];
				k[i] = '\0';
				i++;
				DB[p].codigo = atoi(k);
				for(j=0; cod[i]!=';';i++)
					DB[p].nome[j++] = cod[i];
				DB[p].nome[j] = '\0';
				i++;
				for(j=0;cod[i]!=';';i++)
					k[j++] = cod[i];
				k[j] = '\0';
				i++;
				DB[p].qE = atoi(k);
				for(j=0;cod[i]!='\n';i++)
					k[j++] = cod[i];
				k[j] = '\0';
				i++;
				DB[p].preco = atof(k);
        p++;
    }
    fclose(fp);
} // Carrega a lista de compras para o programa a partir do fixeiro onde esta guardada

void itoa(int a, char b[])
{
    int tamanho=0,temp = a,k=temp;
    while(temp/10 !=0)
    {
        tamanho++;
        temp/=10;
    };
    temp = k;
    for(int i=0; temp!=0; i++)
    {
        k/=10;
        temp -= k*10;
        b[i] = '0' + temp;
        temp/=10;
        k = temp;
    }
    b[tamanho+1]='\0';
} // Passa um inteiro para uma string

int isINlist(char u[],char p[])
{
    char cod[120];
    strcpy(cod,"grep -w ");
    strcat(cod,p);
    strcat(cod," ");
    strcat(cod,u);
    strcat(cod,"-lC.txt | wc -l > f");
    system(cod);
    FILE *fx = fopen("f","r");
    fgets(cod,20,fx);
    fclose(fx);
    system("rm -f");
    return atoi(cod);
} // Verifica se um dado produto está na lista de compras

int isEmptylC(char u[])
{
    char d[80];
    strcpy(d,"cat ");
    strcat(d,u);
    strcat(d,"-lC.txt | wc -l > f");
    system(d);
    FILE *fp = fopen("f","r");
    fgets(d,20,fp);
    fclose(fp);
    system("rm f");
    return atoi(d);
} // Verifica se a lista de compras está vazia

void showlC(Utilizador g)
{
    int p = isEmptylC(g.username);// Verifica se a lista está vazia
    printf(".-------------------------------------------------------------.\n");
    printf("|%-9s|%-30s|%-10s|%-9s|\n","Codigo","Nome","Quantidade","PVP");
    printf(".-------------------------------------------------------------.\n");
    Produto lC[p];
    le_lista_compras(lC,g.username,p);
    g.lC = 0;
    for(int i=0; i<p; i++)
    {
        printf("|%-9d|%-30s|%-10d|%-9.2f|\n",lC[i].codigo,lC[i].nome,lC[i].qE,lC[i].preco);
        g.lC += lC[i].qE * lC[i].preco;
    }
    printf(".--------------------------------------------------------------.\n");
    printf("\nTotal: %.2f €\n",g.lC);
    printf("Saldo: %.2f €\n",g.saldo);
} // Mostra a lista de compras atual

int addProd(Utilizador g)
{
    char d[100],w[20];
    system("cat product.txt | wc -l > f");
    FILE *fx = fopen("f","r");
    fgets(w,10,fx);
    fclose(fx);

    int c = atoi(w);
    if(c==0)
    {
        printf("De momento não existem produtos na base de dados.\n");
        return 2;
    }

    // Imprimir todos os produtos
    printf(".----------------------------------------------------------.\n");
    printf("|%9s|%30s|%10s|%9s|\n","Codigo","Nome","Quantidade","PVP");
    printf(".----------------------------------------------------------.\n");
    Produto DB[c];
    le_base_dados(DB,c);
    for(int i=0; i<c; i++)
        printf("|%9d|%30s|%10d|%9.2f|\n",DB[i].codigo,DB[i].nome,DB[i].qE,DB[i].preco);
    printf(".----------------------------------------------------------.\n");

		int k,i,j=-1;
    scanf(" %d",&k);
    for(i=0; i<c && j==-1; i++)
        if(DB[i].codigo == k)
            j=i;

    if(j==-1)
    {
        printf("Não existe nenhum produto com esse codigo.\n");
        return -2;
    }

    printf("Adicionar quanto? ");
    scanf(" %d",&i);
    if(i<=0 || i> DB[j].qE)
    {
        printf("Quantidade imprórpia.\n");
        return 3;
    }

		int max_lC = isEmptylC(g.username),n=-1;
		Produto lC[max_lC+1];
		le_lista_compras(lC,g.username,max_lC);
		for(int m=0;m<max_lC && n==-1; m++)
			if(lC[m].codigo == DB[j].codigo)
				n = m;
	
		if(n!=-1)
		{
				lC[n].qE += i;
				lCsave(g.username,lC,max_lC);
		}
		else
		{
				lC[max_lC].codigo = DB[j].codigo;
				strcpy(lC[max_lC].nome,DB[j].nome);
				lC[max_lC].qE = i;
				lC[max_lC].preco = DB[j].preco;
				lCsave(g.username,lC,max_lC+1);
		}

    printf("\n");
    printf("1) Continuar\n");
    printf("2) Voltar a trás\n");
    scanf(" %d",&j);
    return j;
} // Adiciona um produto á lista de compras

int remProd(Utilizador g)
{
    int c = isEmptylC(g.username);

    // Imprimir todos os produtos
    printf(".----------------------------------------------------------------------.\n");
    printf("|%9s|%30s|%10s|%9s|%9s|\n","Codigo","Nome","Quantidade","PVP","Custo");
    printf(".----------------------------------------------------------------------.\n");
    Produto DB[c];
    le_lista_compras(DB,g.username,c);
    for(int i=0; i<c; i++)
        printf("|%9d|%30s|%10d|%9.2f|%9.2f|\n",DB[i].codigo,DB[i].nome,DB[i].qE,DB[i].preco,DB[i].custo);
    printf(".----------------------------------------------------------------------.\n");

    char d[20];
    printf("Remover qual? ");
    scanf(" %s",d);
    int i,j=-1;
    for(i=0; i<c && j==-1; i++)
        if(DB[i].codigo==atoi(d))
            j=i;

    if(j==-1)
    {
        printf("Não existe nenhum produto com esse codigo.\n");
        return 2;
    }

    char cod[120];
    strcpy(cod,"grep -vw ");
    strcat(cod,DB[j].nome);
    strcat(cod," ");
    strcat(cod,g.username);
    strcat(cod,"-stats.txt > f");
    system(cod);
    strcpy(cod,"cat f > ");
    strcat(cod,g.username);
    strcat(cod,"-stats.txt");
    system(cod);
    system("rm f");

    printf("Produto removido com sucesso da lista de compras.\n");
    return 0;
} // Remove um produto da lista de compras

int pagar(Utilizador cliente,Produto b)
{
    Utilizador gestor = loadUser("gestor");
    stats q = loadStats("gestor");
    char w[10];
    itoa(b.codigo,w);
    Produto a = loadProduto(w);
    if(a.qE < b.qE)
        return -1;

    float d = b.qE * a.preco;

    cliente.saldo -= d;
    cliente.lC -=d;
    cliente.comprado += b.qE;
    cliente.gasto += d;

    gestor.saldo += d;
    q.t += b.qE;
    q.f += d;

    a.qE -= b.qE;
    a.qV += b.qE;
    a.TotalFeito += d;// Altera os diferentes valores associados ao protudo

    char cod[100];
    int l;
    float m;
    strcpy(cod,"grep -w ");
    strcat(cod,w);
    strcat(cod," ");
    strcat(cod,cliente.username);
    strcat(cod,"-stats.txt > f");
    system(cod);
    FILE *fx = fopen("f","r");
    fscanf(fx,"%d;%s;%d;%f\n",&l,cod,&l,&m);
    fclose(fx);

    strcpy(cod,"grep -vw ");
    strcat(cod,w);
    strcat(cod," ");
    strcat(cod,cliente.username);
    strcat(cod,"-stats.txt >> f");
    system(cod);
    fx = fopen("f","a");
    fprintf(fx,"%d;%s;%d;%.2f\n",a.codigo,a.nome,l+b.qE,m+d);
    fclose(fx);

    strcpy(cod,"sort f > ");
    strcat(cod,cliente.username);
    strcat(cod,"-stats.txt");
    system(cod);
    system("rm f");

    saveStats("gestor",q);
    Csave(cliente);
    Csave(gestor);
    Psave(a);
} // Permite ao cliente comprar os produtos na lista de compras e posteriormente pagar

void showStats(Utilizador g)
{
    printf("\n$ SUPCliente\n");
    printf("** ESTATÍSTICAS **\n");
    printf("Nome: %s\n",g.nome);
    printf("Contacto : %d\n",g.contacto);
    printf("Saldo: %.2f €\n",g.saldo);
    printf("Nº total de produtos comprados: %d\n",g.comprado);
    printf("Montante total gasto em compras: %.2f €\n",g.gasto);
} // Mostra as estatisticas relativas ao cliente

int changePassword(Utilizador g)
{
    char p1[18],p2[18],p3[18];
    printf("\n$ SUPCliente\n");
    printf("** ALTERAR PASSWORD **\n");
    printf("Password atual: ");
    scanf(" %s",p1);
    printf("Nova password: ");
    scanf(" %s",p2);
    printf("Confirme a nova password: ");
    scanf(" %s",p3);

    long int c = strlen(g.password);
    if(strlen(p1)!=c || strncmp(p1,g.password,c)!=0)
        return 1;
    c = strlen(p2);
    if(strlen(p3)!=c || strncmp(p2,p3,c)!=0)
        return 2;
    strcpy(g.password,p2);
    Csave(g);
    return 0;
} // Permite o cliente trocar a sua Password

void main()
{
    system("clear");
    bool a = login();
    while(a.op!=0)
    {
        printf("Username ou password errados. Tente outra vez.\n\n");
        a = login();
    }// Faz o login
    printf("\nBem vindo de volta %s!\n",a.cliente.nome);
    menu();
    scanf(" %d",&a.op);
    while(a.op!=5)  // Corre os diferentes menus e as seleções dentro dos tais
    {
        switch(a.op)
        {
        case 1:  // Permite as alterações aos saldos
        {
            if(Gsaldo(a.cliente)==0)
                printf("Saldo adicionado com sucesso.\n");
            a.cliente = loadUser(a.cliente.username);
            break;
        }
        case 2:  // Permite modificar a lista de compras e comprar os produtos
        {
            int op = isEmptylC(a.cliente.username);
            printf("\n$ SUPCliente\n");
            printf("1) Ver Lista de Compras\n");
            printf("2) Adicionar Produto\n");
            printf("3) Remover Produto\n");
            printf("4) Pagar\n");
            printf("5) Voltar a trás\n");
            scanf(" %d",&a.op);
            while(a.op!=5)
            {
                switch(a.op)
                {
                case 1:
                {
                    if(op==0)
                        printf("A sua lista de compras encontra-se vazia.\n");
                    else
                        showlC(a.cliente);
                    break;
                }
                case 2:
                {
                    while(addProd(a.cliente)!=2);
										
                    break;
                }
                case 3:
                {
                    if(op==0)
                        printf("A sua lista de compras encontra-se vazia.\n");
                    else
                        a.op = remProd(a.cliente);
                    break;
                }
                case 4:
                {
										
										Utilizador gestor = loadUser("gestor");
										char cod[80];
										strcpy(cod,"cat ");
										strcat(cod,a.cliente.username);
										strcat(cod,"-lC.txt | wc -l > f");
										system(cod);
										FILE *fx = fopen("f","r");
										fgets(cod,10,fx);
										fclose(fx);
										int c = atoi(cod);
										Produto lC[c];
										le_lista_compras(lC,a.cliente.username,c);
										a.cliente.lC = 0;
										for(int i=0;i<c;i++)
											a.cliente.lC += lC[i].preco * lC[i].qE;
                    if(op==0)
                        printf("A sua lista de compras encontra-se vazia.\n");
                    else if(a.cliente.saldo >= a.cliente.lC)
                    {
                        for(int i=0; i<c; i++)
                            pagar(a.cliente,lC[i]);
                    }
                    else
                        printf("O utilizador não possui saldo suficiente para efetuar esta operação.\n");
                    break;
                }
                default:
                    printf("ERRO: Operador %d desconhecido.\n",a.op);
                }
                int op = isEmptylC(a.cliente.username);
                printf("\n$ SUPCliente\n");
                printf("1) Ver Lista de Compras\n");
                printf("2) Adicionar Produto\n");
                printf("3) Remover Produto\n");
                printf("4) Pagar\n");
                printf("5) Voltar a trás\n");
                scanf(" %d",&a.op);
            }
            break;
        }
        case 3:
        {
            showStats(a.cliente);    // Mostra as estatisticas
            break;
        }
        case 4:  // Permite a alteração da Password
        {
            if(changePassword(a.cliente)!=0)
                printf("Ocorreu um erro ao alterar a sua password. Tente outra vez mais tarde.\n");
            else
            {
                printf("Password alterada com sucesso.\n");
                a.cliente = loadUser(a.cliente.username);
            }
            break;
        }
        default:
            printf("ERRO: Operador %d desconhecido.\n",a.op);
        }
        menu();
        scanf(" %d",&a.op);
    }
    Csave(a.cliente);
}
