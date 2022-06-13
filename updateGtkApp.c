#include <gtk/gtk.h>

#define BUFFER 100

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *button;
GtkWidget *label;
GtkWidget *text;

gboolean command(gpointer data){
    FILE *p;
    gchar *output;
    GtkTextBuffer *buffer;

    buffer = gtk_text_buffer_new(NULL) ;

    p = popen("sudo apt update && sudo apt upgrade","r");

    if (p == NULL){
        gtk_label_set_text(GTK_LABEL(label), (const gchar*) "POPEN: Failed to execute command.");
    }
    else {
        int count = 1;
        
        gtk_text_buffer_set_text(buffer,(const gchar*) p, BUFFER);

        text = gtk_text_view_new();
        gtk_text_view_set_buffer(GTK_TEXT_VIEW (text), buffer);
        gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Success Updated!");
    }

    return 0;
}

static void on_button_clicked (GtkButton *button, gpointer data) {  
    g_idle_add(command,NULL);
}

int main(int argc, char **argv) {

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "updater.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "ui_window_update"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder,NULL);

    label = GTK_WIDGET(gtk_builder_get_object(builder, "ui_gtklabel_update"));
    text = GTK_WIDGET(gtk_builder_get_object(builder, "ui_gtktext"));

    button = GTK_WIDGET(gtk_builder_get_object(builder, "ui_gtkbutton_update"));
    g_signal_connect (button, "clicked", G_CALLBACK (on_button_clicked), NULL);

    button = GTK_WIDGET(gtk_builder_get_object (builder, "ui_gtkbutton_close"));
    g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
