#include "ei_geometryclasses.h"
#include "ei_geometrymanager.h"
#include "ei_widget.h"
#include "ei_classes.h"
#include "ei_application.h"
#include "ei_globs.h"
#include <string.h>

ei_point_t calcul_point_ancrage(ei_rect_t* rect, ei_anchor_t *anchor)
{
	/*simplification expressions*/

	int x=rect->top_left.x; 
	int y=rect->top_left.y;

	int width=rect->size.width;
	int height=rect->size.height;

	ei_point_t ancrage;

	if (anchor != NULL){
                switch (*anchor){

                case ei_anc_center :
			ancrage.x= x - width/2; 
			ancrage.y= y - height/2;
                        break;

                case ei_anc_north :
			ancrage.x= x - width/2; 
			ancrage.y= y;
                        break;

                case ei_anc_northeast :
			ancrage.x= x - width; 
			ancrage.y= y;
                        break;

                case ei_anc_east :
			ancrage.x= x - width; 
			ancrage.y= y - height/2;
                        break;

                case ei_anc_southeast :
			ancrage.x= x - width; 
			ancrage.y= y - height;
                        break;

                case ei_anc_south :
			ancrage.x= x - width/2; 
			ancrage.y= y - height;
                        break;

                case ei_anc_southwest :
			ancrage.x= x; 
			ancrage.y= y - height;
                        break;

                case ei_anc_west :
			ancrage.x= x; 
			ancrage.y= y - height/2;
                        break;

                case ei_anc_northwest :
			ancrage.x= x; 
			ancrage.y= y;
                        break;

                case ei_anc_none :
			/*cas par défault northwest*/
			ancrage.x= x; 
			ancrage.y= y;
                        break;
                }
        }else{
		/*cas par défault northwest*/
		ancrage.x= x; 
		ancrage.y= y;
	}
	return ancrage;
}


void placer_screen_location(struct ei_widget_t *widget)
{
        if (widget->parent == NULL) {
                /* La root_frame reste fixe */
                return;
        }
        ei_placer_param_t *placer_settings = (ei_placer_param_t *) widget->geom_params;
       
        /* -- Position -- */
        ei_rect_t new_rect;

        new_rect.top_left.x = placer_settings->x;
        new_rect.top_left.y = placer_settings->y;
        /* Pour la position relative le facteur s'applique à la largeur ou
           hauteur du parent */
        new_rect.top_left.x += widget->parent->content_rect->top_left.x
                + (placer_settings->rel_x) * widget->parent->content_rect->size.width;
        new_rect.top_left.y += widget->parent->content_rect->top_left.y
                + (placer_settings->rel_y) * widget->parent->content_rect->size.height;

        /* -- Taille -- */
        new_rect.size.width = placer_settings->width;
        new_rect.size.height = placer_settings->height;
        if (!strcmp(widget->wclass->name,"toplevel")) {
                ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
                /* La taille d'une toplevel être supérieure à la taille min */
                if (new_rect.size.width < toplevel->min_size->width){
                        new_rect.size.width = toplevel->min_size->width;
                }
                if (new_rect.size.height < toplevel->min_size->height){
                        new_rect.size.height = toplevel->min_size->height;
                }

                new_rect.size.width += 2*toplevel->border_width;
                new_rect.size.height += toplevel->border_width
                        + toplevel->height_header;
        }
        new_rect.size.width += (placer_settings->rel_width) * widget->parent->content_rect->size.width;
        new_rect.size.height += (placer_settings->rel_height) * widget->parent->content_rect->size.height;

        /* -- Anchor -- */
        /* Mise à jour de la position : top_left */
        ei_point_t anchor_point = calcul_point_ancrage(&new_rect,
                                                       &(placer_settings->anchor));
        /*On met à jour le screen_location */
        widget->screen_location.top_left.x = anchor_point.x;
        widget->screen_location.top_left.y = anchor_point.y;
        widget->screen_location.size.width = new_rect.size.width;
        widget->screen_location.size.height = new_rect.size.height;
        /* On met à jour le content_rect */
        (*widget->wclass->geomnotifyfunc)(widget, widget->screen_location);

        /* Test de la positivité des valeurs */
        if (widget->content_rect->size.width < 0) {
                widget->content_rect->size.width = 0;
        }
        if  (widget->content_rect->size.height < 0){
                widget->content_rect->size.height = 0;
        }
}

