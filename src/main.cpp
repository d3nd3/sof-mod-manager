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

/*

`configure-event` signal is emitted BEFORE a widget is resized or moved
`size-allocate` signal is emitted AFTER a widget has been resized and allocated an area in its parent widget.

`configure-event` is used to modify the size or position of a widget before it's rendered
`size-allocate` is used to perform any layout or drawing operations based on the widget's new size.

*/

/*
	In short, the size allocation of a GtkBox and its children is determined by several factors. If the homogeneous property of the GtkBox is set to TRUE, all its children will be given equal space. If it’s set to FALSE, the space allocation will depend on the minimum and natural sizes of its children. You can try setting the homogeneous property of the vertical GtkBox to TRUE to ensure that all its children are given equal space.
*/


/*

static void launch_notepad() {
	system("notepad.exe");
}


*/
void MainStyle(void);

int current_width = 0;
int current_height = 0;

GtkWidget * stack = NULL;
static void on_window_destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

static void window_size_alloc_MAIN (GtkWidget *widget, GtkAllocation *allocation, gpointer user_data)
{
	gtk_window_get_size (GTK_WINDOW (widget), &current_width, &current_height);
}

static void window_size_alloc_SWITCHER (GtkWidget *widget, GtkAllocation *allocation, gpointer user_data)
{
	GtkWidget *stack_switcher = (GtkWidget *) user_data;

	int switch_height = current_height * 0.1;
	// cout << "Setting Switcher height " << switch_height << endl;
	gtk_widget_set_size_request(stack_switcher,-1,switch_height);
}

static gboolean on_window_draw (GtkWidget* self, cairo_t* cr, gpointer user_data ) 
{
	cout << "LETS DRAW!!!" << endl;
	cout << "width : " << current_width << " height : " << current_height << endl;
	return FALSE;
}
#if 0
static gboolean on_configure_switcher (GtkWidget* self, GdkEventConfigure event, gpointer user_data)
{
	cout << "Switcher Configure Event" << endl;
	
	return FALSE;
}
#endif

void loadCssFile(char * cssfile)
{
	// Create a new GtkCssProvider object
	GtkCssProvider *provider = gtk_css_provider_new();
	// Load the CSS file
	GFile *file = g_file_new_for_path(cssfile);
	gtk_css_provider_load_from_file(provider, file, NULL);

	// Apply the CSS theme to the default screen
	GdkScreen *screen = gdk_screen_get_default();
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	g_object_unref(file);
}


void search_themes_in_dir(const gchar *path, std::vector<std::string> &themes) {
	GDir *dir = g_dir_open(path, 0, NULL);
	if (dir != NULL) {
		const gchar *name;
		while ((name = g_dir_read_name(dir)) != NULL) {
			std::string theme_path = std::string(path) + "/" + name + "/gtk-3.0";
			if (g_file_test(theme_path.c_str(), G_FILE_TEST_IS_DIR)) {
				themes.push_back(name);
			}
		}
		g_dir_close(dir);
	}
}

std::vector<std::string> get_available_themes() {
	char **builtin_themes;
	guint i;
	gchar *path;
	const gchar * const *dirs;

	/* Builtin themes */
	builtin_themes = g_resources_enumerate_children ("/org/gtk/libgtk/theme", 0, NULL);
	std::vector<std::string> themes;
	for (i = 0; builtin_themes[i] != NULL; i++)
	{
		if (g_str_has_suffix (builtin_themes[i], "/"))
			themes.push_back(std::string(builtin_themes[i], strlen (builtin_themes[i]) - 1));
	}
	g_strfreev (builtin_themes);

	// Search for themes in the system data directories
	dirs = g_get_system_data_dirs();
	for (i = 0; dirs[i]; i++)
	{
		path = g_build_filename (dirs[i], "themes", NULL);
		search_themes_in_dir(path, themes);
		g_free(path);
	}

	// Search for themes in the user's data directory
	path = g_build_filename(g_get_user_data_dir(), "themes", NULL);
	search_themes_in_dir(path, themes);
	g_free(path);

	// Search for themes in the user's home directory
	path = g_build_filename(g_get_home_dir(), ".themes", NULL);
	search_themes_in_dir(path, themes);
	g_free(path);

	// Search for themes in the directory specified by GTK_DATA_PREFIX
	const gchar *data_prefix = g_getenv("GTK_DATA_PREFIX");
	if (data_prefix != NULL) {
		path = g_build_filename(data_prefix, "share", "themes", NULL);
		search_themes_in_dir(path, themes);
		g_free(path);
	}

	return themes;
}

void on_theme_changed(GtkComboBox *widget, gpointer user_data) {
	GtkSettings *settings = gtk_settings_get_default();
	gchar *theme_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
	g_object_set(settings, "gtk-theme-name", theme_name, NULL);
	g_free(theme_name);
}

