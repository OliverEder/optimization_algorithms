
/* 
 * File:   main.cpp
 * Author: Oliver Espinosa Meneses
 *
 * Created on 4 de junio de 2018, 10:02 PM
 */

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <list>
#include <vector>
#include <ctime>
using namespace std;

int **puntero_matriz;
int lineas_cadena;
int algoritmo_seleccionado;
int nodo_s;
int nodo_g;
string grafo_matriz[50][50];
string nodo_inicial;
string nodo_meta;
float gmatriz_float[50][50];

//Funcion para cargar el archivo con el grafo.
void cargar_grafo1()
{
    DIR * directorio;
    ifstream archivo;
    string contenido="";
    string contenido_matriz="";
    string linea;
    string fichero="grafos/";
    string ruta="";
    string grafo="";
    string grafos[70];
    int apuntador=0;
    int ndx=0;
    int nlinea=0;
    struct dirent * elemento;
    printf("Cargar grafo\n\r");
    string dir="grafos";
    
    //se abre el directorio y se selecciona el archivo.
    lineas_cadena=0;
    if (directorio = opendir(dir.c_str()))
    { 
      while (elemento = readdir(directorio))
      {
         grafos[apuntador]=elemento->d_name;
          if (grafos[apuntador] != "." && grafos[apuntador] != "..") 
          {
              cout << grafos[apuntador] << endl;
                        
          }
          apuntador++;
          
      }
      printf("¿Que grafo deseas abrir?");
      std::cin >> grafo;
    }
    closedir(directorio);    
    ruta=fichero+grafo;    
    archivo.open(ruta.c_str(),ios::in);
    if(archivo.fail())
    {
        cout<<"No se puede leer el archivo"<<endl;
    }
    else
    {
        //se obtienen las lineas del archivo.    1
        while(!archivo.eof())
        {
            nlinea++;
            getline(archivo,linea);
            cout<<"linea:"<<nlinea<<endl;
            contenido=contenido+linea+"\n";
            contenido_matriz=contenido_matriz+linea+",";
            lineas_cadena=lineas_cadena+1;
        }
        lineas_cadena=lineas_cadena-2;
        size_t tam= contenido.length();
        contenido_matriz.erase(tam-2);
        puntero_matriz= new int*[lineas_cadena];
        for(int i=0;i<lineas_cadena;i++)
        {
            puntero_matriz[i]= new int[lineas_cadena];

        }
        nlinea=0;
        for(int i=0;i<lineas_cadena;i++)
        {
            nlinea++;
            for(int j=0;j<lineas_cadena;j++)
            {
                ndx=contenido_matriz.find(",");            
                istringstream(contenido_matriz.substr(0,ndx))>>*(*(puntero_matriz+i)+j);
                contenido_matriz.erase(0,ndx+1);
            }
            cout<<"linea:"<<nlinea<<endl;
        }
        
    }
    
  
}

void seleccionar_nodos1()
{
    
    cout<<"Seleccionar los nodos"<<endl;
    int nodos[lineas_cadena];
    cout<<"Nodos del grafo: ";
    for(int i=0;i<lineas_cadena;i++)
    {   
        nodos[i]=i+1;
        cout<<nodos[i]<<",";
    }
    cout<<endl;
    cout<<"Seleccione un nodo inicial: ";
    cin>>nodo_s;
    cout<<"Seleccione un nodo meta: ";
    cin>>nodo_g;
        
}

void seleccionar_nodos()
{
    cout<<"Seleccionar los nodos"<<endl;
    string nodos[lineas_cadena-1];
    cout<<"Nodos del grafo: ";
    for(int i=1;i<lineas_cadena;i++)
    {
        nodos[i-1]=grafo_matriz[0][i];
        cout<<nodos[i-1]<<",";
    }
    cout<<endl;
    cout<<"Seleccione un nodo inicial: ";
    cin>>nodo_inicial;
    cout<<"Seleccione un nodo meta: ";
    cin>>nodo_meta;
    
    
}

