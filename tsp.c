  //
  // Tomas Costa
  // Student name
  // ...
  //
  // AED, 2018/2019
  //
  // solution of the traveling salesman problem
  //





  //falar sobre o problema, falar sobre as 3 metodologias,
  //para cada uma explicar a approach e mostrar resultados
  //Analise fos tempos de execuçao, comparar numa escala logaritmica
  //Mostrar graficos do matlab
  //Consideraçoes finais e bibliografia

  #include <math.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/time.h>
  #include <sys/resource.h>

  #include "cities.h"
  #include "elapsed_time.h" 


  //
  // record best solutions
  //

  static int min_length,max_length;//,j;
  static int min_tour[max_n_cities + 1],max_tour[max_n_cities + 1];
  static long n_tours;
  static long hist[10000];

  //
  // first solution (brute force, distance computed at the end, compute best and worst tours)
  //

  void tsp_v1(int n,int m,int *a)
  {
    int i,t;

    if(m < n - 1)
      for(i = m;i < n;i++)
      {   //fazer permutaçoes aleatorias
        t = a[m];
        a[m] = a[i];
        a[i] = t;
        tsp_v1(n,m + 1,a);
        t = a[m];
        a[m] = a[i];
        a[i] = t;
      }
    else
    { // visit permutation
      n_tours++;
      int distancia = 0;
      // modify the following code to do your stuff
      a[n] = a[0];
      for(i = 0;i < n;i++)
        distancia += cities[a[i]].distance[a[i+1]];
      hist[distancia]++;
      if(distancia<min_length){
        min_length=distancia;
        for (i = 0; i < n; ++i)
          min_tour[i]=a[i];
      }
      else if(distancia>max_length){
        max_length=distancia;  
        for (i = 0; i < n; ++i)
          max_tour[i]=a[i];
      }
    }
  }

                      // dest     mask
  // Talvez ter um array contador a incrementar D e M
  static int best_distance[max_n_cities][1<<18];
  static int best_path[max_n_cities][1<<18];
  int k = 0;

  int calc_dist(int n, int dest, int mask){
    if (mask == ((1<<n)-1))
       return cities[dest].distance[0];
    if (best_distance[dest][mask] != -1) return best_distance[dest][mask];
      
    int i,m=10000,d;
    for (i = 1; i < n;i++)
     {
      if ((mask&(1<<i))==0)
      {
        d = calc_dist(n,i,mask|(1<<i)) + cities[dest].distance[i];
        if(d<m)
          m=d;
          //best_path[k]=i;
          //k++;
      }
    }
    // Explicacao da
    //00001 -> 00011 & 00010
    //        00011 & 00100 -> 00111 ... -> 11111 

    return best_distance[dest][mask]=m;
  }


  //Para saber melhor caminho, fazemos igual mas guardamos melhores i's em vez de melhores dists
  //Mask comeca a 1 e posicao a 9

  //
  // main program
  //

  int main(int argc,char **argv)
  {
    int n_mec,special,n;//i, a[max_n_cities + 1];
    //char file_name[32];
    //double dt1;

    n_mec = 89016; // CHANGE THIS!
    special = 1;
    init_cities_data(n_mec,special);
    printf("data for init_cities_data(%d,%d)\n",n_mec,special);
    fflush(stdout);
  #if 0
    print_distances();
  #endif

    for(n = 3;n <= n_cities;n++)
    {
        // data for init_cities_data(89016,1)
        // tsp_v1() finished in    0.000s (2 tours generated)
        //   min  1131 [ 0, 1, 2]
        //   max  1417 [ 0, 2, 1]
        // tsp_v1() finished in    0.000s (6 tours generated)
        //   min  1228 [ 0, 3, 1, 2]
        //   max  1851 [ 0, 3, 2, 1]
        // tsp_v1() finished in    0.000s (24 tours generated)
        //   min  1218 [ 0, 2, 3, 1, 4]
        //   max  1898 [ 0, 4, 3, 2, 1]
        // tsp_v1() finished in    0.000s (120 tours generated)
        //   min  1217 [ 0, 2, 3, 1, 5, 4]
        //   max  2556 [ 0, 3, 4, 5, 2, 1]
        // tsp_v1() finished in    0.000s (720 tours generated)
        //   min  1219 [ 0, 2, 5, 1, 3, 6, 4]
        //   max  2609 [ 0, 6, 3, 4, 5, 2, 1]
        // tsp_v1() finished in    0.000s (5040 tours generated)
        //   min  1295 [ 0, 1, 5, 3, 6, 4, 2, 7]
        //   max  3343 [ 0, 6, 7, 3, 4, 5, 2, 1]
        // tsp_v1() finished in    0.001s (40320 tours generated)
        //   min  1378 [ 0, 8, 1, 5, 3, 6, 4, 2, 7]
        //   max  3772 [ 0, 6, 7, 3, 8, 4, 5, 2, 1]
        // tsp_v1() finished in    0.008s (362880 tours generated)
        //   min  1395 [ 0, 7, 9, 2, 4, 6, 3, 5, 1, 8]
        //   max  4298 [ 0, 6, 7, 3, 2, 8, 4, 5, 9, 1]
        // tsp_v1() finished in    0.070s (3628800 tours generated)
        //   min  1423 [ 0, 7, 9, 2, 4, 6, 3, 5, 1,10, 8]
        //   max  4839 [ 0, 6, 7, 3, 8, 2,10, 4, 5, 9, 1]
        // tsp_v1() finished in    0.750s (39916800 tours generated)
        //   min  1625 [ 0, 7, 9, 2, 4, 6, 3, 5, 1,11,10, 8]
        //   max  5751 [ 0, 3, 8, 2,10, 4,11, 6, 7, 5, 9, 1]
        // tsp_v1() finished in    9.213s (479001600 tours generated)
        //   min  1673 [ 0, 7, 9,12, 2, 4, 6, 3, 5, 1,11,10, 8]
        //   max  6294 [ 0, 3,12,10, 4,11, 2, 8, 6, 7, 5, 9, 1]
        // tsp_v1() finished in  129.047s (6227020800 tours generated)
        //   min  1655 [ 0, 7, 9,12, 2, 4, 6, 3, 5, 1,11,10, 8,13]
        //   max  6500 [ 0, 3, 8, 2,13,12,10, 4,11, 6, 7, 5, 9, 1]
        // tsp_v1() finished in 1816.690s (87178291200 tours generated)
        //   min  1675 [ 0, 7, 9,12, 2, 4, 6, 3, 5, 1,11,10, 8,13,14]
        //   max  6605 [ 0, 8, 2,13,12,10, 4,11, 6, 7, 3,14, 5, 9, 1]
      //dt1 = -1.0;
      //if(n <= 12) //mudar aqui o n cidades
      //{
        // FILE *fp = fopen("hist.txt","w");
        // for(j=0;j<10000;j++)
        //   fprintf(fp,"%d %ld\n",j,hist[j]);
        // fclose(fp);
        // (void)elapsed_time();
        // for(i = 0;i < n;i++)
        //   a[i] = i;
        // min_length = 1000000000;
        // max_length = 0;
        // n_tours = 0l;
        // tsp_v1(n,1,a); // no need to change the starting city, as we are making a tour
        // dt1 = elapsed_time();

        // printf("tsp_v1() finished in %8.3fs (%ld tours generated)\n",dt1,n_tours);
        // printf("  min %5d [",min_length);
        // for(i = 0;i < n;i++)
        //   printf("%2d%s",min_tour[i],(i == n - 1) ? "]\n" : ",");
        // printf("  max %5d [",max_length);
        // for(i = 0;i < n;i++)
        //   printf("%2d%s",max_tour[i],(i == n - 1) ? "]\n" : ",");
        // fflush(stdout);
        // if(n==12 || n==15)
        // {
        //   min_tour[n] = -1;
        //   sprintf(file_name,"min_%02d.svg",n);
        //   make_map(file_name,min_tour);
        //   max_tour[n] = -1;
        //   sprintf(file_name,"max_%02d.svg",n);
        //   make_map(file_name,max_tour);
        //}
      //}
      for (int i = 0; i<n; i++){
        for (int j = 0; j<(1<<n); j++){
          best_distance[i][j]=-1;
        }
      }

      int best=calc_dist(n,0,1);
      printf("Primeiras %d cidades: %d\n",n, best);
    }
    double dt2 = elapsed_time();
    printf("Finished in %8.3fs\n",dt2);

    printf("Melhor Trajeto: ");
    for (int i = 0; i <= 18; i++)
    {
      printf("%d ", best_path[i]);
    }

    printf("\n");
    return 0;
  }