int main(int argc, char *argv[]) {
	// GTK_THEME=Adwaita:dark
	// putenv("GTK_CSD=0");
	g_setenv("GTK_DATA_PREFIX", "./gtk", TRUE);
	gtk_init(&argc, &argv);

	// starting theme
	GtkSettings *settings = gtk_settings_get_default();
	g_object_set(settings, "gtk-theme-name", "hackerer-main", NULL);


	// Create the main window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);
	g_signal_connect(window,"size-allocate",G_CALLBACK(window_size_alloc_MAIN),NULL);


	// g_signal_connect(window,"draw",G_CALLBACK(on_window_draw),NULL);
	gtk_window_maximize(GTK_WINDOW(window)); // maximize the window
	
	gtk_window_set_default_size(GTK_WINDOW(window), 800,600);
	gtk_window_set_title(GTK_WINDOW(window), "https://github.com/d3nd3/sof-mod-manager");


	stack = gtk_stack_new();
	
	gtk_stack_set_transition_type(GTK_STACK(stack),GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	gtk_stack_set_transition_duration(GTK_STACK(stack),1000);

	PageHome page1("page_home");
	PageAnalysis page2("page_analysis");


	// --------------------ALL---------------------------
	GtkWidget * all = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_widget_set_name(all,"manager");
	gtk_container_add(GTK_CONTAINER(window),all);


	// ------------------------THEMES--------------------------------
	GtkWidget *combo_box = gtk_combo_box_text_new();
	std::vector<std::string> themes = get_available_themes();
	for (const std::string &theme : themes) {
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), theme.c_str());
	}
	g_signal_connect(combo_box, "changed", G_CALLBACK(on_theme_changed), NULL);

	// THEME OUTER_FRAME
	GtkWidget *outer_frame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(outer_frame),combo_box,FALSE,FALSE,0);
	gtk_stack_add_titled(GTK_STACK(stack), outer_frame, "themes", "THEMES");


	
	// SHOW HOME PAGE
	gtk_stack_set_visible_child_name(GTK_STACK(stack), "page_home");


	//----------------------------------------TOP BAR-----------------------------------------
	// all -> sw_container[switcher],stack
	GtkWidget * sw_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_widget_set_name(sw_container,"switch_container");
	gtk_box_set_homogeneous(GTK_BOX(sw_container),TRUE);


	GtkWidget * w_lab = gtk_label_new("SoF1 Mod Manager!");
	gtk_box_pack_start(GTK_BOX(sw_container),w_lab,TRUE,TRUE,0);
	gtk_label_set_justify(GTK_LABEL(w_lab),GTK_JUSTIFY_LEFT);
	gtk_widget_set_halign(w_lab,GTK_ALIGN_START);
	gtk_widget_set_valign(w_lab,GTK_ALIGN_CENTER);
	
	
	// gtk_label_set_xalign(GTK_LABEL(w_lab),0.0);

	GtkWidget * switcher = gtk_stack_switcher_new();
	gtk_box_pack_start(GTK_BOX(sw_container),switcher,TRUE,TRUE,0);

	// gtk_label_set_xalign(switcher,0.5);
	gtk_widget_set_halign(switcher,GTK_ALIGN_CENTER);
	gtk_widget_set_valign(switcher,GTK_ALIGN_CENTER);


	// third fake item to allow middle item to be centered
	GtkWidget* w_fake = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_pack_start(GTK_BOX(sw_container),w_fake,TRUE,TRUE,0);
	

	// g_signal_connect(window,"size-allocate",G_CALLBACK(window_size_alloc_SWITCHER),switcher);
	gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher),GTK_STACK(stack));

	// box,child,   		 consume-free-space?, free-space-resizes-me?,   padding_len
	// TabSwitcher(Menu) at top.
	// INTO VERTICAL BOX
	gtk_box_pack_start(GTK_BOX(all),sw_container,FALSE,FALSE, 0 );
	gtk_box_pack_start(GTK_BOX(all),stack,TRUE,TRUE, 0 );


	MainStyle();

	cout << "START width : " << current_width << " height : " << current_height << endl;

	// show
	gtk_widget_show_all(window);
	// event loop
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

/*
https://docs.gtk.org/gtk3/css-properties.html
https://docs.gtk.org/gtk3/css-overview.html#selectors

opacity: 0;

:backdrop is when its faded when hasn't got focus

background-color - applies to padding

background-image: none

[content / padding] / margin
margin outside
*/
void MainStyle(void)
{
	string style = R"(
		combobox {
			outline: none;
		}
		button {
			outline: none;
		}
		#manager>box {
			border-bottom-style: double;
			border-bottom-width: 12px;
		}
		#manager box:backdrop, #manager button:backdrop {
			border-color: shade(alpha(currentColor,0.32),0.8);
		}

		stackswitcher button:nth-child(even) {
			background-clip: padding-box;
			border-left-style: dotted;
			border-left-color: #00FF00;
			border-left-width: 8px;

			border-right-style: dotted;
			border-right-color: #00FF00;
			border-right-width: 8px;
		}

		stackswitcher button {
			outline: none;
			border: none;
			box-shadow: none;
		}

		window {
			font-size: 20px;
		}

		stackswitcher {
			font-size: 1.5em;
		}

		#switch_container>label {
			font-size: 1.5em;
		}

		tooltip { 
			font-size: 1.5em;
		}

	)";

	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider, style.c_str(), -1, NULL);

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}