void seleccionar_algoritmo()
{
    algoritmo_seleccionado=0;    
    cout<<"Seleccionar algoritmo"<<endl;
    cout<<"1) Branch and Bound (clasico)"<<endl;
    cout<<"2) AWNN Explicitada"<<endl;
    cout<<"3) SAPCNNVM"<<endl;
    cout<<"4) SAPCNN"<<endl;
    cout<<"5) AWNN"<<endl;    
    cout<<"6) Salir"<<endl;
    cout<<"Seleccionar un algoritmo: ";
    cin>>algoritmo_seleccionado;
    cout<<endl;
    if(algoritmo_seleccionado==6)
    {
        algoritmo_seleccionado=0;
    }
    
}
//FIFO
void BranchAndBound()
{
    //declaracion de variables y estructuras
    int nivel=0;
    int longitud;
    int si_dospuntos;
    int si_guion;
    int indice=0;
    int subindice;
    float costo=0.0;
    string estadoactual;
    string nodoactual;
    string estadoencurso;
    string snivel;
    string scosto;
    unsigned t0, t1;
    list <string> agenda;
    list <string> historico;
    list <string> nodos_visitados;
    list <string> nodos_adyasentes;
    list <string> estados_meta;
    //creacion del primer estado.
    nodoactual=nodo_inicial;
    historico.push_back("0:"+nodoactual+":0");
    agenda.push_back("0:"+nodoactual+":0");
    
    list<string>::iterator iterador_historico;
    t0=clock();
    for(iterador_historico=historico.begin();iterador_historico!=historico.end();iterador_historico++)
    {
        
        estadoactual=*iterador_historico;
        estadoencurso=estadoactual;
        longitud=estadoencurso.size();
        //se extrae la parte de la cadena correspondiente al nivel
        si_dospuntos=estadoencurso.find_last_of(":");
        snivel=estadoencurso.substr(si_dospuntos+1,longitud);
        nivel=atoi(snivel.c_str());
        estadoencurso.erase(si_dospuntos,longitud);
        //se extrae la parte de la cadena correspondiente al costo
        si_dospuntos=estadoencurso.find(":");
        scosto=estadoencurso.substr(0,si_dospuntos);
        costo=atof(scosto.c_str());
        estadoencurso.erase(0,si_dospuntos+1);
        //se extrae la parte de la cadena correspondiente al ultimo nodo o nodo en curso
        si_guion=estadoencurso.find_last_of("-");
        if(si_guion!=-1)
        {
            longitud=estadoencurso.size();
            nodoactual.clear();
            nodoactual=estadoencurso.substr(si_guion+1,longitud);
        }
        else{
            nodoactual=estadoencurso;
        }
        //cout<<"nodo actual:"<<nodoactual<<endl;
        nodos_visitados.push_back(nodoactual);
        if(nodoactual==nodo_meta)
        {
            //se verifica si el nodo actual es igual al nodo meta.
            //cout<<"Estado: "<<estadoactual<<" se llego a la meta"<<endl;
            estados_meta.push_back(estadoactual);
            agenda.pop_front();
            //nivel=30;
        }
        else{
            nivel++;//se incrementa el nivel
            //se consigue el indice del nodo en curso.
            for(int i=0;i<lineas_cadena;i++)
            {
                if(grafo_matriz[i][0]==nodoactual)
                {
                    indice=i;
                    
                }
            }
            nodos_adyasentes.clear();
            //se buscan los nodos adyasentes del nodo indice, ademas que no sean nodos ya visitados.
            for(int i=1;i<lineas_cadena;i++)
            {
                
                if(grafo_matriz[indice][i]!="0")
                {
                    int apariciones=0;
                    list<string>::iterator iterador_visitados;
                    for(iterador_visitados=nodos_visitados.begin();iterador_visitados!=nodos_visitados.end();iterador_visitados++)
                    {
                        if(grafo_matriz[0][i]== *iterador_visitados )
                        {
                            apariciones++;
                            //cout<<grafo_matriz[0][i]<<"="<<*iterador_visitados<<endl;
                        }
                    }
                    
                    if(apariciones==0)
                    {
                        stringstream nodo_ad;
                        nodo_ad << i;
                        nodos_adyasentes.push_back(grafo_matriz[indice][i]+"-"+nodo_ad.str());
                    }    
                }    
            }
            nodos_visitados.unique();
            if(nodos_adyasentes.size()>0)
            {
                nodos_adyasentes.sort();//se ordenan los nodos adyasentes
            
                list<string>::iterator iterador_adyasentes;
                for(iterador_adyasentes = nodos_adyasentes.begin();iterador_adyasentes!=nodos_adyasentes.end();iterador_adyasentes++)
                {
                    string cadena;
                    string estado_agenda;
                    string estado;
                    string historico_nodos;
                    string str_costo;
                    stringstream c_costo_historico;
                    stringstream snivel;
                    float costof;
                    float costo_historico;
                    int ndx=0; 
                    snivel<<nivel;
                    list<string>::iterator itera;//se crea un elemento iterativo
                    cadena=*iterador_adyasentes;//se asigna el valor del vector en ese elemento.
                    ndx=cadena.find("-");//se busca la posicicion del caracter en la cadena
                    str_costo=cadena.substr(0,ndx);//se copia el valor de la cadena de la posicion 0 a la posicion ndx
                    cadena.erase(0,ndx+1);//se borra el principio de la cadena hasta el caracter
                    istringstream(cadena)>>subindice;//se asigna el contenido restante a un valor int subindice
                    costof=gmatriz_float[indice][subindice];
                    estado_agenda=*iterador_historico;
                    ndx=estado_agenda.find(":");
                    estado_agenda.erase(0,ndx+1);
                    ndx=estado_agenda.find_last_of(":");//encuentra la ultima posicion donde se  encuentra :
                    historico_nodos=estado_agenda.substr(0,ndx);//se copia el contenido de la cadena desce el inicio hasta la posicion ndx
                    estado_agenda.erase(0,ndx+1);//se borra desde el inicio hasga ndx+1
                    costo_historico=atoi(estado_agenda.c_str());//se combierte la cadena a float
                    costo_historico=costo_historico+costof;//se calcula el nuevo costo
                    c_costo_historico<<costo_historico;//se combierte el costo a string
                    historico_nodos=historico_nodos+"-"+grafo_matriz[0][subindice];//se genera la nueva cadena de nodos
                    estado=c_costo_historico.str()+":"+historico_nodos+":"+snivel.str();//se forma el estado con la cadena de nodos y el costo.
                    historico.push_back(estado);
                    agenda.push_back(estado);
                }
                agenda.pop_front();
            }
            else{
                //cout<<"No se agrego ningun estado a la agenda."<<endl;
                agenda.pop_front();
            }
            
        }
    }
    t1 = clock();
    
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    
    cout<<"Resultados"<<endl;
    cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
    cout<<"Nivel maximo:"<<nivel<<endl;
    list<string>::iterator iterador_meta;
    if(!estados_meta.empty())
    {
        estados_meta.sort();
        cout<<"Numero de estados donde se encontro la meta: "<<estados_meta.size()<<endl;
        cout<<"El mejor estado meta es:"<<estados_meta.front()<<endl;
    }
    else
    {
        cout<<"Numero de estados donde se encontro la meta: 0"<<endl;
    }
    cout<<"Nodos visitados: "<<nodos_visitados.size()<<endl;
    list<string>::iterator iterador_visitados;
    cout<<"Numero de estados generados en historico: "<<historico.size()<<endl;
    list<string>::iterator iterador_historica ;
    cout<<"Numero de estados generados en agenda: "<<agenda.size()<<endl;
    
    
}

