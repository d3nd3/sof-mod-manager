#pragma once

#include <string>
#include <iostream>
using namespace std;

extern GtkWidget * stack;

class PageHome {
public:
	PageHome(std::string name);
	~PageHome();

private:
	GtkWidget *outer_frame = nullptr;

	string css_name;

	void PageStyle(void);
	void Content(void);
};

class PageAnalysis {
public:
	PageAnalysis(std::string name);
	~PageAnalysis();

private:
	GtkWidget *outer_frame = nullptr;

	string css_name;

	void PageStyle(void);
	void Content(void);
};
extern void print_widget_hierarchy(GtkWidget *widget, gint indent);

