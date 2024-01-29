/*
Copyright 2024 Carlos Henrique

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/extensions/XTest.h>

int auto_clicking = 0;
unsigned int click_interval = 1000;

void toggle_auto_clicking(GtkToggleButton* toggle_button) {
    auto_clicking = gtk_toggle_button_get_active(toggle_button);
    
    if (auto_clicking) {
        printf("Auto clicker enabled\n");
    } else {
        printf("Auto clicker disabled\n");
    }
}

void set_click_interval(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;
    GtkDialogFlags flags;
    int response;
    
    flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Set Click Interval", GTK_WINDOW(data), flags,
                                         "Ok", GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), g_strdup_printf("%u", click_interval));
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    
    gtk_widget_show_all(dialog);
    
    response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        sscanf(text, "%u", &click_interval);
        printf("Click interval set to %u milliseconds\n", click_interval);
    }
    
    gtk_widget_destroy(dialog);
}

gboolean auto_clicker(gpointer data) {
    if (auto_clicking) {
        Display *display;
        XEvent event;
        
        display = XOpenDisplay(NULL);
        if (display == NULL) {
            fprintf(stderr, "Error opening connection to X11 server\n");
            return FALSE;
        }
        
        memset(&event, 0, sizeof(event));
        event.xbutton.button = Button1;
        event.xbutton.same_screen = True;
        event.xbutton.subwindow = DefaultRootWindow(display);
        
        /* Simulate left mouse button press */
        XTestFakeButtonEvent(display, Button1, True, 0);
        XFlush(display);
        
        /* Simulate left mouse button release */
        event.type = ButtonRelease;
        XTestFakeButtonEvent(display, Button1, False, 0);
        XFlush(display);
        
        XCloseDisplay(display);
    }
    
    return TRUE;
}

gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->button == GDK_BUTTON_SECONDARY) {
        gtk_menu_popup(GTK_MENU(data), NULL, NULL, NULL, NULL, event->button, event->time);
    }
    return FALSE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    GtkWidget *toggle_button = gtk_toggle_button_new_with_label("Auto Clicker");
    g_signal_connect(toggle_button, "toggled", G_CALLBACK(toggle_auto_clicking), NULL);
    gtk_container_add(GTK_CONTAINER(window), toggle_button);
    
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *item = gtk_menu_item_new_with_label("Set Click Interval");
    g_signal_connect(item, "activate", G_CALLBACK(set_click_interval), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    
    GtkWidget *context_menu = gtk_menu_new();
    item = gtk_menu_item_new_with_label("Set Click Interval");
    g_signal_connect(item, "activate", G_CALLBACK(set_click_interval), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(context_menu), item);
    
    g_signal_connect(toggle_button, "button-press-event", G_CALLBACK(on_button_press_event), context_menu);
    gtk_widget_add_events(toggle_button, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
    
    gtk_widget_show_all(window);
    
    g_timeout_add(click_interval, auto_clicker, NULL);
    
    gtk_main();
    
    return 0;
}
