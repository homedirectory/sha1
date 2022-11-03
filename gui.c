#include <gtk/gtk.h>
#include "hash.h"

GtkWidget* digestLabel;

static void set_margin(GtkWidget* widget, int margin) {
    gtk_widget_set_margin_top(widget, margin);
    gtk_widget_set_margin_bottom(widget, margin);
    gtk_widget_set_margin_start(widget, margin);
    gtk_widget_set_margin_end(widget, margin);
}

static void on_click_calculate_hash(GtkWidget* btn, gpointer user_data) {
    const char* text = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(user_data));

    char sha1_hash[20] ;
    sha1(text, strlen(text), sha1_hash);
    char sha1_str[41];
    sha1_to_str(sha1_hash, sha1_str);

    gtk_label_set_label(GTK_LABEL(digestLabel), sha1_str);
}

static void activate (GtkApplication* app, gpointer user_data) {
    GtkWidget *window;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "SHA1 Calculator");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(box), 20);
    gtk_window_set_child(GTK_WINDOW(window), box);

    GtkWidget* textInput = gtk_text_new();
    gtk_text_set_placeholder_text(GTK_TEXT(textInput), "Text...");
    set_margin(textInput, 15);
    gtk_box_append(GTK_BOX(box), textInput);

    GtkWidget* btn = gtk_button_new_with_label("Calculate");
    set_margin(btn, 15);
    g_signal_connect(btn, "clicked", G_CALLBACK(on_click_calculate_hash), 
            gtk_text_get_buffer(GTK_TEXT(textInput)));
    gtk_box_append(GTK_BOX(box), btn);

    digestLabel = gtk_label_new("");
    set_margin(digestLabel, 15);
    gtk_box_append(GTK_BOX(box), digestLabel);

    gtk_widget_show (window);
}

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new ("cs.cryptohacker", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
