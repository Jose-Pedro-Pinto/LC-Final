#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char username[20];
  char password[18];
  char nome[40];
  int contacto;
  float saldo;
  float lC;
  int comprado;
  float gasto;
} Utilizador;

typedef struct {
  int codigo;
  char nome[30]; 	
  char descricao[100];
  int qE;		//quantidade do produto existente atualmente
  float preco; 		//preço de venda ao público em €
  float custo; 		//custo de aquisição em €   
  int qV;		//quantidade total de produto vendido até agora
  float TotalFeito;	//A soma de todo o dinheiro feito com as vendas deste produto
  float TotalGasto;     //A soma de todo o dinheiro gasto a comprar este produto
} Produto;

typedef struct {
  int u;        //Nº de utilizadore
  int p;	//Nº de produtos
  int t;	//Total de produtos vendidos
  float g;	//Total gasto a comprar produtos
  float f;	//Lucro total efetuado ate ao momento
} stats;

typedef struct {
  int op;
  Utilizador cliente;
  stats dados;
} bool;

int Uexiste(char u[]){
  char cod[80];
  strcpy(cod,"grep -w ");strcat(cod,u);strcat(cod," user.txt | wc -l > f");
  system(cod);
  char p[10];
  FILE *fx = fopen("f","r");
  fgets(p,10,fx);
  fclose(fx);
  system("rm -f f");
  return atoi(p);	
}

void itoa(int a, char b[]){
  int tamanho=0,temp = a,k=temp;
  while(temp/10 !=0) {tamanho++;temp/=10;};
  temp = k;
  for(int i=0; temp!=0; i++){
    k/=10;
    temp -= k*10;
    b[i] = '0' + temp;
    temp/=10;
    k = temp;
  }
  b[tamanho+1]='\0';
}

stats loadStats(bool a){
	char d[10],cod[60];
	
	FILE *fs = fopen("gestor-stats.txt","r");
	fgets(cod,60,fs);
	fclose(fs);
	
	sscanf(cod,"%d;%d;%d;%f;%f\n",&a.dados.u,&a.dados.p,&a.dados.t,&a.dados.g,&a.dados.f);
	return a.dados;
}

Utilizador loadUser(bool a){
  char d[120],p[15];int i,j;
  
  //Copiar para "f" a linha com os dados do gestor
  system("grep -w gestor user.txt > f");
  
  //Ler essa linha
  FILE *fx = fopen("f","r");
  fgets(d,120,fx);
  fclose(fx);
  
  //A linha lida é do género: username;password;nome;contacto;saldo\n
  for(i=0;d[i]!=';';i++) a.cliente.username[i] = d[i];
  a.cliente.username[i]='\0'; i++;
  for(j=0;d[i]!=';';i++) a.cliente.password[j++] = d[i];
  a.cliente.password[j]='\0';i++;
  for(j=0;d[i]!=';';i++) a.cliente.nome[j++] = d[i];
  a.cliente.nome[j] = '\0'; i++;
  for(j=0;d[i]!=';';i++) p[j++] = d[i];
  p[j] = '\0'; i++; a.cliente.contacto = atoi(p);
  for(j=0;d[i]!=';';i++) p[j++] = d[i];
  p[j]='\0'; i++; a.cliente.saldo = atof(p);
  for(j=0;d[i]!=';';i++) p[j++] = d[i];
  p[j]='\0';i++; a.cliente.comprado = atoi(p);
  for(j=0;d[i]!='\n';i++) p[j++] = d[i];
  p[j]='\0';i++; a.cliente.gasto = atof(p);
  system("rm -f f");
  
  return a.cliente;
}