void awnn_e()
{
    int **puntero_matrizU;
    int **puntero_matrizY;
    int menor=1000;
    int E_umbral=0;
    int suma_pulsos=0;
    int delta_E=1;
    int pulsofinal=0;
    int iteracion=0;
    int current=0;
    unsigned t0, t1;
    
    list <int> historico;
    list <int> agenda;
    list <int> ruta;
    //Matrices dinamicas U e Y
    puntero_matrizU = new int*[lineas_cadena];
    puntero_matrizY = new int*[lineas_cadena];
    for(int i=0;i<lineas_cadena;i++)
    {
        puntero_matrizU[i]= new int[lineas_cadena];
        puntero_matrizY[i]= new int[lineas_cadena];
    }
    //Se llenan de ceros las matrices U e Y        
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           *(*(puntero_matrizU+i)+j)=0;
           *(*(puntero_matrizY+i)+j)=0;
        }
    }
    
    cout<<"indice de nodo incial "<<nodo_s-1<<endl;
    cout<<"indice de nodo final "<<nodo_g-1<<endl;
    historico.push_front(nodo_s-1);
    
    //se copian el costo de los nodos vecinos al nodo inicial.
    for(int i=0;i<lineas_cadena;i++)
    {
        *(*(puntero_matrizU+nodo_s-1)+i)=*(*(puntero_matriz+nodo_s-1)+i);
    }
    
    
    //se obtiene el costo menor de entre los costos a los nodos vecinos del nodo incial.
    for(int i=0;i<lineas_cadena;i++)
    {
        if(*(*(puntero_matrizU+nodo_s-1)+i)!=0)
        {
            if(*(*(puntero_matrizU+nodo_s-1)+i)<=menor)
            {
                menor=*(*(puntero_matrizU+nodo_s-1)+i);
            }
        }
    }
    E_umbral=menor;
    t0=clock();
    do{
        //inicio de ciclo
        //Los valores que complen la condicion del umbral en la matriz u dispararan en la matriz y
        list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int j=0;j<lineas_cadena;j++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+j)!=0)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+j)==E_umbral)
                    {
                        for(int k=0;k<lineas_cadena;k++)
                        {
                            if(*(*(puntero_matrizY+k)+j)==1)
                            {
                                suma_pulsos=suma_pulsos+1;
                            }
                        }                        
                        if(suma_pulsos==0)
                        {
                            *(*(puntero_matrizY+*iterador_historico)+j)=1;
                            agenda.push_back(j);
                            historico.push_back(j);
                        }
                        suma_pulsos=0;
                    }
                }
            }
        }
        //Se agregan los nodos vecinos en la matriz U, de los nodos que dispararon en el nodo Y
        list<int>::iterator iterador_agenda;
        for(iterador_agenda=agenda.begin();iterador_agenda!=agenda.end();iterador_agenda++)
        {
            for(int i=0;i<lineas_cadena;i++)
            {
                if(*(*(puntero_matriz+*iterador_agenda)+i)!=0)
                {
                    *(*(puntero_matrizU+*iterador_agenda)+i)=*(*(puntero_matriz+*iterador_agenda)+i)+E_umbral;
                }

            }
        }


        //se pone el valor alto en los nodos vecinos ya disparados.
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {

            for(int i=0;i<lineas_cadena;i++)
            {
                list<int>::iterator iterador_historico1;
                for(iterador_historico1 = historico.begin();iterador_historico1!=historico.end();iterador_historico1++)
                {
                    if(i==*iterador_historico1 && *(*(puntero_matriz+*iterador_historico)+i)!=0)
                    {
                        *(*(puntero_matrizU+*iterador_historico)+i)=1000;
                    }
                }
            }
        }
        
        //se borran los nodos de la agenda.
        agenda.clear();
        //Se incrementa E_umbral en delta_E, es decir en 1
        E_umbral=E_umbral+delta_E;
        //se verifica si alguno de los vecinos del nodo final se ha encendido
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+nodo_g-1)==1)
            {
                pulsofinal=1;
                cout<<"Nodo alcanzado"<<endl;

            }
        }
        iteracion=iteracion+1;
        
        //fin de ciclo
    }while(pulsofinal!=1);
    current=nodo_g-1;
    ruta.push_front(current);
    cout<<"Nodo final:"<<current<<endl;
    cout<<"Extraccion del conocimiento"<<endl;
    do{
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+current)==1)
            {
                current=i;
                ruta.push_front(current);
            }
        }
    }while(current!=nodo_s-1);
    
    t1=clock();
    /*
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           cout<<*(*(puntero_matrizY+i)+j)<<" "; 
        }
        cout<<endl; 
    }
    cout<<endl;
    */
    cout<<"Ruta óptima:";
    list<int>::iterator iterador_ruta;
    for(iterador_ruta = ruta.begin();iterador_ruta!=ruta.end();iterador_ruta++)
    {
        cout<<*iterador_ruta+1<<"-";
    }
    cout<<endl;
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<endl<<"Costo:"<<E_umbral-delta_E<<endl;
    cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
    cout<<"Iteraciones:"<<iteracion<<endl;
    //Libera memoria
    for(int i=0;i<lineas_cadena;i++)
    {
        delete[] puntero_matrizU[i];
        delete[] puntero_matrizY[i];
        
    }
    delete puntero_matrizU;
    delete puntero_matrizY;
    
}

