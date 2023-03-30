#include <gtk/gtk.h>
#include "common.h"

void on_comboboxtext_changed(GtkComboBoxText *combo_box_text, gpointer user_data);
static void on_delete_clicked(GtkButton *button, gpointer data)
{
	
	GtkComboBoxText *combo_box_text = GTK_COMBO_BOX_TEXT(data);
	gtk_combo_box_text_remove(combo_box_text,gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box_text)));
	gtk_combo_box_set_active(GTK_COMBO_BOX( combo_box_text),0);

	
}



void on_comboboxtext_changed(GtkComboBoxText *combo_box_text, gpointer user_data) {
	const gchar *selected_text = gtk_combo_box_text_get_active_text(combo_box_text);
	if (g_strcmp0(selected_text, "<...new...>") == 0) {
		gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
		GtkWidget *dialog;
		GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
		gint res;

		dialog = gtk_file_chooser_dialog_new("Select a folder", NULL, action,
											 "Cancel", GTK_RESPONSE_CANCEL,
											 "Select", GTK_RESPONSE_ACCEPT, NULL);
		res = gtk_dialog_run(GTK_DIALOG(dialog));
		if (res == GTK_RESPONSE_ACCEPT) {
			gchar *folder_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			if (folder_path != NULL) {
				// Check if the folder_path is valid here, using e.g. g_file_test
				// If the folder_path is not valid, show an error message to the user and return
				// Otherwise, append the new item to the combo box and activate the button

				// Append the new item to the combo box
				gtk_combo_box_text_prepend_text(combo_box_text, folder_path);

				// This triggers this function again.
				gtk_combo_box_set_active(GTK_COMBO_BOX( combo_box_text),0);

				
				g_free(folder_path);
			}
		}

		gtk_widget_destroy(dialog);
	} else
	if ( g_strcmp0(selected_text, "" ) == 0 ) {
		gtk_widget_set_sensitive(GTK_WIDGET(user_data), FALSE);
	} else
		gtk_widget_set_sensitive(GTK_WIDGET(user_data), TRUE);
	g_free(selected_text);
}





// This callback function shows the tooltip of the button when it is clicked
void show_tooltip (GtkWidget *button, gpointer data)
{
  // Get the tooltip text from the button
  const gchar *tooltip = gtk_widget_get_tooltip_text (button);

  // Create a message dialog to display the tooltip
  GtkWidget *dialog = gtk_message_dialog_new (NULL,
											  GTK_DIALOG_MODAL,
											  GTK_MESSAGE_INFO,
											  GTK_BUTTONS_OK,
											  "%s",
											  tooltip);


	// Get the message area widget of the dialog
	GtkWidget *message_area = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog));

	// Get the label widget that displays the message text
	GList *children = gtk_container_get_children(GTK_CONTAINER(message_area));
	GtkWidget *label = GTK_WIDGET(children->data);
	g_list_free(children);

		// Create a new font description with the desired size

	// Create a new Pango font description with the desired size
	PangoFontDescription *font_desc = pango_font_description_new();
	pango_font_description_set_size(font_desc, 24 * PANGO_SCALE);

	// Set the new font for the label widget
	gtk_widget_override_font(label, font_desc);

	// Show the dialog and wait for a response
	gtk_dialog_run (GTK_DIALOG (dialog));

	// Destroy the dialog
	gtk_widget_destroy (dialog);
}


