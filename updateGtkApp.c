#include <gtk/gtk.h>

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *button;
GtkWidget *label;

gboolean idle(gpointer data){
    gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Successfully Updated!");
    return 0;
}

gboolean cb_out_watch(GIOChannel* channel, GIOCondition condition, gpointer data){
    gsize actually_read;
    gchar* shell_out = (gchar*)g_malloc0(512);
    GIOStatus status = g_io_channel_read_chars(channel,shell_out, 512,&actually_read,NULL );
    if(status != G_IO_STATUS_NORMAL){
        g_free(shell_out);
        return TRUE;
    }
    
    printf("%s \n",shell_out);
    g_free(shell_out);

    return TRUE;
}

void child_handler(GPid pid, gint status, gpointer data){
    g_idle_source_new();
    g_idle_add_full(G_PRIORITY_DEFAULT,idle,NULL,NULL);

    g_warning("child killer");
    g_spawn_close_pid(pid);
}

void updater(GtkWidget *widget, gpointer data){
    gchar *command[] = {"/usr/bin/pkexec","apt","update",NULL};
    GPid pid;
    gint  out;
    GIOChannel *out_ch;
    gboolean ret;
    g_autoptr(GError) error = NULL;

    ret = g_spawn_async_with_pipes( NULL, command, NULL,
                                G_SPAWN_DO_NOT_REAP_CHILD, 
                                NULL, NULL, &pid, NULL, &out, NULL, &error );
    if(!ret) {
        printf("Error\n");
        g_error( "SPAWN FAILED" );
    }
    g_spawn_check_exit_status(out,NULL);
    
    out_ch = g_io_channel_unix_new(out);
    g_io_add_watch( out_ch, G_IO_IN | G_IO_ERR | G_IO_HUP, (GIOFunc)cb_out_watch, NULL );
    g_io_channel_unref(out_ch);
    g_child_watch_add(pid , (GChildWatchFunc)child_handler, NULL);

    if (error != NULL) {
        g_error ("Spawning child failed: %s", error->message);
        return;
    }
}

int main(int argc, char **argv) {

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "updater.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "ui_window_update"));

    gtk_builder_connect_signals(builder,NULL);

    label = GTK_WIDGET(gtk_builder_get_object(builder, "ui_gtklabel_update"));

    button = GTK_WIDGET(gtk_builder_get_object(builder, "ui_gtkbutton_update"));
    g_signal_connect (button, "clicked", G_CALLBACK (updater), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(G_OBJECT(builder));
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
