#ifndef S_SCAN_HEADER
#define S_SCAN_HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KBLK  "\x1B[30m"

void Uppercase(char * temp) {
  char * name;
  name = strtok(temp,":");

  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }

}

int S_Scan_V(char *ip)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *record;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int p_total = 20;
	int p_geschlossen = p_total;
	int ret, status;
	char port[6] = { 0 };
	
	int offen_ports[150];
	int p_offen;

	struct timeval timeout;
	socklen_t len = sizeof(timeout);

	timeout.tv_sec = 0;
	timeout.tv_usec = 150000;
	
	struct servent *sp;
	
	printf("\n%s ────────────────────────────────────────────────── \n", KYEL);
	printf(" %s Scan-Typ : Schnell Scan \n", KWHT);
	printf(" %s Scan Ziel : %s \n", KWHT, ip);
	printf(" %s Gescannte Ports : 21, 22, 23, 25, 53, 80, 110,\n  111, 135, 139, 143, 194, 443, 445, 993, 995,\n  3306, 3389, 5900, 8080, 8081\n", KWHT);
	printf("%s ────────────────────────────────────────────────── \n \n", KYEL, KWHT);

	for (int i = 0; i <= 20; i++)
	{	
		switch(i)
		{
			case 0:
				sprintf(port, "%d", 21);
				break;
			case 1:
				sprintf(port, "%d", 22);
				break;
			case 2:
				sprintf(port, "%d", 23);
				break;
			case 3:
				sprintf(port, "%d", 25);
				break;
			case 4:
				sprintf(port, "%d", 53);
				break;
			case 5:
				sprintf(port, "%d", 80);
				break;
			case 6:
				sprintf(port, "%d", 110);
				break;
			case 7:
				sprintf(port, "%d", 111);
				break;
			case 8:
				sprintf(port, "%d", 135);
				break;
			case 9:
				sprintf(port, "%d", 139);
				break;
			case 10:
				sprintf(port, "%d", 143);
				break;
			case 11:
				sprintf(port, "%d", 194);
				break;
			case 12:
				sprintf(port, "%d", 443);
				break;
			case 13:
				sprintf(port, "%d", 445);
				break;
			case 14:
				sprintf(port, "%d", 993);
				break;
			case 15:
				sprintf(port, "%d", 995);
				break;
			case 16:
				sprintf(port, "%d", 3306);
				break;
			case 17:
				sprintf(port, "%d", 3389);
				break;
			case 18:
				sprintf(port, "%d", 5900);
				break;
			case 19:
				sprintf(port, "%d", 8080);
				break;
			case 20:
				sprintf(port, "%d", 8081);
				break;
		}
		
		time_t t = time(NULL);
  		struct tm tm = *localtime(&t);
  		
  		printf("%s [%02d:%02d:%02d]", KYEL, tm.tm_hour, tm.tm_min, tm.tm_sec);
		
		printf("%s Scannen von Port %s \n", KWHT, port);
		
		if ((ret = getaddrinfo(ip, port, &hints, &res)) != 0)
		{
			fprintf(stderr, "\n %sGetAddrInfo Fehler: %s\n", KRED, gai_strerror(ret));
			return -1;
		}

		for (record = res; record != NULL; record = record->ai_next)
		{
			status = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
			if (status == -1) continue; 
			
			int s_port = atoi( port );
			
			sp = getservbyport( htons(s_port), NULL );

			setsockopt(status, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);
			if (connect(status, record->ai_addr, record->ai_addrlen) != -1)
			{
				if (sp)
				{
					Uppercase(sp->s_name);
					printf("%s [%02d:%02d:%02d]", KYEL, tm.tm_hour, tm.tm_min, tm.tm_sec);
					printf(" %s%s OFFEN [%s] \n", KGRN, port, sp->s_name);
					
					p_offen = p_total - p_geschlossen;
					offen_ports[p_offen] = s_port;
					
					p_geschlossen--;				
					close(status);
					break;
				}
				else
				{
					printf("%s [%02d:%02d:%02d]", KYEL, tm.tm_hour, tm.tm_min, tm.tm_sec);
					printf(" %s%s OFFEN \n", KGRN, port);
										
					p_offen = p_total - p_geschlossen;
					offen_ports[p_offen] = s_port;
					
					p_geschlossen--;				
					close(status);
					break;
				}
			}
			close(status);
		}
	}
	
	p_offen = p_total - p_geschlossen;
	
	if(p_offen == 0)
	{
		printf("\n%s %d gescannt : Kein geöffneter Port wird erkannt. \n", KRED, p_total);
	}
	else
	{
		printf("\n%s %d Port(s) gescannt : %d offen, %d geschlossen. \n \n", KGRN, p_total, p_offen, p_geschlossen);
		
		int a;
		printf(" %sOffen Port(s) : %s\n", KYEL, KWHT);
		
		for(a = 0; a < p_offen; a++){
			
			sp = getservbyport( htons(offen_ports[a]), NULL );
			Uppercase(sp->s_name);
			
			if (a + 1 == p_offen)
			{
				printf("   └──%d [%s] \n", offen_ports[a], sp->s_name);
			}
			else
			{
				printf("   ├──%d [%s] \n", offen_ports[a], sp->s_name);
			}	
		}
	}
	
	char g_otp[5];
	
	printf("\n %3sSpeichern ? [J/N] >> ", KYEL);
	scanf("%s", &g_otp);

	if( strcmp ("J", g_otp) == 0 || strcmp ("j", g_otp) == 0)
	{
		char otp[100];
		sprintf(otp, "%s.txt", ip);
	
		FILE *f = fopen(otp, "w");
		
		if (f == NULL)
		{
		    printf("%s Fehler beim Speichern der Ausgabe.\n", KRED);
		    exit(1);
		}
				
		fprintf(f, "%s ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n", KYEL);
		fprintf(f, "%s                SCHNELL SCAN SUB-MODUL              \n", KWHT);
		fprintf(f, "%s ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n", KYEL);

		fprintf(f, "\n%s ────────────────────────────────────────────────── \n", KYEL);
		fprintf(f, " %s Scan-Typ : Schnell Scan \n", KWHT);
		fprintf(f, " %s Scan Ziel : %s \n", KWHT, ip);
		fprintf(f, " %s Gescannte Ports : 21, 22, 23, 25, 53, 80, 110,\n  111, 135, 139, 143, 443, 445, 993, 995, 1723,\n  3306, 3389, 5900, 8080, 8081\n", KWHT);
		fprintf(f, "%s ────────────────────────────────────────────────── \n \n", KYEL, KWHT);
		
		if(p_offen == 0)
		{
			fprintf(f, "%s %d gescannt : Kein geöffneter Port wird erkannt. \n", KRED, p_total);
		}
		else
		{
			fprintf(f, "%s %d Port(s) gescannt : %d offen, %d geschlossen. \n \n", KGRN, p_total, p_offen, p_geschlossen);
			
			int a;
			fprintf(f, "%s Offen Port(s) : %s\n", KYEL, KWHT);
			
			for(a = 0; a < p_offen; a++){
				
				sp = getservbyport( htons(offen_ports[a]), NULL );
				Uppercase(sp->s_name);
				
				if (a + 1 == p_offen)
				{
					fprintf(f, "   └──%d [%s] \n", offen_ports[a], sp->s_name);
				}
				else
				{
					fprintf(f, "   ├──%d [%s] \n", offen_ports[a], sp->s_name);
				}	
			}
		}
		
		fclose(f);
		
		printf("\n %sAusgabe gespeichert als : %s \n", KWHT, otp);
	}

	freeaddrinfo(res);
	return 0;
}