Produto loadProduto(char data[]){
  Produto choice;
  char d[120];
  strcpy(d,"grep -w ");strcat(d,data);strcat(d," product.txt > f");
  system(d);
  FILE *fp = fopen("f","r");
  fgets(d,120,fp);
  fclose(fp);
  system("rm -f f");
  
  int i,j;
  for(i=0; d[i]!=';';i++)	data[i]=d[i];
  data[i]='\0';i++;choice.codigo = atoi(data);
  for(j=0;d[i]!=';';i++) choice.nome[j++] = d[i];
  choice.nome[j] = '\0'; i++;
  for(j=0;d[i]!=';';i++) choice.descricao[j++] = d[i];
  choice.descricao[j] = '\0'; i++;
  for(j=0; d[i]!=';';i++)	data[j++]=d[i];
  data[j]='\0';i++;choice.qE = atoi(data);
  for(j=0; d[i]!=';';i++)	data[j++]=d[i];
  data[j]='\0';i++;choice.preco = atof(data);
  for(j=0; d[i]!=';';i++)	data[j++]=d[i];
  data[j]='\0';i++;choice.custo = atof(data);
  for(j=0; d[i]!=';';i++)	data[j++]=d[i];
  data[j]='\0';i++;choice.qV = atoi(data);
  for(j=0; d[i]!=';';i++)	data[j++]=d[i];
  data[j]='\0';i++;choice.TotalFeito = atof(data);
  for(j=0; d[i]!='\n';i++)	data[j++]=d[i];
  data[j]='\0';i++;choice.TotalGasto = atof(data); 
  
  return choice;
}

bool login() {
  bool a;
  char u[20],p[18];
  printf("** LOGIN **\n");
  printf("Username: ");
  scanf("%s",u);
  printf("Password: ");
  scanf(" %s",p);
  
  //Apenas a conta "gestor" pode aceder a este programa logo testa-se o username inserido para ver se é ou não essa conta
  if(strcmp(u,"gestor")!=0){
    printf("1 -> %s | %s\n","gestor",u);
    a.op = 1;
    return a;
  }
  
  a.cliente = loadUser(a);
  
  //Comparar password do ficheiro com a lida do stdin
  long int c = strlen(a.cliente.password);
  if(strlen(p) != c){
    a.op = 2;
    return a;
  }
  if(strncmp(p,a.cliente.password,c)!=0){
    a.op = 3;
    return a;
  }
  
  a.dados = loadStats(a);
  a.op = 0;
  return a;
}

void menu(){
  printf("\n$ SUPServidor\n");
  printf("** MENU **\n");
  printf("1) Novo Utilizador\n");
  printf("2) Gerir Stocks\n");
  printf("3) Ver estatísticas\n");
	printf("4) Alterar password\n");
	printf("5) Investir na empresa\n");
  printf("6) Logout\n");
}

void saveStats(bool a){
	char cod[60];
	strcpy(cod,a.cliente.username);strcat(cod,"-stats.txt");
	FILE *fs = fopen(cod,"w");
	fprintf(fs,"%d;%d;%d;%.2f;%.2f\n",a.dados.u,a.dados.p,a.dados.t,a.dados.g,a.dados.f);
	fclose(fs);
}

void Csave(Utilizador g){
	char cod[80];
	strcpy(cod,"grep -vw ");strcat(cod,g.username);strcat(cod," user.txt > f");
	system(cod);
	FILE *fx = fopen("f","a");
	fprintf(fx,"%s;%s;%s;%d;%.2f;%d;%.2f\n",g.username,g.password,g.nome,g.contacto,g.saldo,g.comprado,g.gasto);
	fclose(fx);
	system("sort f > user.txt");
	system("rm -f f");	
}

void Psave(Produto p){
	char d[5],cod[80];
	strcpy(cod,"grep -vw '");strcat(cod,p.descricao);strcat(cod,"' product.txt > f");
	system(cod);
  FILE *fp = fopen("f","a");
  fprintf(fp,"%d;%s;%s;%d;%.2f;%.2f;%d;%.2f;%.2f\n",p.codigo,p.nome,p.descricao,p.qE,p.preco,p.custo,p.qV,p.TotalFeito,p.TotalGasto);
  fclose(fp);
	system("sort f > product.txt");
	system("rm -f f");
}

int newUser(bool a){
	Utilizador new;
  printf("\n$ SUPServidor\n");
  printf("** NOVO UTILIZADOR **\n");
  printf("Nome: ");
	char c = getchar(); int i;
  for(i=0; i<40 && new.nome[i-1]!='\n'; i++) new.nome[i]=getchar();
	new.nome[i-1] = '\0';
  printf("Contacto: "); scanf(" %d",&new.contacto);
  printf("Username: "); scanf(" %s",new.username);
  printf("Password: "); scanf(" %s",new.password);
  new.saldo = 0;
	new.comprado = 0;
	new.gasto = 0;
  if(Uexiste(new.username)!=0)
    return 1;
	else{
		char cod[80];
		strcpy(cod,"touch ");strcat(cod,new.username);strcat(cod,"-lC.txt");
		system(cod);
		strcpy(cod,new.username);strcat(cod,"-stats.txt");
		FILE *fp = fopen(cod,"w");fprintf(fp,"0;0.0\n");fclose(fp);
		a.dados.u++;
		Csave(new);
		saveStats(a);
  	return 0;
	}
}

