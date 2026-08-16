#define _GNU_SOURCE //1.2
//1.1
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
//1.2
#include <string.h> //necessario per strlen()
//1.3
#include <stdlib.h> //per la free()
#include <sys/wait.h> //per waitpid

int main(int argc, char *argv[]){
    int opt;
    int stop_at_lex = 0;
    int stop_at_parse = 0;
    int stop_at_codegen = 0;
    int stop_at_assembly = 0;
    //struttura per getopt_long
    static struct option long_options[] = {
        {"lex", no_argument, 0, 'l'},
        {"parse", no_argument, 0, 'p'},
        {"codegen", no_argument, 0, 'c'},
        {0, 0, 0, 0} //terminatore obbligatorio
    };

    //gestore flags
    int option_index = 0;
    while ((opt=getopt_long(argc,argv,"S",long_options,&option_index)) != -1) {
        switch (opt) {
            case 'l':
                stop_at_lex = 1;
                break;
            case 'p':
                stop_at_parse = 1;
                break;
            case 'c':
                stop_at_codegen = 1;
                break;
            case 'S':
                stop_at_assembly = 1;
                break;
            case '?':
                //l'utente ha inserito un flag inesistente
                printf("Uso: %s [--lex] [--parse] [--codegen] [-S] file_input\n", argv[0]);
                return 1;
        }
    }

    //ho processato tutti i flags, ora i file
    if (optind >= argc) {
        printf ("Errore: manca il file di input\n");
        return 1;
    }

    char *input_file = argv[optind];
    printf("File da processare: %s\n", input_file);
    
    //fase 1.2: devo produrre return_2.i, pertanto tronco il .c, ovvero gli ultimi 2 caratteri del file di input e li sostituisco con .i tramite asprintf()
    char *output_file = NULL;

    int len_truncated = strlen(input_file) -2;
    //asprintf scrive nell'indirizzo di output_file la stringa.i, dove stringa sono i primi len_truncated caratteri della stringa input_file
    asprintf(&output_file, "%.*s.i", len_truncated, input_file);
    printf("Nome file .i: %s\n", output_file);

    //fase 1.3: invocazione di GCC tramite fork()+execvp()
    //costruisco l'array di stringhe per execvp()
    char *gcc_args[] = {
        "gcc",       //argv[0] del programma figlio
        "-E",        //ferma gcc dopo il pre-processore
        "-P",        //rimuove i metadati di linea extra dal file generato
        input_file,  
        "-o",       //specifica output
        output_file,
        NULL        //elemento obbligatorio al termine dell'array
    };

    //clono il padre
    pid_t pid = fork();

    if (pid<0){
        perror("Errore durante la fork");
        return 1;
    }

    else if (pid==0){
        //sono nel processo figlio, eseguo il comando di gcc
        execvp("gcc",gcc_args);
        //se ho successo, il figlio diventa il nuovo programma principale e le righe sottostanti non vengono eseguite

        perror("Errore: impossibile lanciare gcc");
        exit(1);
    }

    else {
        //sono nel padre
        //il padre si blocca e attende che il figlio termini, per poi verificare l'esito di execvp()
        int status;
        //per il padre, pid contiene l'ID numerico del figlio creato
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            //uscita volontaria del figlio (è arrivato a fare return o exit)
            int return_code = WEXITSTATUS(status);
            printf("Il figlio è terminato correttamente con codice: %d\n",return_code);

            if (return_code!=0){
                printf("Tuttavia, il comando ha sollevato un errore interno\n");
                //elimino il .i corrotto
                remove(output_file);
                free(output_file);
                return 1;
            }
       }
        else if (WIFSIGNALED(status)){
            //uscita non volontaria
            printf("Il figlio è terminato in modo anomalo\n");
            remove(output_file);
            free(output_file);
            return 1;
        }


    }
    printf("Il padre ha terminato correttamente\n");

    //L'utente vuole fermarsi in anticipo? = L'utente ha inserito dei flags?
    if (stop_at_lex || stop_at_parse || stop_at_codegen){
        remove(output_file);
        free(output_file);
        return 0;
    }
    //creo il nome del file assembly (.s)
    char *assembly_file = NULL;
    asprintf(&assembly_file, "%.*s.s", len_truncated, input_file);
    printf("Nome file assembly: %s\n", assembly_file);

    printf("Esecuzione compilatore STUB\n");
    FILE *fake_s = fopen(assembly_file,"w");
    if (fake_s) fclose(fake_s);

    //free necessaria per evitare memory leakage
    remove(output_file);
    free(output_file);
    free(assembly_file);
    return 0;
}