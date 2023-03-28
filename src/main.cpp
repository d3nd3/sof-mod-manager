#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

/*

	Be told the SoF directory.

	Analyse the SoF directory.

	Analyse the SoF.exe binary.

	Specify launch options.

	Be told checksum of each file and how it compares.

	Be shown xxd comparions to original

	Request sofree/sof-plus-plus specific integrations

	Show the serverlist before the game is open

	Show size of user dir and directory

	Tools
		Map Editting
		Bsp Parsing
			Entity Table
			Textures Lump
		Pak Editor
		M32 Editor

*/



/*
	A possible TLDR of the last paragraph with every detail is:

- Ownership = memory management responsibility
- Annotations = labels for ownership transfer
- (transfer full) = caller owns object/data ( US/WE )
- (transfer none) = called function owns object/data ( THEY )
- g_object_unref() = free memory of owned object/data ( ONLY ALLOWED ON US )
- you should only call g_object_unref() on an object or a data structure that you own, and that you should not call it on an object or a data structure that you do not own. 

The data is owned by the called function. == THEM

*/

/*
	homogeneous = alike
	// Enable wrapping
	gtk_box_set_homogeneous(GTK_BOX(hbox), FALSE);


	// 					box,child,   		 expand?,  fill?,   padding_len
	gtk_box_pack_start(GTK_BOX(hbox), button1, TRUE,   FALSE, 		0)

	By default, the box will not wrap, so if the widgets are too wide to fit in the available space, they will overflow. To enable wrapping, you can set the homogeneous property of the box to FALSE and the hexpand property of each widget to TRUE. This will allow the widgets to shrink and grow as needed to fit the available space, and wrap to the next row if necessary:


	The correct way to achieve wrapping in a GtkBox layout is to set the halign parameter to GTK_ALIGN_START and the valign parameter to GTK_ALIGN_FILL, as I mentioned earlier.
*/

static void launch_notepad() {
	system("notepad.exe");
}

GtkWidget * stack = NULL;
static void on_window_destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	// Create the main window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "https://github.com/d3nd3/sof-mod-manager");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


	// ---------------------SIGNALS----------------------------

	g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

	stack = gtk_stack_new();
	// Add the stack widget to the main window
	gtk_container_add(GTK_CONTAINER(window), stack);

	PageHome page1("page_home");
	PageAnalysis page2("page_analysis");
	
	// SHOW HOME PAGE
	gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_home");
	// Show the main window and start the main event loop
	gtk_widget_show_all(window);

	gtk_window_maximize(GTK_WINDOW(window)); // maximize the window

	gtk_main();

	return 0;
}

void print_widget_hierarchy(GtkWidget *widget, gint indent) {
	for (gint i = 0; i < indent; i++) {
		g_print("  ");
	}

	g_print("Widget name: %s\n", gtk_widget_get_name(widget));

	if (GTK_IS_CONTAINER(widget)) {
		GList *children, *iter;
		children = gtk_container_get_children(GTK_CONTAINER(widget));
		for (iter = children; iter != NULL; iter = g_list_next(iter)) {
			GtkWidget *child = GTK_WIDGET(iter->data);
			print_widget_hierarchy(child, indent + 1);
		}
		g_list_free(children);
	}
}