void placer_button_toplevel(ei_widget_t *widget)
{
        ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
        ei_widget_t *close_button = widget->next_sibling;
        ei_anchor_t close_button_anch = ei_anc_center;
        ei_color_t red = {0xFF, 0x00, 0x00, 0xFF};
        int radius_header = toplevel->height_header/2;
        int radius_close_button = toplevel->height_header/4;
        int border_close_button = toplevel->height_header/8;
        ei_size_t close_button_size = ei_size(radius_header,
                                              radius_header);
        ei_point_t close_button_point = ei_point(radius_header,
                                                 radius_header);
        close_button_point.x += widget->screen_location.top_left.x - 
                widget->parent->content_rect->top_left.x;
        close_button_point.y += widget->screen_location.top_left.y - 
                widget->parent->content_rect->top_left.y;
        ei_button_configure(close_button, &close_button_size,
                            &red, &border_close_button,
                            &radius_close_button, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL, NULL,
                            NULL, NULL);
        ei_place(close_button, &close_button_anch,
                 &(close_button_point.x),
                 &(close_button_point.y), NULL, NULL, NULL, NULL,
                 NULL, NULL);
}

void placer_frame_toplevel(ei_widget_t *widget)
{
        ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
        ei_widget_t *resize_zone = widget->next_sibling->next_sibling;
        ei_color_t resize_zone_color = eclaircir_assombrir(toplevel->color,100, -1);
        ei_anchor_t resize_zone_anch = ei_anc_southeast;
        ei_size_t resize_zone_size;
        if (toplevel->border_width >= min_resize_zone){
                resize_zone_size = ei_size(2*toplevel->border_width,
                                           2*toplevel->border_width);
        } else {
                resize_zone_size = ei_size(2*min_resize_zone,
                                           2*min_resize_zone);
        }
                        
        ei_point_t resize_zone_point = ei_point(widget->screen_location.size.width,
                                                widget->screen_location.size.height);
        resize_zone_point.x += widget->screen_location.top_left.x -
                widget->parent->content_rect->top_left.x;
        resize_zone_point.y += widget->screen_location.top_left.y -
                widget->parent->content_rect->top_left.y;
        int resize_zone_border = 0;
        ei_frame_configure(resize_zone, &resize_zone_size,
                           &(resize_zone_color),
                           &resize_zone_border, NULL, NULL,
                           NULL, NULL, NULL, NULL, NULL, NULL);
        ei_place(resize_zone, &resize_zone_anch,
                 &(resize_zone_point.x), &(resize_zone_point.y),
                 NULL, NULL, NULL, NULL, NULL, NULL);
}

/* void placer_runfunc(struct ei_widget_t *widget); */
void placer_runfunc(struct ei_widget_t *widget)
{
        /* Calcul de la géométrie sur le widget*/
        if ((widget == NULL)
            || (widget->geom_params == NULL)
            || (strcmp(widget->geom_params->manager->name, "placer") != 0)){
                return;
        }
        placer_screen_location(widget);
        if (strcmp(widget->wclass->name, "toplevel") == 0){
                /* On doit placer le bouton et le redimensionnement */
                ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
                if (toplevel->closable == EI_TRUE){
                        placer_button_toplevel(widget);
                }
                if  (toplevel->resizable){
                        placer_frame_toplevel(widget);
                }
        }
        
        /* On calcule la géométrie pour les fils */
        ei_widget_t* cour = widget->children_head;
        while (cour != NULL) {
                if (cour->geom_params != NULL){
                        (*cour->geom_params->manager->runfunc)(cour);
                }
                if (!strcmp(cour->wclass->name,"toplevel")) {
                        cour = cour->next_sibling->next_sibling;
                }
                cour = cour->next_sibling;
        }
}


/* void placer_releasefunc(struct ei_widget_t *widget); */
void placer_releasefunc(struct ei_widget_t *widget)
{
        /* La disparition d'un widget nécessite de répercuter l'action
           sur ses fils */
        ei_widget_t *cour = widget->children_head;
        while (cour != NULL){
                ei_geometrymanager_unmap(cour);
                cour = cour->next_sibling;
        }
}
