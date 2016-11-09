#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_entry.h"
#include "ei_types.h"
#include "ei_classes.h"

ei_bool_t limite_entry(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
        ei_widget_t* root = ei_app_root_widget();
        ei_widget_t *entry = root->children_head->children_tail->children_tail;
        ((ei_entry_t*) entry)->char_max = 7;
        return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

        ei_widget_t*	toplevel;
        ei_size_t       toplevel_size           = {500, 200};
        int             toplevel_border         = 7;
        ei_color_t      toplevel_color          = {0x99, 0x99, 0x99, 0xff};
	int		toplevel_x		= 25;
	int		toplevel_y		= 50;
        ei_size_t*      toplevel_min_size;
        toplevel_min_size = calloc (1, sizeof(ei_size_t));
        toplevel_min_size->width = 265;
        toplevel_min_size->height = 150;

	ei_widget_t*	frame;
	ei_size_t   	frame_size       	= {0, 0};
	int		frame_x			= 0;
	int		frame_y			= 0;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;
	float		frame_rel_width		= 1.0;
	float		frame_rel_height	= 1.0;
        char*           frame_text              = "Saisissez votre nom :";
	ei_anchor_t	frame_anchor		= ei_anc_north;

	ei_widget_t*	entry;
	ei_size_t	entry_size		= {200,30};
	ei_anchor_t	entry_anchor		= ei_anc_center;
	int		entry_x			= 0;
	int		entry_y			= 0;
	float		entry_rel_x		= 0.5;
	float		entry_rel_y		= 0.5;
	int		entry_char_max		= 30;

/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	/* Create the application and change the color of the background. */
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the toplevel on screen. */
        toplevel = ei_widget_create("toplevel", ei_app_root_widget());
        ei_toplevel_configure(toplevel, &toplevel_size, &toplevel_color, &toplevel_border, NULL, NULL, NULL, &(toplevel_min_size));
        ei_place(toplevel, NULL, &toplevel_x, &toplevel_y, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", toplevel);
	ei_frame_configure(frame, &frame_size, &frame_color,
			    &frame_border_width, &frame_relief, &frame_text, NULL, NULL, &frame_anchor,
                           NULL, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, &frame_rel_width, &frame_rel_height);
        /* Création d'un bouton */
        ei_button_t *button = (ei_button_t *) ei_widget_create("button", frame);
        float rel_x = 1.0;
        float rel_y = 1.0;
        char *text_button = "Limiter à 7 caractères";
        ei_anchor_t anch_button= ei_anc_southeast;
        ei_callback_t func = limite_entry;
        ei_button_configure((ei_widget_t *)button, NULL, &frame_color,
			    NULL, NULL, NULL, &text_button, NULL, NULL, NULL, NULL, NULL, NULL, &func, NULL);
        ei_place((ei_widget_t *) button, &anch_button, NULL, NULL, NULL, NULL, &rel_x, &rel_y, NULL, NULL);
        
	/* Create, configure and place the entry on screen. */
        entry = ei_widget_create("entry", frame);
        ei_entry_configure(entry, &entry_size, NULL, NULL, NULL, NULL, &entry_char_max);
        ei_place(entry, &entry_anchor, &entry_x, &entry_y, NULL, NULL, &entry_rel_x, &entry_rel_y, NULL, NULL);
        
	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
