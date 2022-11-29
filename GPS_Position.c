#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_users;

struct user_t {
  char name[256];
  double longitude;
  double latitude;
  double altitude;
  double time;
} our_user, other_users[1024];

struct distance{
    char name[1024];
    double dist;
}arr_dist[1024];


int scan_users(char *name1, double nanosec, double lati, double longi,
               double alti, int ZeroOrOne, int count) {

  if (ZeroOrOne == 0) {
    strcpy(our_user.name, name1);
    our_user.time = nanosec;
    our_user.longitude = longi;
    our_user.latitude = lati;
    our_user.altitude = alti;
  }

  else {
      strcpy(other_users[count].name, name1);
      other_users[count].time = nanosec;
      other_users[count].longitude = longi;
      other_users[count].latitude = lati;
      other_users[count].altitude = alti;
  }

  return 0;
}

int data_collection(char *filename) {
  char name[256];
  double nanosec;
  double lati;
  double longi;
  double alti;
  int count = 0;

  FILE *read_file;
  read_file = fopen(filename, "r");

  if (read_file == NULL) {
    printf("Error opening file.\n");
    return 1;
  }

  fscanf(read_file, "%d", &num_users);

  for (int i = 0; i < ((num_users + 1) * 5); i += 5) {
    fscanf(read_file, "%s", &name[0]);
    fscanf(read_file, "%lf", &nanosec);
    fscanf(read_file, "%lf", &lati);
    fscanf(read_file, "%lf", &longi);
    fscanf(read_file, "%lf", &alti);

    if (i == 0) {
      scan_users(name, nanosec, lati, longi, alti, 0, count);
    }

    else {
      scan_users(name, nanosec, lati, longi, alti, 1, count);
      count++;
    }
  }

  fclose(read_file);

  return 0;
}

int distance(int num_users){
  for(int i = 0; i < num_users; i++){
    double calc_dist = sqrt( ( (our_user.latitude - other_users[i].latitude)*(our_user.latitude - other_users[i].latitude) ) + ( (our_user.longitude - other_users[i].longitude)*(our_user.longitude - other_users[i].longitude) ) + ( (our_user.altitude - other_users[i].altitude)*(our_user.altitude - other_users[i].altitude) ) );

    strcpy(arr_dist[i].name, other_users[i].name);
    arr_dist[i].dist = calc_dist;
  }
  return 0;
}

int find_closest(int num_users){
  int min = arr_dist[0].dist;
  int index;

  for (int i = 0; i < num_users; i++) {   
    if(arr_dist[i].dist < min){    
      min = arr_dist[i].dist;
      index = i;
    }      
  }

  return index;
}

int main(void) {
  char filename[256];
  printf("Enter a filename: ");
  scanf("%s", &filename);

  data_collection(filename);
  distance(num_users);
  
  int index = find_closest(num_users);
  //printf("%s\n", arr_dist[5].name);
  //printf("%lf\n", arr_dist[5].dist);
  
  printf("The name of the closest user is %s.\n", arr_dist[index].name);
  printf("The location of %s is:\n{%lf Latitude, %lf Longitude, %lf Altitude}.\n", arr_dist[index].name, other_users[index].latitude, other_users[index].longitude, other_users[index].altitude);

}