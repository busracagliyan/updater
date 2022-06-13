#include <gtk/gtk.h>

#define BUFFER 100

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *button;
GtkWidget *label;
GtkWidget *text;
GtkTextBuffer *buffer;

gboolean command(gpointer data){
    GPid pid;
    gchar *cmd[] = {"/usr/bin/sudo", "apt update", "apt upgrade", "--args",NULL};
    gint in, out, err;
    GIOChannel *out_ch, *err_ch;
    gboolean ret;
    g_autoptr(GError) error = NULL;
    

    ret = g_spawn_async_with_pipes( NULL, cmd, NULL,
                                G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_FILE_AND_ARGV_ZERO, NULL,
                                NULL, &pid, &in, &out, &err, &error );

    if (error != NULL){
        g_error ("Spawning child failed: %s", error->message);
        return 1;
    }
    else {
        gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Successfully Updated!");
    }

    return 0;
}

static void on_button_clicked (GtkButton *button, gpointer data) {  
    g_idle_add(command,data);
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
