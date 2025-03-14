#include <gtk/gtk.h>

// Callback function to toggle key visibility
void toggle_visibility(GtkWidget *button, gpointer entry) {
    gboolean visible = gtk_entry_get_visibility(GTK_ENTRY(entry));
    gtk_entry_set_visibility(GTK_ENTRY(entry), !visible);

    // Change the button label based on visibility
    gtk_button_set_label(GTK_BUTTON(button), visible ? "Show" : "Hide");
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *title_label;
    GtkWidget *plaintext_label, *plaintext_entry;
    GtkWidget *key_label, *key_entry, *toggle_button;
    GtkWidget *button1, *button2;

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "AES");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Title label
    title_label = gtk_label_new("AES Implementation");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);

    // Plaintext input
    plaintext_label = gtk_label_new("Plaintext:");
    plaintext_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(plaintext_entry), 32);
    gtk_widget_set_hexpand(plaintext_entry, TRUE);  // Expand to match width

    // Key input
    key_label = gtk_label_new("Key:");
    key_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(key_entry), 32);
    gtk_entry_set_visibility(GTK_ENTRY(key_entry), FALSE);  // Default: hidden
    gtk_widget_set_hexpand(key_entry, TRUE);  // Expand to match width

    // Toggle visibility button
    toggle_button = gtk_button_new_with_label("Show");
    g_signal_connect(toggle_button, "clicked", G_CALLBACK(toggle_visibility), key_entry);

    // Encrypt & Decrypt buttons
    button1 = gtk_button_new_with_label("Encrypt");
    button2 = gtk_button_new_with_label("Decrypt");

    // Attach widgets to the grid
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), plaintext_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), plaintext_entry, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), key_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), key_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), toggle_button, 2, 2, 1, 1);  // Button next to key field
    gtk_grid_attach(GTK_GRID(grid), button1, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button2, 1, 3, 1, 1);

    // Center the grid in the window
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    // Connect the close event
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
