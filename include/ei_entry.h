#include "ei_types.h"
#include "ei_widget.h"

typedef struct ei_entry_t{
        ei_widget_t             widget;
	ei_color_t      	color;
        char*                   text;
        ei_font_t               text_font;
        ei_color_t              text_color;
        int                     char_max;
}ei_entry_t;


/* Fonctions spécifique à la classe de widget entry */
void ei_entry_configure(struct ei_widget_t *widget, ei_size_t *requested_size,
                        ei_color_t* color, char **text, ei_font_t *text_font,
                        ei_color_t *text_color, int *char_max);

void entry_geomnotifyfunc (struct ei_widget_t *widget, ei_rect_t rect);

void* entry_allocfunc ();

void entry_releasefunc (struct ei_widget_t* widget);

void entry_drawfunc(struct ei_widget_t* widget,
		     ei_surface_t		surface,
		     ei_surface_t		pick_surface,
		     ei_rect_t*		clipper);

void entry_setdefaultsfunc(struct ei_widget_t* widget);

void ei_entry_register_class();
