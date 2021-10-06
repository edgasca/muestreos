/*Author: Eduardo de Jesús Gasca Laguna*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

//Matriz Original
int M=120;
int N=180;

//Tamaño de muestreo
int Rm=30;
int Cm=30;

void Memoria(float **matriz);
void llenarMatriz(float **matriz, int m, int n);
void obtenerMatriz(float **matriz);
void promedio(float **matriz, float data[][3], int nR);
void mayor(float **matriz, float data[][3], int nR);
void moda(float **matriz,float data[][3], int nR);
void liberarmemoria(float **matriz);

void comparador(float datosA[][3],float datosB[][3], float datosC[][3], float datComp[][3], int nR);
void tamtxt();
void leertxt(float **matriz, int Y);
void guardaEntxt(float **A1, float **B1, float **C1, float datosA[][3],float datosB[][3], float datosC[][3], float datComp[][3], int nR);

int main()
{
    //Dividir en regiones de 20.
    //Calcular por region. Promedio, Dato Mayo,Moda
    float **A, **B, **C;
    int nR=(M*N)/(Rm*Cm), S=0;
    int opcion;
    float datMatrizA[nR][3], datMatrizB[nR][3], datMatrizC[nR][3], datComparados[nR][3]; //[0]Promedio, [1]DatoMayor, [2]Moda
    
    printf("1. Generar datos aleatorios. \n");
    printf("2. Obtener datos desde los archivos de texto.\n");
    scanf("%d", &S);

    if(S==2)
    {
        system("clear");
        tamtxt(); //obtener tamaño de las matrices en el archivo para asignar memoria
    }
    
    //Asignando Memoria dinamica
        A = (float **)calloc(M,sizeof(float*));
        B = (float **)calloc(M,sizeof(float*));
        C = (float **)calloc(M,sizeof(float*));

        for(int i=0;i<M;i++)
        {
            A[i]=(float *)calloc(N,sizeof(float));
            B[i]=(float *)calloc(N,sizeof(float));
            C[i]=(float *)calloc(N,sizeof(float));
        }
    
    //llenado aleatorio
    if(S==1)
    {
        system("clear");
        printf("Generando datos aleatorios..........\n");
        llenarMatriz(A,M,N);
        llenarMatriz(B,M,N);
        llenarMatriz(C,M,N);
    }
    //llenado desde txt
    if(S==2)
    {   
        system("clear");
        printf("Obteniendo datos de los archivos de texto....... \n");
        //El numero indica el archivo de texto que se abre en esa matriz 1=matriz1.xt 2=matriz2.txt etc...
        leertxt(A, 1);
        leertxt(B, 2);
        leertxt(C, 3);
    }

    nR= (M*N)/(Rm*Cm);
    printf("Numero de regiones: %d \n", nR);
    //Resuelven los datos de interes
    
    promedio(A, datMatrizA, nR);
    promedio(B, datMatrizB, nR);
    promedio(C, datMatrizC, nR);
    
    mayor(A, datMatrizA, nR);
    mayor(B, datMatrizB, nR);
    mayor(C, datMatrizC, nR);

    moda(A,datMatrizA,nR);
    moda(B,datMatrizB,nR);
    moda(C,datMatrizC,nR);
    
    comparador(datMatrizA, datMatrizB,datMatrizC, datComparados, nR);

    //Guardando datos en archivo de texto
    printf("Datos cargados en \"estadisticos.txt\" \n");
    guardaEntxt(A, B, C, datMatrizA, datMatrizB, datMatrizC,datComparados, nR);

    //Liberando memoria
    liberarmemoria(A);
    liberarmemoria(B);
    liberarmemoria(C);
    return 0;
}

void llenarMatriz(float **matriz, int m, int n)
{
    srand((unsigned)time(NULL));
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            matriz[i][j]= (float)(rand()%1000/100.0);
}

void promedio(float **matriz, float data[][3], int nR)
{
    float promedio=0.0, array[(Cm*Rm)];
    int saltoN=0, saltoM=0, j=0, i=0, k=0;

    for (int region=0; region<nR;region++)
    {
        for (i=saltoM*Rm; i<(saltoM*Rm)+Rm; i++)
        {
            for(j=saltoN*Cm; j<((saltoN*Cm)+Cm); j++)
            {
                array[k]=matriz[i][j];
                k++;
            }
            
        }
        k=0;

        for(int pr=0; pr<(Cm*Rm); pr++)
        {
            promedio+=array[pr];
        }

        if(j<N)
        saltoN++;
        else if(j>=N)
        {
            saltoN=0;
            saltoM++;
        }
        
        //Promedio

        data[region][0]=promedio/(Cm*Rm);
        promedio=0;          
    } 
}

void mayor(float **matriz, float data[][3], int nR)
{
    int saltoN=0, saltoM=0, j=0, i=0, k=0;
    float Mayor=0, array[(Cm*Rm)], temp=0;

    for (int region=0; region<nR;region++)
    {
        for (i=saltoM*Rm; i<(saltoM*Rm)+Rm; i++)
        {
            for(j=saltoN*Cm; j<((saltoN*Cm)+Cm); j++)
            {
                array[k]=matriz[i][j];
                k++;
            }
            
        }
        k=0;

        //Ordenando arreglo de menor a mayor
        for(int a=0;a<((Cm*Rm)-1);a++)
            for(int l=(a+1); l<(Cm*Rm); l++)
            {
                if (array[a]>array[l])
                    {
                        temp=array[a];
                        array[a]=array[l];
                        array[l]=temp;
                    }
            }
    
        if(j<N)
        saltoN++;
        else if(j>=N)
        {
            saltoN=0;
            saltoM++;
        }
        
        //Pasando dato mayor
        Mayor=array[((Rm*Cm)-1)];
        data[region][1]=Mayor;     
    } 
}

void moda(float **matriz,float data[][3], int nR)
{
    int saltoN=0, saltoM=0, j=0, i=0, k=0, contador=0, frecuencia=0;
    float moda=0, array[(Cm*Rm)], temp=0;

    for (int region=0; region<nR;region++)
    {
        frecuencia=0;
        for (i=saltoM*Rm; i<(saltoM*Rm)+Rm; i++)
        {
            for(j=saltoN*Cm; j<((saltoN*Cm)+Cm); j++)
            {
                array[k]=matriz[i][j];
                k++;
            }
        }
        k=0;

        //Ordenando arreglo
        for(int a=0;a<((Cm*Rm)-1);a++)
            for(int l=(a+1); l<(Cm*Rm); l++)
            {
                if (array[a]>array[l])
                    {
                        temp=array[a];
                        array[a]=array[l];
                        array[l]=temp;
                    }
            }

        //Calculando frecuencias para saber la moda
        contador=1;
        for(int a=0;a<(Cm*Rm);a++)
        {
            for(int l=0; l<(Cm*Rm); l++)
            {
                if(array[a]==array[l])
                {
                    contador++;
                    
                if(contador>frecuencia)
                {
                frecuencia= contador;
                moda=array[l];
                }
                }
            }
            contador=1;
        }
            if((frecuencia-1)== 1)
            {
                moda=0.000;
            }

            data[region][2]=moda;
        
        if(j<N)
        saltoN++;
        else if(j>=N)
        {
            saltoN=0;
            saltoM++;
        }
    }
}

void comparador(float datosA[][3],float datosB[][3], float datosC[][3], float datComp[][3], int nR)
{
    float arreglo[3]={0,0,0}, temp=0;

    //dato -> Promedios[0], Mayor[1], Moda[2]
    for(int dato=0; dato<3; dato++)
    {
        for (int region=0; region<nR; region++)
            {
                arreglo[0]=datosA[region][dato];
                arreglo[1]=datosB[region][dato];
                arreglo[2]=datosC[region][dato];



            for(int a=0;a<3;a++)
            {
                for(int l=(a+1); l<3; l++)
                {
                    if (arreglo[a]>arreglo[l])
                    {
                        temp=arreglo[a];
                        arreglo[a]=arreglo[l];
                        arreglo[l]=temp;
                    }
                }
            }
        datComp[region][dato]=arreglo[2];
    }
    }
}

void guardaEntxt(float **A1, float **B1, float **C1, float datosA[][3],float datosB[][3], float datosC[][3], float datComp[][3], int nR)
{
    FILE *archivo;
    
    archivo = fopen("estadisticos.txt", "wt");

    //A
    fprintf(archivo,"Datos de las regiones A: \n");
    for(int x=0;x<nR;x++)
    {
        for(int y=0; y<3;y++)
        {
        fprintf(archivo,"%.2f ",datosA[x][y]);
        }
        fprintf(archivo,"\n");
    }
    fprintf(archivo,"Matriz A: \n");    
        for(int i=0;i<M;i++)
        {
        for(int j=0;j<N;j++)
            fprintf(archivo,"%.2f ", A1[i][j]);
            fprintf(archivo,"\n");
        }
    
    //B
    fprintf(archivo," \n\nDatos de las regiones de B: \n");
    for(int x=0;x<nR;x++)
    {
        for(int y=0; y<3;y++)
        {
        fprintf(archivo,"%.2f ",datosB[x][y]);
        }
        fprintf(archivo,"\n");
    }
    
    fprintf(archivo,"Matriz B: \n");    
        for(int i=0;i<M;i++)
        {
        for(int j=0;j<N;j++)
            fprintf(archivo,"%.2f ", B1[i][j]);
            fprintf(archivo,"\n");
        }
    
    //C
    fprintf(archivo,"\n\nDatos de las regiones de C: \n");
    for(int x=0;x<nR;x++)
    {
        for(int y=0; y<3;y++)
        {
        fprintf(archivo,"%.2f ",datosC[x][y]);
        }
        fprintf(archivo,"\n");
    }
    fprintf(archivo,"Matriz C: \n");    
        for(int i=0;i<M;i++)
        {
        for(int j=0;j<N;j++)
            fprintf(archivo,"%.2f ", C1[i][j]);
            fprintf(archivo,"\n");
        }
    
    //Matriz Resultante de la comparacion de datos
    fprintf(archivo,"\n\nMATRIZ CON LOS DATOS MAYORES DE CADA REGION. \n");
    for(int x=0;x<nR;x++)
    {
        for(int y=0; y<3;y++)
        {
        fprintf(archivo,"%.2f ",datComp[x][y]);
        }
        fprintf(archivo,"\n");
    }


    fclose(archivo);
}

void liberarmemoria(float **matriz)
{
    printf("Memoria liberada. \n");
    free(matriz);
    matriz=NULL;
}

void tamtxt(float **matriz)
{
    FILE *archivo;

    archivo = fopen("matriz1.txt", "rt");

    fscanf(archivo,"%d", &M);
    fscanf(archivo, "%d", &N);

    fclose(archivo);
}

void leertxt(float **matriz, int Y)
{
    FILE *archivo;
    
    if(Y==1)
    archivo = fopen("matriz1.txt", "rt");
    if(Y==2)
    archivo = fopen("matriz2.txt", "rt");
    if(Y==3)
    archivo = fopen("matriz3.txt", "rt");
    if(Y==4)
    archivo = fopen("matriz3.txt", "rt");

    fscanf(archivo,"%d", &M);
    fscanf(archivo, "%d", &N);

    for(int i=0;i<M; i++)
        for (int j= 0;j<N; j++)
        {
            fscanf(archivo,"%f", &matriz[i][j]);
        }
    fclose(archivo);
}