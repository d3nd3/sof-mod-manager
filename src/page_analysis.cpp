#include <gtk/gtk.h>
#include "common.h"

// box,child,   		 consume-free-space?, free-space-resizes-me?,   padding_len
void PageAnalysis::Content(void)
{
	GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_widget_set_name(hbox,"row1");

	gtk_box_pack_start(GTK_BOX(outer_frame),hbox,TRUE,TRUE, 0 );
	
	
	
	char tmp_name[64];
	snprintf(tmp_name,64,"item0");
	GtkWidget * boxitem = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);	
	gtk_widget_set_size_request(boxitem,20,-1);
	gtk_widget_set_name(boxitem,tmp_name);
	
	gtk_box_pack_start(GTK_BOX(hbox),boxitem,TRUE,TRUE, 0 );

	snprintf(tmp_name,64,"item1");
	boxitem = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);	
	gtk_widget_set_size_request(boxitem,20,-1);
	gtk_widget_set_name(boxitem,tmp_name);
	
	gtk_box_pack_start(GTK_BOX(hbox),boxitem,TRUE,TRUE, 0 );

	snprintf(tmp_name,64,"item2");
	boxitem = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);	
	gtk_widget_set_size_request(boxitem,60,-1);
	gtk_widget_set_name(boxitem,tmp_name);
	
	gtk_box_pack_start(GTK_BOX(hbox),boxitem,TRUE,TRUE, 0 );


	print_widget_hierarchy(outer_frame, 0);
}


PageAnalysis::PageAnalysis(std::string name) {

	css_name = name;

	//------------------OUTER FRAME---------------------
	// gtk_widget_add_css_class (button, "fancy");
	outer_frame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_name(outer_frame, css_name.c_str());

	//--------------------CONTENT-----------------------
	Content();

	
	// Add to Stack
	gtk_stack_add_titled(GTK_STACK(stack), outer_frame, css_name.c_str(), "ANALYSIS");
	// STYLE
	// PageStyle();
	
}

PageAnalysis::~PageAnalysis(void) {
	cout << "destructor called\n";
}

/*
https://docs.gtk.org/gtk3/css-properties.html
https://docs.gtk.org/gtk3/css-overview.html#selectors
*/
void PageAnalysis::PageStyle(void) {

	string style = R"(
		
	)";

	/*
		Per widget context? seems awful
		// GtkStyleContext *context = gtk_widget_get_style_context(outer_frame);
		// gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	*/
	
	/*
		CAUTION : GLOBAL CONTEXT
	*/
	GtkCssProvider *provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(provider, style.c_str(), -1, NULL);

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}