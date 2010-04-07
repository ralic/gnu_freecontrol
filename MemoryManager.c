/*
    MemoryManager is part of FreeControl Package
    Copyright (C) 2009, 2010  Quique Rodiguez <quesoruso74@yahoo.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#define LOCK_FILE	"MemoryManager.lock"
#define LOG_FILE	"MemoryManager.log"

#include "Common.c"


char *shmaddr;
int shared_mem_id;
int key_proc;
char* name;

struct mymsgbuf {
	long mtype;
	char name[50];
	int shmid;
	int shmsize;
	char mtext[10];
};

int view_stat() {
	printf("Printing status.... \n\n");	

	}

int ending() {
	printf("Ending... \n");
	shmdt(shmaddr);
	shmctl(shared_mem_id, IPC_RMID, 0);
	}

main(int argc, char* argv[]) {

	program_name=argv[0];
	print_copyright();

	if (argc<=1)
	{
		print_usage();
		return(0);
	}

	key_proc = -1;
	int next_option;

	const char* const short_options = "lrskvhn:p:";

	const struct option long_options[] = {
		{ "load", 0, NULL, 'l' },
		{ "run", 0, NULL, 'r' },
		{ "stop", 0, NULL, 's' },
		{ "kill", 0, NULL, 'k' },
		{ "view", 0, NULL, 'v' },
		{ "help", 0, NULL, 'h' },
		{ "name", 1, NULL, 'n' },
		{ "proc", 1, NULL, 'p' },
		{ NULL, 0, NULL, 0 }
		};
	printf("------------------------------\n");

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		printf("--%d \n", next_option);
		switch (next_option)
		{
		case 'l':
			printf("Option -l\n");
			
			break;
		case 'r':
			printf("Option -r\n");
			return(0);
			break;
		case 's':
			printf("Option -s\n");
			return(0);
			break;
		case 'k':
			printf("Option -k\n");
			return(0);
			break;
		case 'v':
			printf("Option -v\n");
			return(0);
			break;
		case 'h':
			print_usage();
			return(0);
		case 'n':
			name = optarg;
			printf("Name = %s\n", name);
			break;
		case 'p':
			key_proc = atoi(optarg);
			printf("Key_proc= %d\n",key_proc);
			break;
		case '?':
			print_usage();
			return(1);
		}
	}
	while (next_option != -1);
	
	daemonize();

	int i;
	struct mymsgbuf qbuf;
	long type;
	int cant;

	//FILE * Archivo;
	//Archivo=fopen("MM.dbg","w");
	//fprintf(Archivo, "Starting MemoryManager\n");

	key_t key;

	//   Get the main key 'A'

	key = ftok(".",'A');

	if (key == -1)  {
		//fprintf(Archivo, "Couldn't open the main key.\n");
		return (1);
		}
	//   Open or create the main message queue
	int msgqueue_id;
	if ((msgqueue_id = msgget(key, IPC_CREAT)) ==-1)
	{
		printf("Could not open the main message queue\n");
		return(1);
	}

	key = key_proc;
	if (key == -1) {
		key = ftok(".",'D');
		}

	if (key == -1)  {
		printf("Couldn't open the main key.\n");
		return (1);
		}
	//   Open or create the main message queue
	if ((shared_mem_id = shmget(key, 1000, IPC_CREAT | IPC_EXCL | 0660)) ==-1)
	{
		printf("Could not open the shared memory\n");
		//return(1);
	}


	shmaddr = shmat(shared_mem_id,0,0);


	type = 1;
	qbuf.mtype = type;
	strcpy(qbuf.name, name);
	qbuf.shmid = shared_mem_id;
	qbuf.shmsize = 1000;
	char* text;
	text="OK";
	//strcpy(qbuf.mtext, &text);
	qbuf.mtext[0]='O';
	qbuf.mtext[1]='K';
	qbuf.mtext[2]=0;
	sleep(5);
	printf("Sending...%d - %s\n", qbuf.mtype, qbuf.mtext);	
	
	int error;
	error =	msgsnd(msgqueue_id, &qbuf, sizeof(qbuf)-4, 0);

	printf("Response: %d \n", error);
	//cant = msgrcv(n+1,  &qbuf, strlen(qbuf.mtext), type, 0);
	//fclose(Archivo);

	struct timespec interval_t;

	interval_t.tv_sec=0;
	interval_t.tv_nsec=1000;

	for (;;) {
		//nanosleep(interval_t);
		sleep(10);
		printf("msgqueue_id = %d - shared_mem_id = %d - %d \n",msgqueue_id, shared_mem_id, shmaddr[0] );
		printf("type= %d - text= %s \n",qbuf.mtype, qbuf.mtext);
		shmaddr[0]++;
		//error =	msgsnd(msgqueue_id, &qbuf, 3, 0);

		printf("Response: %d \n", error);
	
		
		}
	}