void Gstocks(){
  printf("$ SUPServidor\n");
  printf("1) Novo produto\n");
  printf("2) Alterar produto existente\n");
  printf("3) Remover produto\n");
  printf("4) Voltar a trás\n");
}

int codigo(){
  char d[10];
  system("tail -n1 product.txt | cut -d';' -f1 > f");
  FILE *f = fopen("f","r");
  fgets(d,10,f);
  fclose(f);
  system("rm -f f");
  return atoi(d)+1;	
}

int newProduto(bool a){
  Produto new;
	char c;int i;
	
  printf("\n$ SUPServidor\n");
  printf("** NOVO PRODUTO **\n");
  new.codigo = codigo();
  printf("Codigo: %d\n",new.codigo);
  printf("Nome: "); scanf(" %s",new.nome);
  printf("Descrição: "); scanf("%c",&c);
  for(i=0; i<100 && new.descricao[i-1]!='\n'; i++) new.descricao[i] = getchar();
	new.descricao[i-1] = '\0';
  printf("Custo de aquisição: "); scanf(" %f",&new.custo);
  printf("Preço de Venda ao Público: "); scanf(" %f",&new.preco);
  printf("Quantidade inicial: "); scanf(" %d",&new.qE);
  new.qV = 0;
  new.TotalFeito = 0;
  new.TotalGasto = new.qE*new.custo;
	
	if(a.cliente.saldo - new.qE*new.custo <=0){
		printf("\nA empresa não tem fundos suficientes para pagar por este produto.\n");
		return 1;
	}
	a.cliente.saldo -= new.qE*new.custo;
  FILE *fp = fopen("product.txt","a");
  fprintf(fp,"%d;%s;%s;%d;%.2f;%.2f;%d;%.2f;%.2f\n",new.codigo,new.nome,new.descricao,new.qE,new.preco,new.custo,new.qV,new.TotalFeito,new.TotalGasto);
  fclose(fp);
	system("sort product.txt > f");
	system("cat f > product.txt");
	system("rm -f f");
	a.dados.p++;
	saveStats(a);
	Csave(a.cliente);
  return 0;
}

void le_base_dados(Produto DB[], int c){
	char cod[250],k[15];int n=0,i,j;
	FILE *fp = fopen("product.txt","r");
	while(n<c){
		fgets(cod,250,fp);
		for(i=0; cod[i]!=';';i++)	k[i]=cod[i];
		k[i]='\0';i++;DB[n].codigo = atoi(k);
		for(j=0;cod[i]!=';';i++) DB[n].nome[j++] = cod[i];
		DB[n].nome[j] = '\0'; i++;
		for(j=0;cod[i]!=';';i++) DB[n].descricao[j++] = cod[i];
		DB[n].descricao[j] = '\0'; i++;
		for(j=0; cod[i]!=';';i++)	k[j++]=cod[i];
		k[j]='\0';i++;DB[n].qE = atoi(k);
		for(j=0; cod[i]!=';';i++)	k[j++]=cod[i];
		k[j]='\0';i++;DB[n].preco = atof(k);
		for(j=0; cod[i]!=';';i++)	k[j++]=cod[i];
		k[j]='\0';i++;DB[n].custo = atof(k);
		for(j=0; cod[i]!=';';i++)	k[j++]=cod[i];
		k[j]='\0';i++;DB[n].qV = atoi(k);
		for(j=0; cod[i]!=';';i++)	k[j++]=cod[i];
		k[j]='\0';i++;DB[n].TotalFeito = atof(k);
		for(j=0; cod[i]!='\n';i++)	k[j++]=cod[i];
		k[j]='\0';i++;DB[n].TotalGasto = atof(k);
		n++;
	}
	fclose(fp);
}

int Pexiste(char d[]){
  char data[60];
  strcpy(data,"grep -w ");strcat(data,d);strcat(data," product.txt | wc -l > f");
  system(data);
	FILE *fp = fopen("f","r");
  fgets(data,3,fp);
  fclose(fp);
  system("rm -f f");
  return atoi(data);
}

