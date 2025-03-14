#include <gtk/gtk.h>
#include "aes.h"

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *title_label;
    GtkWidget *input_label, *input_entry;
    GtkWidget *key_label, *key_entry;
    GtkWidget *button_box, *button1, *button2;
    GtkWidget *output_label, *output_entry;

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

    // Input box
    input_label = gtk_label_new("Input:");
    input_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(input_entry), 32);
    gtk_widget_set_hexpand(input_entry, TRUE);

    // Key input box
    key_label = gtk_label_new("Key:");
    key_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(key_entry), 32);
    gtk_widget_set_hexpand(key_entry, TRUE);

    // Button box (for alignment)
    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    // Encrypt & Decrypt buttons
    button1 = gtk_button_new_with_label("Encrypt");
    button2 = gtk_button_new_with_label("Decrypt");

    // Ensure both buttons are the same size
    gtk_widget_set_size_request(button1, 120, 40);
    gtk_widget_set_size_request(button2, 120, 40);

    // Add buttons to the button box for centering
    gtk_box_pack_start(GTK_BOX(button_box), button1, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(button_box), button2, FALSE, FALSE, 10);

    // Output box
    output_label = gtk_label_new("Output:");
    output_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(output_entry), 32);
    gtk_widget_set_hexpand(output_entry, TRUE);
    gtk_editable_set_editable(GTK_EDITABLE(output_entry), FALSE);  // Read-only

    // Attach widgets to the grid
    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), input_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), input_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), key_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), key_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_box, 0, 3, 2, 1);  // Centered button box
    gtk_grid_attach(GTK_GRID(grid), output_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), output_entry, 1, 4, 1, 1);

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