void sapcnnvm()
{
    int **puntero_matrizU;
    int **puntero_matrizY;
    int menor=1000;
    int E_umbral=0;
    int suma_pulsos=0;
    int delta_E=1;
    int pulsofinal=0;
    int iteracion=0;
    int current=0;
    unsigned t0, t1;
    
    list <int> historico;
    list <int> agenda;
    list <int> ruta;
    
    //Matrices dinamicas U e Y
    puntero_matrizU = new int*[lineas_cadena];
    puntero_matrizY = new int*[lineas_cadena];
    for(int i=0;i<lineas_cadena;i++)
    {
        puntero_matrizU[i]= new int[lineas_cadena];
        puntero_matrizY[i]= new int[lineas_cadena];
    }
    //Se llenan de ceros las matrices U e Y        
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           *(*(puntero_matrizU+i)+j)=0;
           *(*(puntero_matrizY+i)+j)=0;
        }
    }
   
    cout<<"indice de nodo incial "<<nodo_s-1<<endl;
    cout<<"indice de nodo final "<<nodo_g-1<<endl;
    historico.push_front(nodo_s-1);
    
    //se copian el costo de los nodos vecinos al nodo inicial.
    for(int i=0;i<lineas_cadena;i++)
    {
        *(*(puntero_matrizU+nodo_s-1)+i)=*(*(puntero_matriz+nodo_s-1)+i);
    }
    
          
    //se obtiene el costo menor de entre los costos a los nodos vecinos del nodo incial.
    for(int i=0;i<lineas_cadena;i++)
    {
        if(*(*(puntero_matrizU+nodo_s-1)+i)!=0)
        {
            if(*(*(puntero_matrizU+nodo_s-1)+i)<=menor)
            {
                menor=*(*(puntero_matrizU+nodo_s-1)+i);
            }
        }
    }
    E_umbral=menor;
    //Se inicia la toma del tiempo
    t0=clock();
    do{
        
        //inicio de ciclo
        
        //Los valores que complen la condicion del umbral en la matriz U dispararan en la matriz y
        list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int j=0;j<lineas_cadena;j++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+j)!=0)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+j)==E_umbral)
                    {
                        for(int k=0;k<lineas_cadena;k++)
                        {
                            if(*(*(puntero_matrizY+k)+j)==1)
                            {
                                suma_pulsos=suma_pulsos+1;
                            }
                        }                        
                        if(suma_pulsos==0)
                        {
                            *(*(puntero_matrizY+*iterador_historico)+j)=1;
                            agenda.push_back(j);
                            historico.push_back(j);
                        }
                        suma_pulsos=0;
                    }
                }
            }
        }
        //Se agregan los nodos vecinos en la matriz U, de los nodos que dispararon en el nodo Y
        list<int>::iterator iterador_agenda;
        for(iterador_agenda=agenda.begin();iterador_agenda!=agenda.end();iterador_agenda++)
        {
            for(int i=0;i<lineas_cadena;i++)
            {
                if(*(*(puntero_matriz+*iterador_agenda)+i)!=0)
                {
                    *(*(puntero_matrizU+*iterador_agenda)+i)=*(*(puntero_matriz+*iterador_agenda)+i)+E_umbral;
                }

            }
        }


        //se pone el valor alto en los nodos vecinos ya disparados.
        //list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {

            for(int i=0;i<lineas_cadena;i++)
            {
                list<int>::iterator iterador_historico1;
                for(iterador_historico1 = historico.begin();iterador_historico1!=historico.end();iterador_historico1++)
                {
                    if(i==*iterador_historico1 && *(*(puntero_matriz+*iterador_historico)+i)!=0)
                    {
                        *(*(puntero_matrizU+*iterador_historico)+i)=10000;
                    }
                }
            }
        }
        
        //se borran los nodos de la agenda.
        agenda.clear();
        //Se obtiene el costo menor de la matriz U de los nodos adyasentes a los nodos que ya han disparado en la matriz Y.
        //Direrentes de 0 y mayores que el umbral anterior.
        menor=1000;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int i=0;i<lineas_cadena;i++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+i)!=0 && *(*(puntero_matrizU+*iterador_historico)+i)> E_umbral)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+i)<=menor)
                    {
                        menor=*(*(puntero_matrizU+*iterador_historico)+i);
                    }
                }
            }
        }
        //Se incrementa E_umbral en delta_E.
        delta_E=menor-E_umbral;
        E_umbral=E_umbral+delta_E;
        //se verifica si alguno de los vecinos del nodo final se ha encendido
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+nodo_g-1)==1)
            {
                pulsofinal=1;
                cout<<"Nodo alcanzado"<<endl;

            }
        }
        
        
        iteracion=iteracion+1;
        cout<<"iteracion:"<<iteracion<<endl;
        cout<<"--------------------------------"<<endl;
        //fin de ciclo
    }while(pulsofinal!=1);
    current=nodo_g-1;
    ruta.push_front(current);
    cout<<"Nodo final:"<<current<<endl;
    cout<<"Extraccion del conocimiento"<<endl;
    do{
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+current)==1)
            {
                current=i;
                ruta.push_front(current);
            }
        }
    }while(current!=nodo_s-1);
    
    t1=clock();
    /*
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           cout<<*(*(puntero_matrizY+i)+j)<<" "; 
        }
        cout<<endl; 
    }
    cout<<endl;
    */
    cout<<"Ruta óptima:";
    list<int>::iterator iterador_ruta;
    for(iterador_ruta = ruta.begin();iterador_ruta!=ruta.end();iterador_ruta++)
    {
        cout<<*iterador_ruta+1<<"-";
    }
    cout<<endl;
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<endl<<"Costo:"<<E_umbral-delta_E<<endl;
    cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
    cout<<"Iteraciones:"<<iteracion<<endl;
    //Libera memoria
    for(int i=0;i<lineas_cadena;i++)
    {
        delete[] puntero_matrizU[i];
        delete[] puntero_matrizY[i];
        
    }
    delete puntero_matrizU;
    delete puntero_matrizY;
    
}