int addStock(Produto p,bool a){
  printf("** ADICIONAR STOCK **\n");
  printf("Quantidade atual: %d\n",p.qE);
  int add;
  printf("Quantidade a adicionar: ");
  scanf(" %d",&add);
  if(add<=0){
		printf("Quantidade imprópria a adicionar.\n");
    return 1;
	}
	if(a.cliente.saldo - p.custo*add < 0){
		printf("Não tem fundos suficientes para efetuar esta operação.\n");
		return 2;
	}
	a.cliente.saldo -= p.custo*add;
	p.qE += add;
	p.TotalGasto += p.custo*add;
	a.dados.g += p.custo*add;
	Csave(a.cliente);
	Psave(p);
	saveStats(a);
  return 0;
}

int gPVP(Produto p){
  printf("** ALTERAR PVP **\n");
  printf("PVP atual: %.2f€\n",p.preco);
  float new;
  printf("Novo PVP: ");
  scanf(" %f",&new);
  if(new <= p.custo)
    return 1;
  p.preco = new;
	Psave(p);
  return 0;
}

bool altproduto(bool a){
	char data[60],d[120];
	a.dados = loadStats(a);
	if(a.dados.p == 0){
		printf("De momento não existem produtos na base de dados.\n");
		a.op = 3;
		return a;
	}
	
	//Imprimir todos os produtos
	printf(".----------------------------------------------------------------------.\n");
	printf("|%9s|%30s|%10s|%9s|%9s|\n","Codigo","Nome","Quantidade","PVP","Custo");
	printf(".----------------------------------------------------------------------.\n");
	Produto DB[a.dados.p];
	le_base_dados(DB,a.dados.p);
	for(int i=0; i<a.dados.p; i++)
		printf("|%9d|%30s|%10d|%9.2f|%9.2f|\n",DB[i].codigo,DB[i].nome,DB[i].qE,DB[i].preco,DB[i].custo);
	printf(".----------------------------------------------------------------------.\n");
	
	scanf(" %s",d);
	int i,j=-1;
	for(i=0;i<a.dados.p && j==-1;i++)
		if(DB[i].codigo==atoi(d))
			j=i;
	
	if(j==-1){
		printf("Não existe nenhum produto com esse codigo.\n");
		a.op = -2;
		return a;
	}
	
	printf("\n$ SUPServidor\n");
	printf("** ALTERAR DADOS PRODUTO **\n");
	printf("1) Adicionar stock\n");
	printf("2) Alterar PVP\n");
	printf("3) Voltar a trás\n");
	scanf(" %d",&a.op);
	
	if(a.op==1){
		if(addStock(DB[j],a)==0){
			printf("Stock adicionado com sucesso.\n");
			a.cliente = loadUser(a);
			a.dados = loadStats(a);
		}
	}
	else if(a.op ==2){
		if(gPVP(DB[j])!=0)
			printf("O preço de um produto não pode ser menor ou igual ao custo desse mesmo produto.\n");
		else
			printf("PVP alterado com sucesso.\n");
	}
	else
		return a;	
}

int remProduto(bool a){
	char d,l[10];
	a.dados = loadStats(a);
	if(a.dados.p == 0){
		printf("De momento não existem produtos na base de dados.\n");
		return 2;
	}
	
	//Imprimir todos os produtos
	printf(".----------------------------------------------------------------------.\n");
	printf("|%9s|%30s|%10s|%9s|%9s|\n","Codigo","Nome","Quantidade","PVP","Custo");
	printf(".----------------------------------------------------------------------.\n");
	Produto DB[a.dados.p];
	le_base_dados(DB,a.dados.p);
	for(int i=0; i<a.dados.p; i++)
		printf("|%9d|%30s|%10d|%9.2f|%9.2f|\n",DB[i].codigo,DB[i].nome,DB[i].qE,DB[i].preco,DB[i].custo);
	printf(".----------------------------------------------------------------------.\n");
	
	scanf(" %s",l);
	int i,j=-1;
	for(i=0;i<a.dados.p && j==-1;i++)
		if(DB[i].codigo == atoi(l))
			j=i;
	
  if(j==-1)
    return 1;
	
  printf("Tem mesmo a certeza que pretende remover %s?\n",l);
	printf("y n\n");
  scanf(" %c",&d);
  if(d=='n')
    return 2;
  else if(d=='y'){
    char data[50];
		strcpy(data,"grep -vw ");strcat(data,l);strcat(data," product.txt > f");
		system(data);
		system("sort f > product.txt");
		system("rm -f f");
		printf("%d\n",a.dados.p);
		a.dados.p--;
		printf("%d\n",a.dados.p);
		Csave(a.cliente);
		saveStats(a);
    return 0;
  }
	return -1;
}

