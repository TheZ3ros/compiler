#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    int opt;
    //gestore flags
    while ((opt=getopt(argc,argv,"vo:")) != -1) {
        switch (opt) {
            case 'v':
                printf("Modalità verbosa attivata\n");
                break;
            case 'o':
                printf("File di output: %s\n",optarg);
                break;
            default:
                printf ("Uso: %s [-v] [-o file] file_input\n",argv[0]);
                return 1;
        }
    }

    //ho processato tutti i flags, ora i file
    if (optind >= argc) {
        printf ("Errore: manca il file di input\n");
        return 1;
    }

    printf("File da processare: %s\n", argv[optind]);
    return 0;
}