void sapcnn()
{
    int **puntero_matrizU;
    int **puntero_matrizY;
    int menor=1000;
    int E_umbral=0;
    int suma_pulsos=0;
    int delta_E=1;
    int pulsofinal=0;
    int iteracion=0;
    int current=0;
    int large_constant_Ve=0, W_max=0, y=1,Fp=0;
    unsigned t0, t1;
    
    list <int> historico;
    list <int> agenda;
    list <int> ruta;
    //Se obtiene el la entrada maxima de la matrix W
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
            if(W_max<*(*(puntero_matriz+i)+j))
            {
                W_max=*(*(puntero_matriz+i)+j);
            }
        }
    }
    //se calcula el valor grande
    large_constant_Ve=W_max*lineas_cadena;
    
    //Matrices dinamicas U e Y
    puntero_matrizU = new int*[lineas_cadena];
    puntero_matrizY = new int*[lineas_cadena];
    for(int i=0;i<lineas_cadena;i++)
    {
        puntero_matrizU[i]= new int[lineas_cadena];
        puntero_matrizY[i]= new int[lineas_cadena];
    }
    //Se llenan de ceros las matrices U e Y        
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           *(*(puntero_matrizU+i)+j)=0;
           *(*(puntero_matrizY+i)+j)=0;
        }
    }
    
    cout<<"indice de nodo incial "<<nodo_s-1<<endl;
    cout<<"indice de nodo final "<<nodo_g-1<<endl;
    historico.push_front(nodo_s-1);
    
    //se copian el costo de los nodos vecinos al nodo inicial.
    for(int i=0;i<lineas_cadena;i++)
    {
        *(*(puntero_matrizU+nodo_s-1)+i)=*(*(puntero_matriz+nodo_s-1)+i);
    }
    
    E_umbral=E_umbral+delta_E;
    t0=clock();
    do{
        //inicio de ciclo
        //Los valores que complen la condicion del umbral en la matriz u dispararan en la matriz y
        list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int j=0;j<lineas_cadena;j++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+j)!=0)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+j)<=E_umbral)
                    {
                        for(int k=0;k<lineas_cadena;k++)
                        {
                            if(*(*(puntero_matrizY+k)+j)==1)
                            {
                                suma_pulsos=suma_pulsos+1;
                            }
                        }                        
                        if(suma_pulsos==0)
                        {
                            *(*(puntero_matrizY+*iterador_historico)+j)=1;
                            agenda.push_back(j);
                            historico.push_back(j);
                        }
                        suma_pulsos=0;
                    }
                }
            }
        }
        //Se agregan los nodos vecinos en la matriz U, de los nodos que dispararon en el nodo Y
        list<int>::iterator iterador_agenda;
        for(iterador_agenda=agenda.begin();iterador_agenda!=agenda.end();iterador_agenda++)
        {
            for(int i=0;i<lineas_cadena;i++)
            {
                if(*(*(puntero_matriz+*iterador_agenda)+i)!=0)
                {
                    *(*(puntero_matrizU+*iterador_agenda)+i)=*(*(puntero_matriz+*iterador_agenda)+i)+E_umbral;
                }

            }
        }


        //se pone el valor alto en los nodos vecinos ya disparados.
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {

            for(int i=0;i<lineas_cadena;i++)
            {
                list<int>::iterator iterador_historico1;
                for(iterador_historico1 = historico.begin();iterador_historico1!=historico.end();iterador_historico1++)
                {
                    if(i==*iterador_historico1 && *(*(puntero_matriz+*iterador_historico)+i)!=0)
                    {
                        *(*(puntero_matrizU+*iterador_historico)+i)=large_constant_Ve;
                    }
                }
            }
        }
        
        //se borran los nodos de la agenda.
        agenda.clear();
        
        
        
        //Se obtiene el costo menor de la matriz U de los nodos adyasentes a los nodos que ya han disparado en la matriz Y.
        //Direrentes de 0 y mayores que el umbral anterior.
        menor=1000;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int i=0;i<lineas_cadena;i++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+i)!=0 && *(*(puntero_matrizU+*iterador_historico)+i)> E_umbral)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+i)<=menor)
                    {
                        menor=*(*(puntero_matrizU+*iterador_historico)+i);
                    }
                }
            }
        }
        //Se determina el valor de Fp
        Fp=0;
        if(menor<E_umbral+delta_E)
        {
            Fp=lineas_cadena;
        }
        if(menor==E_umbral+delta_E)
        {
            Fp=-1;
        }
        cout<<Fp<<endl;
        //Se determina el valor de delta_E
        
        if(Fp>0)
        {
           delta_E=y; 
        }
        if(Fp==0)
        {
            delta_E=delta_E+y;
        }
        
        //Se incrementa E_umbral en delta_E, es decir en 1
        E_umbral=E_umbral+delta_E;
        //se verifica si alguno de los vecinos del nodo final se ha encendido
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+nodo_g-1)==1)
            {
                pulsofinal=1;
                cout<<"Nodo alcanzado"<<endl;

            }
        }
        iteracion=iteracion+1;
        
        //fin de ciclo
    }while(pulsofinal!=1);
    current=nodo_g-1;
    ruta.push_front(current);
    cout<<"Nodo final:"<<current<<endl;
    cout<<"Extraccion del conocimiento"<<endl;
    do{
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+current)==1)
            {
                current=i;
                ruta.push_front(current);
            }
        }
    }while(current!=nodo_s-1);
    
    t1=clock();
    /*
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           cout<<*(*(puntero_matrizY+i)+j)<<" "; 
        }
        cout<<endl; 
    }
    cout<<endl;
    */
    cout<<"Ruta óptima:";
    list<int>::iterator iterador_ruta;
    for(iterador_ruta = ruta.begin();iterador_ruta!=ruta.end();iterador_ruta++)
    {
        cout<<*iterador_ruta+1<<"-";
    }
    cout<<endl;
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<endl<<"Costo:"<<E_umbral-delta_E<<endl;
    cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
    cout<<"Iteraciones:"<<iteracion<<endl;
    //Libera memoria
    for(int i=0;i<lineas_cadena;i++)
    {
        delete[] puntero_matrizU[i];
        delete[] puntero_matrizY[i];
        
    }
    delete puntero_matrizU;
    delete puntero_matrizY;
    
}

