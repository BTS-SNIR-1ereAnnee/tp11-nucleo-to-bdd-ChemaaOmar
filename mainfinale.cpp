#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>         // descripteurs de fichiers ( open()...)
#include <termios.h>    //
#include <errno.h>
#include <iostream>
#include <mariadb/mysql.h>
#include <stdlib.h>

#define HOST "localhost"
#define USER "bts"
#define PWD  "snir"
#define BASE "capteurs"
#define BAUDRATE B115200            
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE;
int main()
{
    int sfd, c, res;
    struct termios newtio;
    char buf[255];
    ssize_t size;

    
    sfd = open(MODEMDEVICE, O_RDONLY | O_NOCTTY );
    if (sfd == -1)
    {
      printf ("Error  no is : %d\n", errno);
      printf("Error description is : %s\n",strerror(errno));
      return(-1);
    }

    bzero(&newtio, sizeof(newtio));
       
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
         
        
    newtio.c_iflag = IGNPAR | ICRNL;
         
       
    newtio.c_lflag = ICANON;
         

     tcflush(sfd, TCIFLUSH);
     tcsetattr(sfd,TCSANOW,&newtio);
    
    for (int i =0 ; i<50 ; i++)
    {
        
    
      size = read (sfd, buf, 127);
   
     
 
      buf[size] = 0;
   
     
      std::cout << i << " " << buf << "\n";
      }
 
    /* Ne pas oublier de libérer ton file descriptor */
    close(sfd);
 
    MYSQL *conn;

    if ((conn = mysql_init(NULL)) == NULL)

    {

    fprintf(stderr, "Could not init DB\n");
    return EXIT_FAILURE;
    }

    if (mysql_real_connect(conn, "localhost", "bts", "snir", "capteurs", 0, NULL,
    0) == NULL)
    {

    fprintf(stderr, "DB Connection Error\n");
    return EXIT_FAILURE;

    }

    if (mysql_query(conn, "INSERT INTO Mesures (temperature,humidite,pression) VALUES ('15','5','10')") != 0)
    {

    fprintf(stderr, "Query Failure\n");
    return EXIT_FAILURE;

    }

    mysql_close(conn);
    return EXIT_SUCCESS;
    return 0;

}
