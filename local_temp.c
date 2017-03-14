#include <mraa/aio.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

sig_atomic_t volatile run = 1;

void sig_handler(int sig){
  if (sig == SIGINT)
    run = 0;
}

//returns double corresponding to Fahrenheit temperature
// similar to sample code on Grove sensor site
double read_analog_temperature(int ana_sig){
  int B = 4275;
  double R = 1023.0 / ((double) ana_sig) - 1.0;
  R = R * 100000.0;
  double temperature = 1.0 / (log(R/100000.0)/B+1/298.15) - 273.15;
  temperature = temperature * (9 / 5) + 32;
  return temperature;
}

int main(void){

  uint16_t value;
  mraa_aio_context temperature = mraa_aio_init(0);

  FILE* log_txt = fopen("part1_log.txt", "w");
  signal(SIGINT, sig_handler);

  while(run){
    //get time and read from temperature sensor
    time_t curr_time = time(NULL);
    value = mraa_aio_read(temperature);
    
    //format time
    struct tm* timestamp = localtime(&curr_time);
    char out_time[10];
    memset(out_time, 0, 10);
    strftime(out_time, 9, "%H:%M:%S", timestamp);
    
    //convert signal and output data
    double f_temperature = read_analog_temperature(value);
    fprintf(log_txt, "%s %.1f\n", out_time, f_temperature);
    sleep(1);
  }

  fclose(log_txt);
  mraa_aio_close(temperature);

}




