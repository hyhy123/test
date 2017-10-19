#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#include "random_16807.h"
#include "Draw_Path.h"

typedef struct SystemInformation_Type{
  int n_total;
  int n_mc;
  int n_sweep;
  double epsilon;
  double delta;
  int intval;
} SystemInfo_T;
typedef SystemInfo_T *SystemInfo_P_T;

int GetSystemInfo(SystemInfo_P_T systeminfo_p){
  printf("INPUT N TOTAL: ");
  scanf("%d",&(systeminfo_p->n_total));
  printf("INPUT N_MC: ");
  scanf("%d",&(systeminfo_p->n_mc));
  printf("INPUT epsilon: ");
  scanf("%lf",&(systeminfo_p->epsilon));
  printf("INPUT delta: ");
  scanf("%lf",&(systeminfo_p->delta));
  printf("INPUT N sweep: ");
  scanf("%d", &(systeminfo_p->n_sweep));
  printf("INPUT INTERVAL:");
  scanf("%d",&(systeminfo_p->intval));
  return 0;
}

double GetDeltaAction(double *position_p,int index_k,double delta,
		      SystemInfo_P_T systeminfo_p){
  double action = 0;
  double A = (1/systeminfo_p->epsilon + systeminfo_p->epsilon / 4);
  double B = (1/systeminfo_p->epsilon - systeminfo_p->epsilon / 4);

  // if ( index_k == 0){
  //   action = A * 2 * position_p[index_k] * delta
  //     - B * position_p[index_k + 1] * delta + delta * delta;
  //   return action;
  // }
  // if ( index_k == systeminfo_p->n_total){
  //   action = A * 2 * position_p[index_k] * delta
  //     - B * position_p[index_k - 1] * delta + delta * delta;
  //   return action;
  // }

  action = A * 2 * position_p[index_k] * delta
    - B * (position_p[index_k-1] + position_p[index_k+1]) * delta
    + delta * delta * A;
  return action;
}

int RelaxProcess(double *position_p,SystemInfo_P_T systeminfo_p){
  int index_k; double delta,action,propability,randn;
  
  index_k = (int)((double)random_16807() / myrandom_max
		  * (systeminfo_p->n_total-1))+1; /* "+1" for index n_total */
  delta = (2*(double) random_16807() / myrandom_max -1)* systeminfo_p->delta;
  action = GetDeltaAction(position_p, index_k, delta, systeminfo_p);
  //printf("%lf %lf \n",delta ,action);
  
  propability = exp(-action)<1?exp(-action):1;
  randn = (double) random_16807() / myrandom_max;
  if ( randn < propability){
    position_p[index_k] += delta;
    return 1;
  } // Accepted, return 1
  else
    return 0;			// refused return 0;
}

int CheckAutocorrelation(double *position_p,SystemInfo_P_T systeminfo_p){
  /* Check autocorrelation */
  double autocor[100];
  double *tmp_position_p;
  int i,j;
  tmp_position_p = (double *) malloc(sizeof(double) * (systeminfo_p->n_total + 1));

  double avg_p=0;
  for (i=0;i<systeminfo_p->n_total;i++){
    tmp_position_p[i] = position_p[i];
    avg_p += position_p[i];
  }
  avg_p /= systeminfo_p->n_total;
  
  for ( i=0;i<100;i++){
    for (j=1;j<systeminfo_p->n_total;j++)
      RelaxProcess(position_p, systeminfo_p);
    autocor[i] = 0;
    for (j=1;j<systeminfo_p->n_total;j++){
      autocor[i] += position_p[j] * tmp_position_p[j];
    }
  }

  FILE *fp = fopen("autocorrelation.out","w"); 
  for (i=0;i<100;i++){
    if ( i%10 == 0){
      autocor[i] /= systeminfo_p->n_total;
      fprintf(fp,"%d\t%lf\n",i,(autocor[i]-avg_p *avg_p)/(autocor[0]-avg_p*avg_p));
    }
  }
  fclose(fp);
  free(tmp_position_p);

  return 0;
}

int main(int argc,char **argv){
  SystemInfo_T systeminfo;
  GetSystemInfo(&systeminfo);
  double *position_p;
  position_p = (double *) malloc(sizeof(double)*(systeminfo.n_total+1));

  getseed();
#ifdef DRAW
  InitWindow(argc, argv);
#endif
  int i,j;
  position_p[0] = 0;
  position_p[systeminfo.n_total] = 0;
  for (i=1;i<systeminfo.n_total;i++) /* Initialize A Path (Hot Initialization)*/
    position_p[i] = 2 * (double) random_16807() / myrandom_max - 1;
    //position_p[i] = 0;
  int n_therm = 1000;

  double statavg=0; // stat avg for f
  double statsquareavg= 0; // stat avg for  f^2
  int count=0;
  int interval=systeminfo.intval;

  for (i=0;i<n_therm;i++){ 	// Thermalizeing
    for (j=1;j<systeminfo.n_total;j++)
      RelaxProcess(position_p, &systeminfo);
    
#ifdef DRAW
    //display(position_p, systeminfo.n_total);
    //    getchar();
#endif
  }
  
  CheckAutocorrelation(position_p, &systeminfo);
  
  /* Calculate avg for <u(t)u(t+t0)>*/
  long accept_num=0;
  long k;
  double autocor[100];
  for (k=0;k<100;k++) autocor[k] = 0;
  double *tmp_position_p;
  tmp_position_p = (double *) malloc(sizeof(double) * (systeminfo.n_total + 1));
  
  for (i=0;i<systeminfo.n_mc;i++){
    
#ifdef DRAW
    display(position_p, systeminfo.n_total);
    //getchar();
#endif
    
    // statistics
    for (k=interval;k<systeminfo.n_total-interval;k+=20){
      statavg += position_p[k]*position_p[k + interval];
      statsquareavg += position_p[k]*position_p[k + interval] *position_p[k]*position_p[k + interval];
      count++;
    }

    for (k=0;k<systeminfo.n_total;k++)
      tmp_position_p[k] = position_p[k];
    
    for (k=0;k<systeminfo.n_sweep;k++){ // reduce autocorrelation bewteen two paths
      for (j=interval;j<systeminfo.n_total - interval;j+=20)
	autocor[k] += position_p[j]*position_p[j+interval]
	  * tmp_position_p[j] * tmp_position_p[j+interval];
      for (j=1;j<systeminfo.n_total;j++){
	accept_num += RelaxProcess(position_p, &systeminfo);
      }
    }
    
    
  }
  statavg /= count;
  statsquareavg /= count;
  printf("Accept_Rate:\t%lf\n",(double)accept_num / systeminfo.n_total / systeminfo.n_sweep/systeminfo.n_mc);
  printf("<x(t)x(t+%d)>=\t%lf\n",systeminfo.intval,statavg);
  printf("Standard_Error:\t %f\n", sqrt((statsquareavg - statavg * statavg) / count));
  free(position_p);

  FILE *fp = fopen("autocorrelation.out","w"); 
  for (i=0;i<100;i++){
    if ( i%5 == 0){
      autocor[i] /= count;
      fprintf(fp,"%d\t%lf\n",i,(autocor[i]-statavg *statavg)/(statsquareavg-statavg*statavg));
    }
  }
  fclose(fp);
  free(tmp_position_p);
}
