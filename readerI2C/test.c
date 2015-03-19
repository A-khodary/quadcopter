#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
  int fd ;
  char c;
  char buffer[32];
  char finishedbuff[32];
  char *data;
  char *numb;
  char data_c[16];
  char numb_c[16];
  int i;
  int inside, started, j, k, number = 0;
  float pwm1, pwm2, pwm3, pwm4, pwm5, pwm6, pwm7, pwm8, pwm9, ultradist;

  if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
  {
    printf("ERROR opening serial");
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    printf("ERROR launching WPI");
    return 1 ;
  }
while (1)
{

     c = serialGetchar(fd);
     if(c == '_')
     {
      if (started)
      {
        buffer[i]='\0';
	data =  strtok( buffer, "=");
	numb = strtok(NULL, "=");
	if (data != NULL && numb != NULL)
	{
        //printf("Data : %s containing : %s\n", data, numb);
	//fflush(stdout);

	if (!strcmp(data, "pwm1"))
	{
		pwm1 = strtof(numb, NULL);
		printf("Got PWM1 value : %f\n", pwm1);
                fflush(stdout);
	}

	if (!strcmp(data, "pwm2"))
        {
                pwm1 = strtof(numb, NULL);
                printf("Got PWM2 value : %f\n", pwm2);
                fflush(stdout);
        }

	}

	

        strcpy(buffer, "");
        i=0;
      }
      else
      {
      	started=1;
        strcpy(buffer, "");
      }
     }

     else
     {
     buffer[i]=c;
     i++;
     }

}

return 0;
}


