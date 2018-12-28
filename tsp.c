  //
  // Tomas Costa
  // Student name
  // ...
  //
  // AED, 2018/2019
  //
  // solution of the traveling salesman problem
  //

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

  static int min_length,max_length,j;
  static int min_tour[max_n_cities + 1],max_tour[max_n_cities + 1],min_tour2[max_n_cities + 1],max_tour2[max_n_cities + 1];
  static long n_tours;
  static long hist[10000];

  //
  // TSP V1-bruce force
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

//TSP V2-dynamic programming

	int best_dist[max_n_cities][1 << max_n_cities];
	int best_path[max_n_cities][1 << max_n_cities];

int dist_min(int dest, int mask){
  if(mask == 0)
    return cities[0].distance[dest];
  if (best_dist[dest][mask]==0)
  {
    int i,m=1000000,p=-1;
    for (i = 0; i<max_n_cities ; i++)
    {
      if (mask & (1<<i))
      {
       int d = dist_min(i,mask ^ (1<<i)) + cities[i].distance[dest];
        if (d < m)
        {
          m = d;
          p = i;
        }
      }
    }
    best_dist[dest][mask] = m;
    best_path[dest][mask] = p;
  }
  return best_dist[dest][mask];
}

int dist_max(int dest, int mask){
  if(mask == 0)
    return cities[0].distance[dest];
  if (best_dist[dest][mask]==0)
  {
    int i,m=0,p=-1;
    for (i = 0; i<max_n_cities ; i++)
    {
      if (mask & (1<<i))
      {
       int d = dist_max(i,mask ^ (1<<i)) + cities[i].distance[dest];
        if (d > m)
        {
          m = d;
          p = i;
        }
      }
    }
    best_dist[dest][mask] = m;
    best_path[dest][mask] = p;
  }
  return best_dist[dest][mask];
}
  //Mask comeca a 1 e posicao a 9

void tsp_v2(int n)
{
  int count=0;
  memset(&best_dist[0][0],0,sizeof(best_dist));

  int d,d2,m,ii;

  min_length = 100000;
  max_length=0;
  m = ii = -1;
  for (int i = 1; i < n; i++)
  {
    d=dist_min(i,(1<<n)-1-(1<<0)-(1<<i)) + cities[i].distance[0];
  
    if (d<min_length)
    {
      min_length=d;
      ii = i;
      m = (1<<n)-1-(1<<0)-(1<<i);
    }
  }
  min_tour2[count]=0;
  count++;
  min_tour2[count]=ii;
  while(m != 0)
  {
    count++;
    ii = best_path[ii][m];
    m ^= 1 << ii;
    min_tour[count]=ii;
  }
  min_tour[count+1]=0;
  memset(&best_dist[0][0],0,sizeof(best_dist));
  
  m = ii = -1;
  count=0;

  for (int i = 1; i < n; i++)
  {
    d2=dist_max(i,(1<<n)-1-(1<<0)-(1<<i)) + cities[i].distance[0];
  
    if (d2>max_length)
    {
      max_length=d2;
      ii = i;
      m = (1<<n)-1-(1<<0)-(1<<i);
    }
  }

  max_tour2[count]=0;
  count++;
 max_tour2[count]=ii;

  while(m != 0)
  {
    count++;
    ii = best_path[ii][m];
    m ^= 1 << ii;
    max_tour2[count]=ii;
  }
  max_tour2[count+1]=0;

}
  //
  // main program
  //

  int main(int argc,char **argv)
  {
    int n_mec,special,n,i,a[max_n_cities + 1];
    char file_name[32];
    double dt1;

    n_mec = 89016; // CHANGE THIS!
    special = 1;
    init_cities_data(n_mec,special);
    printf("data for init_cities_data(%d,%d)\n",n_mec,special);
    fflush(stdout);
  #if 0
    print_distances();
  #endif
  
	 // TSP V1
	 
   for(n = 3;n <= n_cities;n++)
    {
      dt1 = -1.0;
      if(n <= 2) //mudar aqui o n cidades
      {
        FILE *fp = fopen("hist.txt","w");
        for(j=0;j<10000;j++)
          fprintf(fp,"%d %ld\n",j,hist[j]);
        fclose(fp);
        (void)elapsed_time();
        for(i = 0;i < n;i++)
          a[i] = i;
        min_length = 1000000000;
        max_length = 0;
        n_tours = 0l;
        tsp_v1(n,1,a); // no need to change the starting city, as we are making a tour
        dt1 = elapsed_time();

        printf("tsp_v1() finished in %8.3fs (%ld tours generated)\n",dt1,n_tours);
        printf("  min %5d [",min_length);
        for(i = 0;i < n;i++)
          printf("%2d%s",min_tour[i],(i == n - 1) ? "]\n" : ",");
        printf("  max %5d [",max_length);
        for(i = 0;i < n;i++)
			  printf("%2d%s",max_tour[i],(i == n - 1) ? "]\n" : ",");
		    fflush(stdout);
		
          min_tour[n] = -1;
          sprintf(file_name,"min_%02d.svg",n);
          make_map(file_name,min_tour);
          max_tour[n] = -1;
          sprintf(file_name,"max_%02d.svg",n);
          make_map(file_name,max_tour);
    }
}
    // TSP V2
 
    dt1 = -1.0;
    for(n = 3;n <= n_cities;n++)
    {
      (void)elapsed_time();
      tsp_v2(n); // no need to change the starting city, as we are making a tour
      dt1 = elapsed_time();

      printf(" %d cidades tsp_v2() finished in %8.3fs\n",n,dt1);
    
      printf("  min %5d [",min_length); 

      for(i = n;i >= 0 ;i--)
        printf( "%2d%s",min_tour2[i], (i == 0) ? "]\n" : "," );

      printf("  max %5d [",max_length);

     for(i = n;i >= 0 ;i--)
        printf( "%2d%s",max_tour2[i], (i == 0) ? "]\n" : "," );

      fflush(stdout);

    }
  return 0;
}
