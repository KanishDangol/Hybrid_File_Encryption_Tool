#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"
#include "hybrid.h"

GtkWidget *input_entry;
GtkWidget *status_label;

void on_encrypt_clicked(GtkWidget *widget, gpointer data) {
    const char *input_file = gtk_entry_get_text(GTK_ENTRY(input_entry));
    if (strlen(input_file) == 0) {
        gtk_label_set_text(GTK_LABEL(status_label), "Please select a file to encrypt.");
        return;
    }

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save Encrypted File As",
        GTK_WINDOW(gtk_widget_get_toplevel(widget)),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        ("_Cancel"), GTK_RESPONSE_CANCEL,
        ("_Save"), GTK_RESPONSE_ACCEPT,
        NULL);

    char *suggested_name = g_strdup_printf("%s.enc", input_file);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), g_path_get_basename(suggested_name));
    g_free(suggested_name);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *output_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // Dummy RSA values (not used)
        mpz_t e, n;
        mpz_inits(e, n, NULL);
        mpz_set_ui(e, 65537);
        mpz_set_ui(n, 123456789);

        hybrid_encrypt(input_file, output_file, e, n);
        gtk_label_set_text(GTK_LABEL(status_label), "File encrypted successfully.");

        g_free(output_file);
        mpz_clears(e, n, NULL);
    }

    gtk_widget_destroy(dialog);
}

void on_decrypt_clicked(GtkWidget *widget, gpointer data) {
    const char *input_file = gtk_entry_get_text(GTK_ENTRY(input_entry));
    if (strlen(input_file) == 0) {
        gtk_label_set_text(GTK_LABEL(status_label), "Please select a file to decrypt.");
        return;
    }

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save Decrypted File As",
        GTK_WINDOW(gtk_widget_get_toplevel(widget)),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        ("_Cancel"), GTK_RESPONSE_CANCEL,
        ("_Save"), GTK_RESPONSE_ACCEPT,
        NULL);

    char *suggested_name = g_strdup_printf("%s.dec", input_file);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), g_path_get_basename(suggested_name));
    g_free(suggested_name);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *output_file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        mpz_t d, n;
        mpz_inits(d, n, NULL);
        mpz_set_ui(d, 123456789);
        mpz_set_ui(n, 123456789);

        hybrid_decrypt(input_file, output_file, d, n);
        gtk_label_set_text(GTK_LABEL(status_label), "File decrypted successfully.");

        g_free(output_file);
        mpz_clears(d, n, NULL);
    }

    gtk_widget_destroy(dialog);
}

void on_browse_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
        GTK_WINDOW(gtk_widget_get_toplevel(widget)),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        ("_Cancel"), GTK_RESPONSE_CANCEL,
        ("_Open"), GTK_RESPONSE_ACCEPT,
        NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gtk_entry_set_text(GTK_ENTRY(input_entry), filename);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Encryption Program");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *title_label = gtk_label_new("File Encryption Program");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 3, 1);

    GtkWidget *file_label = gtk_label_new("File:");
    gtk_grid_attach(GTK_GRID(grid), file_label, 0, 1, 1, 1);

    input_entry = gtk_entry_new();
    gtk_widget_set_hexpand(input_entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), input_entry, 1, 1, 1, 1);

    GtkWidget *browse_button = gtk_button_new_with_label("Browse");
    g_signal_connect(browse_button, "clicked", G_CALLBACK(on_browse_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), browse_button, 2, 1, 1, 1);

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_grid_attach(GTK_GRID(grid), button_box, 0, 2, 3, 1);  // Span 3 columns to center

    GtkWidget *encrypt_button = gtk_button_new_with_label("Encrypt");
    GtkWidget *decrypt_button = gtk_button_new_with_label("Decrypt");

    gtk_widget_set_size_request(encrypt_button, 120, 40);
    gtk_widget_set_size_request(decrypt_button, 120, 40);

    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(on_encrypt_clicked), NULL);
    g_signal_connect(decrypt_button, "clicked", G_CALLBACK(on_decrypt_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(button_box), encrypt_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), decrypt_button, FALSE, FALSE, 0);
 
    status_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), status_label, 0, 3, 3, 1);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
