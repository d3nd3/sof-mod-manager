#include <gtk/gtk.h>
#include "common.h"

void on_file_button_clicked(GtkButton *button, gpointer user_data) {
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	gint res;

	dialog = gtk_file_chooser_dialog_new("Open Folder", NULL, action, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

	gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

	res = gtk_dialog_run(GTK_DIALOG(dialog));

	if (res == GTK_RESPONSE_ACCEPT) {
		char *selected;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		selected = gtk_file_chooser_get_filename(chooser);
		// Do something with the directory path
		cout << "path selected is " << selected;

		gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_analysis");

		g_free(selected);
	}

	gtk_widget_destroy(dialog);
}


void PageHome::Content(void)
{
	GtkWidget *file_button;
	file_button = gtk_button_new_with_label("Open File");
	g_signal_connect(file_button, "clicked", G_CALLBACK(on_file_button_clicked), NULL);

	// box,child,expand?,fill?,padding_len
	gtk_box_pack_start(GTK_BOX(outer_frame), file_button,FALSE,FALSE,0);

}


PageHome::PageHome(std::string name) {

	css_name = name;

	//------------------OUTER FRAME---------------------
	// gtk_widget_add_css_class (button, "fancy");
	outer_frame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_name(outer_frame, css_name.c_str());

	//--------------------CONTENT-----------------------
	Content();

	
	// Add to Stack
	gtk_stack_add_titled(GTK_STACK(stack), outer_frame, css_name.c_str(), "HOME PAGE");
	// STYLE
	// PageStyle();
	
}

PageHome::~PageHome(void) {
	cout << "destructor called\n";
}

/*
https://docs.gtk.org/gtk3/css-properties.html
https://docs.gtk.org/gtk3/css-overview.html#selectors
*/
void PageHome::PageStyle(void) {

	string style = R"(
		#page_home {
		    background-color: #99e6ff;
		}
	)";

	// what
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider, style.c_str(), -1, NULL);

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}