int S_Scan_NV(char *ip)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *record;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int p_total = 20;
	int p_geschlossen = p_total;
	int ret, status;
	char port[6] = { 0 };
	
	int offen_ports[150];
	int p_offen;

	struct timeval timeout;
	socklen_t len = sizeof(timeout);

	timeout.tv_sec = 0;
	timeout.tv_usec = 150000;
	
	struct servent *sp;
	
	printf("\n%s ──────────────────────────────────────────────────\n", KYEL);
	printf(" %s Scan-Typ : Schnell Scan \n", KWHT);
	printf(" %s Scan Ziel : %s \n", KWHT, ip);
	printf(" %s Gescannte Ports : 21, 22, 23, 25, 53, 80, 110,\n  111, 135, 139, 143, 443, 445, 993, 995, 1723,\n  3306, 3389, 5900, 8080, 8081\n", KWHT);
	printf("%s ────────────────────────────────────────────────── \n \n", KYEL, KWHT);

	for (int i = 0; i <= 20; i++)
	{	
		switch(i)
		{
			case 0:
				sprintf(port, "%d", 21);
				break;
			case 1:
				sprintf(port, "%d", 22);
				break;
			case 2:
				sprintf(port, "%d", 23);
				break;
			case 3:
				sprintf(port, "%d", 25);
				break;
			case 4:
				sprintf(port, "%d", 53);
				break;
			case 5:
				sprintf(port, "%d", 80);
				break;
			case 6:
				sprintf(port, "%d", 110);
				break;
			case 7:
				sprintf(port, "%d", 111);
				break;
			case 8:
				sprintf(port, "%d", 135);
				break;
			case 9:
				sprintf(port, "%d", 139);
				break;
			case 10:
				sprintf(port, "%d", 143);
				break;
			case 11:
				sprintf(port, "%d", 194);
				break;
			case 12:
				sprintf(port, "%d", 443);
				break;
			case 13:
				sprintf(port, "%d", 445);
				break;
			case 14:
				sprintf(port, "%d", 993);
				break;
			case 15:
				sprintf(port, "%d", 995);
				break;
			case 16:
				sprintf(port, "%d", 3306);
				break;
			case 17:
				sprintf(port, "%d", 3389);
				break;
			case 18:
				sprintf(port, "%d", 5900);
				break;
			case 19:
				sprintf(port, "%d", 8080);
				break;
			case 20:
				sprintf(port, "%d", 8081);
				break;
		}
		
		if ((ret = getaddrinfo(ip, port, &hints, &res)) != 0)
		{
			fprintf(stderr, " %sGetAddrInfo Fehler: %s\n", KRED, gai_strerror(ret));
			return -1;
		}

		for (record = res; record != NULL; record = record->ai_next)
		{
			status = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
			if (status == -1) continue; 
			
			int s_port = atoi( port );
			
			sp = getservbyport( htons(s_port), NULL );

			setsockopt(status, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);
			if (connect(status, record->ai_addr, record->ai_addrlen) != -1)
			{
				if (sp)
				{
					Uppercase(sp->s_name);
					printf(" %s[+] %s OFFEN [%s] \n", KGRN, port, sp->s_name);
					
					p_offen = p_total - p_geschlossen;
					offen_ports[p_offen] = s_port;
					
					p_geschlossen--;				
					close(status);
					break;
				}
				else
				{
					printf(" %s[+] %s OFFEN \n", KGRN, port);
										
					p_offen = p_total - p_geschlossen;
					offen_ports[p_offen] = s_port;
					
					p_geschlossen--;				
					close(status);
					break;
				}
			}
			close(status);
		}
	}
	
	p_offen = p_total - p_geschlossen;
	
	if(p_offen == 0)
	{
		printf("\n%s %d gescannt : Kein geöffneter Port wird erkannt. \n", KRED, p_total);
	}
	else
	{
		printf("\n%s %d Port(s) gescannt : %d offen, %d geschlossen. \n \n", KGRN, p_total, p_offen, p_geschlossen);
		
		int a;
		printf(" %sOffen Port(s) : %s\n", KYEL, KWHT);
		
		for(a = 0; a < p_offen; a++){
			
			sp = getservbyport( htons(offen_ports[a]), NULL );
			Uppercase(sp->s_name);
			
			if (a + 1 == p_offen)
			{
				printf("   └──%d [%s] \n", offen_ports[a], sp->s_name);
			}
			else
			{
				printf("   ├──%d [%s] \n", offen_ports[a], sp->s_name);
			}	
		}
	}
	
	char g_otp[5];
	
	printf("\n %3sSpeichern ? [J/N] >> ", KYEL);
	scanf("%s", &g_otp);

	if( strcmp ("J", g_otp) == 0 || strcmp ("j", g_otp) == 0)
	{
		char otp[100];
		sprintf(otp, "%s.txt", ip);
	
		FILE *f = fopen(otp, "w");
		
		if (f == NULL)
		{
		    printf("%s Fehler beim Speichern der Ausgabe.\n", KRED);
		    exit(1);
		}
		
		fprintf(f, "%s ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n", KYEL);
		fprintf(f, "%s                SCHNELL SCAN SUB-MODUL              \n", KWHT);
		fprintf(f, "%s ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n", KYEL);

		fprintf(f, "\n%s ────────────────────────────────────────────────── \n", KYEL);
		fprintf(f, " %s Scan-Typ : Schnell Scan \n", KWHT);
		fprintf(f, " %s Scan Ziel : %s \n", KWHT, ip);
		fprintf(f, " %s Gescannte Ports : 21, 22, 23, 25, 53, 80, 110,\n  111, 135, 139, 143, 443, 445, 993, 995, 1723,\n  3306, 3389, 5900, 8080, 8081\n", KWHT);
		fprintf(f, "%s ────────────────────────────────────────────────── \n \n", KYEL, KWHT);
		
		if(p_offen == 0)
		{
			fprintf(f, "%s %d gescannt : Kein geöffneter Port wird erkannt. \n", KRED, p_total);
		}
		else
		{
			fprintf(f, "%s %d Port(s) gescannt : %d offen, %d geschlossen. \n \n", KGRN, p_total, p_offen, p_geschlossen);
			
			int a;
			fprintf(f, " %sOffen Port(s) : %s\n", KYEL, KWHT);
			
			for(a = 0; a < p_offen; a++){
				
				sp = getservbyport( htons(offen_ports[a]), NULL );
				Uppercase(sp->s_name);
				
				if (a + 1 == p_offen)
				{
					fprintf(f, "   └──%d [%s] \n", offen_ports[a], sp->s_name);
				}
				else
				{
					fprintf(f, "   ├──%d [%s] \n", offen_ports[a], sp->s_name);
				}	
			}
		}
		
		fclose(f);
		
		printf("\n %sAusgabe gespeichert als : %s \n", KWHT, otp);
	}

	freeaddrinfo(res);
	return 0;
}

int S_Menu()
{
	printf("\n");
	
	printf("%s ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n", KYEL);
	printf("%s                SCHNELL SCAN SUB-MODUL              \n", KWHT);
	printf("%s ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ \n \n", KYEL);

	char g_ip[100];
	char g_verbose[5];
	int g_timeout;
	int g_e_port;
	int g_l_port;

	printf("%s IP Adresse >> ", KWHT);
	scanf("%100s", &g_ip);

	printf("%s Verbose [J/N] >> ", KWHT);
	scanf("%5s", &g_verbose);
	
	if( strcmp (g_verbose, "J") == 0 || strcmp (g_verbose, "j") == 0)
	{
		S_Scan_V(g_ip);
	}
	else
	{
		S_Scan_NV(g_ip);
	}

	return 0;
}
#endif
