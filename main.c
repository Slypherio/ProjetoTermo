#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_TENTATIVAS 6
#define TAM_PALAVRA 5
#define NUM_PALAVRAS 10

typedef struct {
    GtkWidget *entry;
    GtkWidget *label_feedback;
    GtkWidget *window;
    char palavra_secreta[TAM_PALAVRA + 1];
    int tentativas_restantes;
} Jogo;

// Banco de palavras
const char *palavras[NUM_PALAVRAS] = {
    "casa", "bloco", "mundo", "carro", "tempo",
    "futuro", "amigo", "lazer", "livro", "festa"
};

// Função para gerar palavra secreta aleatória
void gerar_palavra_secreta(Jogo *jogo) {
    strcpy(jogo->palavra_secreta, palavras[rand() % NUM_PALAVRAS]);
}

// Função para verificar e exibir feedback
void verificar_tentativa(GtkWidget *widget, gpointer data) {
    Jogo *jogo = (Jogo *)data;
    const char *tentativa = gtk_entry_get_text(GTK_ENTRY(jogo->entry));
    
    // Verifica se tem 5 letras
    if (strlen(tentativa) != TAM_PALAVRA) {
        gtk_label_set_text(GTK_LABEL(jogo->label_feedback), "A palavra deve ter 5 letras!");
        return;
    }

    char feedback[20] = "";
    for (int i = 0; i < TAM_PALAVRA; i++) {
        if (tentativa[i] == jogo->palavra_secreta[i]) {
            feedback[i] = tentativa[i]; // Verde
        } else if (strchr(jogo->palavra_secreta, tentativa[i])) {
            feedback[i] = '?'; // Amarelo (exemplo)
        } else {
            feedback[i] = '_'; // Cinza
        }
    }
    feedback[TAM_PALAVRA] = '\0';

    // Atualiza o feedback na interface
    gtk_label_set_text(GTK_LABEL(jogo->label_feedback), feedback);

    // Verifica se ganhou
    if (strcmp(tentativa, jogo->palavra_secreta) == 0) {
        gtk_label_set_text(GTK_LABEL(jogo->label_feedback), "Parabéns! Você acertou!");
        gtk_widget_set_sensitive(jogo->entry, FALSE);
    } else {
        jogo->tentativas_restantes--;
        if (jogo->tentativas_restantes == 0) {
            char msg[50];
            sprintf(msg, "Fim de jogo! A palavra era: %s", jogo->palavra_secreta);
            gtk_label_set_text(GTK_LABEL(jogo->label_feedback), msg);
            gtk_widget_set_sensitive(jogo->entry, FALSE);
        }
    }

    // Limpa a entrada para próxima tentativa
    gtk_entry_set_text(GTK_ENTRY(jogo->entry), "");
}

// Função para criar a interface
void criar_interface(int argc, char *argv[], Jogo *jogo) {
    gtk_init(&argc, &argv);

    // Criando a janela
    jogo->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(jogo->window), "Jogo Termo");
    gtk_container_set_border_width(GTK_CONTAINER(jogo->window), 20);
    gtk_window_set_default_size(GTK_WINDOW(jogo->window), 300, 200);

    // Layout vertical
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(jogo->window), vbox);

    // Entrada de texto
    jogo->entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(jogo->entry), TAM_PALAVRA);
    gtk_box_pack_start(GTK_BOX(vbox), jogo->entry, FALSE, FALSE, 5);

    // Botão de envio
    GtkWidget *botao = gtk_button_new_with_label("Enviar");
    gtk_box_pack_start(GTK_BOX(vbox), botao, FALSE, FALSE, 5);

    // Label de feedback
    jogo->label_feedback = gtk_label_new("Digite uma palavra de 5 letras");
    gtk_box_pack_start(GTK_BOX(vbox), jogo->label_feedback, FALSE, FALSE, 5);

    // Conectando sinais
    g_signal_connect(botao, "clicked", G_CALLBACK(verificar_tentativa), jogo);
    g_signal_connect(jogo->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrando tudo
    gtk_widget_show_all(jogo->window);
    gtk_main();
}

// Função principal
int main(int argc, char *argv[]) {
    srand(time(NULL));

    Jogo jogo;
    jogo.tentativas_restantes = NUM_TENTATIVAS;
    gerar_palavra_secreta(&jogo);

    criar_interface(argc, argv, &jogo);
    return 0;
}

