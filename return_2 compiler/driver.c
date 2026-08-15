//1.1
#include <unistd.h>
#include <stdio.h>
//1.2
#define _GNU_SOURCE 
#include <string.h> //necessario per strlen()

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
    //fase 1.2: devo produrre return_2.i, pertanto tronco il .c, ovvero gli ultimi 2 caratteri del file di input e li sostituisco con .i tramite asprintf()
    char *input_file = argv[optind];
    char *output_file = NULL;

    int len_truncated = strlen(input_file) -2;
    asprintf(&output_file, "%.*s.i", len_truncated, input_file);
    printf("File prodotto: %s\n", output_file);

    return 0;
}