// box,child,expand?,fill?,padding_len
/*
	When ALIGNMENT is set, it turns FILL OFF
	Alignment requires Expand to be ON. Because it uses PADDING to align?

	EXPAND WHEN YOU WANT TO CENTER.:/
*/
void PageHome::Content(void)
{
	/*
		outer_frame ->
			hbox1 - instance_edit
				vbox
					welcome_text
					combobox_house
			hbox2
			hbox3
	*/

	GtkWidget * w_hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_widget_set_name(w_hbox1,"instance_edit");
	GtkWidget * w_hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget * w_hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);


	// --------------------w_vbox_welcome---------------------
	GtkWidget * w_vbox_welcome = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);


	GtkWidget * w_combo_house = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
		GtkWidget * w_lab_combo = gtk_label_new("SoF Instance:");

		gtk_widget_set_name(w_combo_house,"combo_house");
		gtk_widget_set_halign(w_combo_house,GTK_ALIGN_CENTER);
		// Create a new button with an "i" label
		GtkWidget *w_infobut = gtk_button_new_with_label("ℹ");
		gtk_widget_set_name(w_infobut,"tooltip_info_button");

		gtk_widget_set_tooltip_text(w_infobut, "This program can manage multiple SoF directories.\nUse the dropdown box to track and untrack instances.\nThese will be remembered for next time.");
		
		// gtk_widget_set_sensitive(w_infobut, FALSE);
		g_signal_connect (w_infobut, "clicked", G_CALLBACK (show_tooltip), NULL);

		// Create the combo box
		GtkWidget *w_combo = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w_combo), "");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(w_combo), "<...new...>");
		
		GtkWidget *delete_button = gtk_button_new_with_label("Untrack");
		// Connect the "changed" signal
		g_signal_connect(w_combo, "changed", G_CALLBACK(on_comboboxtext_changed), delete_button);
		

		
		gtk_widget_set_sensitive(delete_button, FALSE);

		g_signal_connect (delete_button, "clicked", G_CALLBACK (on_delete_clicked), w_combo);

		gtk_box_pack_start(GTK_BOX(w_combo_house), delete_button, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(w_combo_house),w_lab_combo,FALSE,FALSE,0);
		gtk_box_pack_start(GTK_BOX(w_combo_house),w_combo,FALSE,FALSE,0);
		gtk_box_pack_start(GTK_BOX(w_combo_house),w_infobut,FALSE,FALSE,0);


	
	gtk_box_pack_start(GTK_BOX(w_vbox_welcome),w_combo_house,FALSE,FALSE,0);

	// The mod manager can manage multiple sof directories, they are like profiles.
	// So I need a comboBox to select the current directory managed.
	// With perhaps an entry in the combo box to add a new one, at the bottom?
	

	// ------------------hbox1 contents-----------------------
	gtk_box_pack_start(GTK_BOX(w_hbox1), w_vbox_welcome,TRUE,TRUE,0);

	// place the hboxes into the outer_frame
	// I want them to expand and fill so that they behave like equal dividers
	gtk_box_pack_start(GTK_BOX(outer_frame),w_hbox1,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(outer_frame),w_hbox2,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(outer_frame),w_hbox3,TRUE,TRUE,0);

}


PageHome::PageHome(std::string name) {

	css_name = name;

	//------------------OUTER FRAME---------------------
	// gtk_widget_add_css_class (button, "fancy");
	outer_frame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_name(outer_frame, css_name.c_str());
	gtk_box_set_homogeneous(GTK_BOX(outer_frame),TRUE);

	//--------------------CONTENT-----------------------
	Content();

	
	// Add to Stack
	gtk_stack_add_titled(GTK_STACK(stack), outer_frame, css_name.c_str(), "HOME");
	// STYLE
	PageStyle();
	
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
		window { 
			
		}

		#page_home>box:backdrop {
			border-color: shade(alpha(currentColor,0.32),0.8);
		}
		#page_home>box:nth-child(1) {
				border-style: dashed;
				border-bottom-width: 5px;
		}
		#page_home>box:nth-child(2) {
				border-style: dashed;
				border-bottom-width: 5px;
		}
		#page_home>box:nth-child(3) {
				border-style: dashed;

		}
		#welcome_text {
			margin-top: 6px;
		}

		#tooltip_info_button {
			opacity: 1;
		}
		#tooltip_info_button>label {
			color: #00FF00;
		}

		tooltip {
			color: #FFFF00;
			border: solid 1px #FFFF00;
		}

		#combo_house{
			margin-top: 10px
		}

		#instance_edit window, #instance_edit combobox {
			min-width: 800px;
		}

		#combo_house>label {
			margin-right: 10px;
			margin-left: 10px;

		}

	)";

	// what
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider, style.c_str(), -1, NULL);

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}