void awnn()
{
    cout<<"pcnn"<<endl;
    int **puntero_matrizU;
    int **puntero_matrizY;
    int menor=1000;
    int E_umbral=0;
    int suma_pulsos=0;
    int delta_E=1;
    int pulsofinal=0;
    int iteracion=0;
    int current=0;
    int large_constant_Ve=0, W_max=0, y=1,Fp=0;
    unsigned t0, t1;
    
    list <int> historico;
    list <int> agenda;
    list <int> ruta;
    
    //se calcula el valor grande
    large_constant_Ve=170;
    
    //Matrices dinamicas U e Y
    puntero_matrizU = new int*[lineas_cadena];
    puntero_matrizY = new int*[lineas_cadena];
    for(int i=0;i<lineas_cadena;i++)
    {
        puntero_matrizU[i]= new int[lineas_cadena];
        puntero_matrizY[i]= new int[lineas_cadena];
    }
    //Se llenan de ceros las matrices U e Y        
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           *(*(puntero_matrizU+i)+j)=0;
           *(*(puntero_matrizY+i)+j)=0;
        }
    }
    
    cout<<"indice de nodo incial "<<nodo_s-1<<endl;
    cout<<"indice de nodo final "<<nodo_g-1<<endl;
    historico.push_front(nodo_s-1);
    
    //se copian el costo de los nodos vecinos al nodo inicial.
    for(int i=0;i<lineas_cadena;i++)
    {
        *(*(puntero_matrizU+nodo_s-1)+i)=*(*(puntero_matriz+nodo_s-1)+i);
    }
    
    E_umbral=E_umbral+delta_E;
    t0=clock();
    do{
        //inicio de ciclo
        //Los valores que complen la condicion del umbral en la matriz u dispararan en la matriz y
        list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int j=0;j<lineas_cadena;j++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+j)!=0)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+j)<=E_umbral)
                    {
                        for(int k=0;k<lineas_cadena;k++)
                        {
                            if(*(*(puntero_matrizY+k)+j)==1)
                            {
                                suma_pulsos=suma_pulsos+1;
                            }
                        }                        
                        if(suma_pulsos==0)
                        {
                            *(*(puntero_matrizY+*iterador_historico)+j)=1;
                            agenda.push_back(j);
                            historico.push_back(j);
                        }
                        suma_pulsos=0;
                    }
                }
            }
        }
        //Se agregan los nodos vecinos en la matriz U, de los nodos que dispararon en el nodo Y
        list<int>::iterator iterador_agenda;
        for(iterador_agenda=agenda.begin();iterador_agenda!=agenda.end();iterador_agenda++)
        {
            for(int i=0;i<lineas_cadena;i++)
            {
                if(*(*(puntero_matriz+*iterador_agenda)+i)!=0)
                {
                    *(*(puntero_matrizU+*iterador_agenda)+i)=*(*(puntero_matriz+*iterador_agenda)+i)+E_umbral;
                }

            }
        }


        //se pone el valor alto en los nodos vecinos ya disparados.
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {

            for(int i=0;i<lineas_cadena;i++)
            {
                list<int>::iterator iterador_historico1;
                for(iterador_historico1 = historico.begin();iterador_historico1!=historico.end();iterador_historico1++)
                {
                    if(i==*iterador_historico1 && *(*(puntero_matriz+*iterador_historico)+i)!=0)
                    {
                        *(*(puntero_matrizU+*iterador_historico)+i)=large_constant_Ve;
                    }
                }
            }
        }
        
        //se borran los nodos de la agenda.
        agenda.clear();
               
        //Se incrementa E_umbral en delta_E, es decir en 1
        E_umbral=E_umbral+delta_E;
        //se verifica si alguno de los vecinos del nodo final se ha encendido
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+nodo_g-1)==1)
            {
                pulsofinal=1;
                cout<<"Nodo alcanzado"<<endl;

            }
        }
        iteracion=iteracion+1;
        
        //fin de ciclo
    }while(pulsofinal!=1);
    current=nodo_g-1;
    ruta.push_front(current);
    cout<<"Nodo final:"<<current<<endl;
    cout<<"Extraccion del conocimiento"<<endl;
    do{
        for(int i=0;i<lineas_cadena;i++)
        {
            if(*(*(puntero_matrizY+i)+current)==1)
            {
                current=i;
                ruta.push_front(current);
            }
        }
    }while(current!=nodo_s-1);
    
    t1=clock();
    /*
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           cout<<*(*(puntero_matrizY+i)+j)<<" "; 
        }
        cout<<endl; 
    }
    cout<<endl;
    */
    cout<<"Ruta óptima:";
    list<int>::iterator iterador_ruta;
    for(iterador_ruta = ruta.begin();iterador_ruta!=ruta.end();iterador_ruta++)
    {
        cout<<*iterador_ruta+1<<"-";
    }
    cout<<endl;
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout<<endl<<"Costo:"<<E_umbral-delta_E<<endl;
    cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
    cout<<"Iteraciones:"<<iteracion<<endl;
    //Libera memoria
    for(int i=0;i<lineas_cadena;i++)
    {
        delete[] puntero_matrizU[i];
        delete[] puntero_matrizY[i];
        
    }
    delete puntero_matrizU;
    delete puntero_matrizY;
    
}