int showStats(bool a){
	char cod[100],data[60],c[10];
	int i,j,op;

	printf("\n$ SUPServidor\n");
	printf("** ESTATÌSTICAS **\n");
	printf("1) Gerais\n");
	printf("2) Produto\n");
	printf("3) Cliente\n");
	printf("4) Voltar a trás\n");
	scanf(" %d",&op);
	
	a.dados = loadStats(a);
	a.cliente = loadUser(a);
	
	if(op==1){
		printf("\n** ESTATISTICAS GERAIS **\n");
		printf("Nº de utilizadores: %d\n",a.dados.u);
		printf("Nº de produtos: %d\n",a.dados.p);
		printf("Total de produtos vendidos: %d\n",a.dados.t);
		printf("Total gasto a comprar produtos: %.2f €\n",a.dados.g);
		printf("Total feito a vender produtos: %.2f €\n",a.dados.f);
		printf("Lucro total efetuado ate ao momento: %.2f €\n",a.dados.f-a.dados.g);
		printf("Montante disponível para investimentos: %.2f €\n",a.cliente.saldo);
		return 0;
	}
	else if(op==2){
		if(a.dados.p == 0){
			printf("De momento não existem produtos na base de dados.\n");
			return 4;
		}
		
		//Imprimir todos os produtos
		printf(".----------------------------------------------------------------------.\n");
		printf("|%9s|%30s|%10s|%9s|%9s|\n","Codigo","Nome","Quantidade","PVP","Custo");
		printf(".----------------------------------------------------------------------.\n");
		Produto DB[a.dados.p];
		le_base_dados(DB,a.dados.p);
		for(int i=0; i<a.dados.p; i++)
			printf("|%9d|%30s|%10d|%9.2f|%9.2f|\n",DB[i].codigo,DB[i].nome,DB[i].qE,DB[i].preco,DB[i].custo);
		printf(".----------------------------------------------------------------------.\n");
	
		scanf(" %s",c);
		int i,j=-1;
		for(i=0;i<a.dados.p && j==-1;i++)
			if(DB[i].codigo == atoi(c))
				j=i;
	
  	if(j==-1)
    	return 1;		
		
		printf("\n** ESTATÌSTICAS **\n");
		printf("Codigo: %d\n",DB[j].codigo);
		printf("Nome: %s\n",DB[j].nome);
		printf("Quantidade existente: %d\n",DB[j].qE);
		printf("Quantidade vendida ate ao momento: %d\n",DB[j].qV);
		printf("Lucro atual: %.2f €\n",DB[j].preco-DB[j].custo);
		printf("Total gasto neste produto: %.2f €\n",DB[j].TotalGasto);
		printf("Total feito com este produto: %.2f €\n",DB[j].TotalFeito);
		printf("Total lucrado com este produto: %.2f €\n",DB[j].TotalFeito - DB[j].TotalGasto);
		return 0;
	}
	else if(op==3){
		if(a.dados.u==0){
			printf("De momento não existem utilizadores na base de dados.\n");
			return 4;
		}
		char u[20];int p;float f;
		
		//Imprime a lista de todos os utilizadores
		printf("Utilizadores:\n");
		system("grep -vw gestor user.txt | cut -d';' -f1");
		
		//Lê o username do utilizador escolhido
		scanf(" %s",data);
		if(Uexiste(data)==0)
			return -1;
		
		strcpy(cod,"grep -w ");strcat(cod,data);strcat(cod," user.txt > f");
		system(cod);
		
		Utilizador g;
		FILE *fx = fopen("f","r");
		fscanf(fx,"%s;%s;%s;%d;%f\n",g.username,g.password,g.nome,&g.contacto,&g.saldo);
		fclose(fx);
		
		strcat(data,"-stats.txt");
		
		fx = fopen(data,"r");
		fgets(u,20,fx);
		
		for(i=0; u[i]!=';';i++) data[i] = u[i];
		data[i]='\0'; i++; p = atoi(data);
		
		for(j=0; u[i]!='\n';i++) data[j++] = u[i];
		data[j] = '\0'; i++; f = atof(data);
		
		
		printf("\n** ESTATISTICAS **\n");
		printf("Nome: %s\n",g.nome);
		printf("Contacto: %d\n",g.contacto);
		printf("Saldo: %.2f €\n",g.saldo);
		printf("Nº total de produtos comprados: %d\n",p);
		printf("Montante total gasto: %.2f €\n",f);
		
		if(p==0)
			fclose(fx);
		else{
			for(i=0; i<p; i++){
				char nome[30]; int comprado; float gasto;
				fgets(cod,100,fx);
				sscanf(cod,"%s;%d;%f\n",nome,&comprado,&gasto);
				printf("\nNome: %s\nTotal comprado: %d\nTotal gasto: %.2f €\n",nome,comprado,gasto);
			}
			fclose(fx);
		}
		return 0;
	}
	else
		return 4;
}