void run()
{
    double tiempo_promedio=0, suma=0, tiempo;
    cout<<"Buscando..."<<endl;
    switch(algoritmo_seleccionado)
    {
        case 1:
            cout<<"1) Branch and Bound (clasico)"<<endl;
            BranchAndBound();
            break;
            
        case 2:
            cout<<"2) AWNN E"<<endl;
            awnn_e();
            break;
        case 3:
            cout<<"3) SAPCNNVM"<<endl;
            sapcnnvm();
            break;
        case 4:
            cout<<"4) SAPCNN"<<endl;
            sapcnn();
            break;
        case 5:
            cout<<"5) AWNN"<<endl;
            awnn();
            break;
        
    }
    
}

//funcion del menu principal
void mainmenu()
{
    int opcionMenu=0;
    int ndx=0;
    string copiacontenido="";
    string contenido="";
    string coma=",";
    string salto="\n";
    
    
    do{
            printf("Busqueda de rutas\n\n\r");
            printf("Seleccione una opcion ingresando el numero correspondiente.\n\t");
            printf("1)Cargar grafo \n\t2)Seleccionar los nodos \n\t3)Seleccionar algoritmo \n\t4)Run \n\t5)Salir");
            printf("\n\n\rOpcion:");
            std::cin >> opcionMenu;
            system("clear");
            switch(opcionMenu)
            {
                case 1:
                    cargar_grafo1();
                    break;
                case 2:
                    seleccionar_nodos1();
                    break;
                case 3:
                    seleccionar_algoritmo();
                    break;
                case 4:
                    run();
                    break;
                case 5:
                    printf("Salir\n\r");
                    break;
                default:
                    printf("Opcion invalida");
                    break;
            }
    }while(opcionMenu!=5);
    return ;
}

//funcion principal.
int main(int argc, char *argv[]) {
    mainmenu();
    return 0;
}