int changePassword(Utilizador g){
	char p1[20],p2[20],p3[20],data[55];
  printf("** ALTERAR PASSWORD **\n");
  printf("Password atual: ");
  scanf("%s",p1);
  printf("Nova password: ");
  scanf("%s",p2);
  printf("Confirme a nova password: ");
  scanf("%s",p3);
	long int c = strlen(g.password);
  if(strlen(p1) != c || strncmp(p1,g.password,c))
    return 1;
	c = strlen(p2);
  if(strlen(p3) != c || strncmp(p2,p3,c))
    return 2;
	strcpy(g.password,p2);
	Csave(g);
  return 0;
}

int addSaldo(bool a){
	float invest;
	a.cliente = loadUser(a);
	printf("** INVESTIR **\n");
	printf("A empresa tem %.2f € disponíveis.\n",a.cliente.saldo);
	printf("Quanto pretende investir?\n");scanf(" %f",&invest);
	if(invest<=0){
		printf("Montante impróprio a investir.\n");
		return 1;
	}
	a.cliente.saldo += invest;
	Csave(a.cliente);
	return 0;
}

void main(){
  system("clear");
	bool b,a = login();
  while(a.op!=0){
    printf("Username ou password errados. Tente outra vez.\n\n");
		a = login();
  }
  printf("\nBem vindo de volta %s!\n",a.cliente.nome);
	menu();
	scanf(" %d",&a.op);		
  while(a.op!=6){
    switch(a.op){
    case 1:{
			a.op = newUser(a);
			if(a.op==0){
				printf("Utilizador criado com sucesso.\n");
				a.dados = loadStats(a);	
			}
			else
				printf("Ocorreu um erro. Tente outra vez mais tarde.\n");
			break;
		}
    case 2:{
      Gstocks();
	  	scanf(" %d",&a.op);		
    while(a.op!=4){
	switch(a.op){
	case 1:{
		if(newProduto(a)==0){
			printf("Produto criado com sucesso.\n");
			a.dados = loadStats(a);
			a.cliente = loadUser(a);
		}
		break;
	}
	case 2:{
		b = altproduto(a);
		a.cliente = loadUser(b);
		while(b.op!=3){
			b = altproduto(a);
			a.cliente = loadUser(b);
		}
		break;
	}
	case 3:{
		a.op = remProduto(a); 
	  if(a.op==1)
	    printf("O produto que pretende remover não existe.\n");
	  else if(a.op==-1)
	    printf("Um erro ocorreu ao remover o produto. Tente outra vez mais tarde.\n");
	  else if(a.op==0){
	    printf("Produto removido com sucesso.\n");
			a.dados = loadStats(a);
			printf("%d\n",a.dados.p);
		}
	  break;
	}
	default: printf("ERRO: Operador %d desconhecido.\n",a.op);
	}
	Gstocks();
  	scanf(" %d",&a.op);		
      }
      break;
    }
    case 3:{while(showStats(a)!=4);break;}
    case 4:{
			if(changePassword(a.cliente)!=0)
				printf("Ocorreu um erro ao alterar a sua password. Tente outra vez mais tarde.\n");
			else{
				printf("Password alterada com sucesso.\n");
				a.cliente = loadUser(a);
			}
			break;
		}
		case 5:{
			if(addSaldo(a)==0){
				printf("Investimento realizado com sucesso.\n");
				a.cliente = loadUser(a);
			}
			break;
	 }
    default: printf("ERRO: Operador %d desconhecido.\n",a.op);
    }
		menu();
  	scanf(" %d",&a.op);		
  }
	Csave(a.cliente);
	saveStats(a);
}
