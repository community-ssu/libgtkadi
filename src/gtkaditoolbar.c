/* vim:set ts=2 sw=4 sts=2 et cindent: */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is GtkAdi library.
 *
 * The Initial Developer of the Original Code is
 * Leonid Zolotarev <leonid.zolotarev@gmail.com>.
 * Portions created by the Initial Developer are Copyright (C) 2003-2007
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): Leonid Zolotarev <leonid.zolotarev@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "../include/gtkaditoolbar.h"

//#define GETTEXT_PACKAGE "gtk20"
#include <glib/gi18n-lib.h>
#include "gtkadistock.h"
#include "gtkadiutils.h"
#include "gtkadicmd.h"

#ifdef HILDON_SUPPORT
#define GTK_ADI_TOOLBAR_PAN_MODE_STR    "P"
#define GTK_ADI_TOOLBAR_BOX_MODE_STR    "B"
#define GTK_ADI_TOOLBAR_BOX_VIEW_STR    "B"
#define GTK_ADI_TOOLBAR_TAB_VIEW_STR    "T"
#define GTK_ADI_TOOLBAR_WIN_VIEW_STR    "W"
#else
#define GTK_ADI_TOOLBAR_PAN_MODE_STR    "Paned"
#define GTK_ADI_TOOLBAR_BOX_MODE_STR    "Box"
#define GTK_ADI_TOOLBAR_BOX_VIEW_STR    "Box View"
#define GTK_ADI_TOOLBAR_TAB_VIEW_STR    "Tab View"
#define GTK_ADI_TOOLBAR_WIN_VIEW_STR    "Win View"
#endif /* HILDON_SUPPORT */

/* here are local prototypes */
static void gtk_adi_toolbar_class_init (GtkAdiToolbarClass * c);
static void gtk_adi_toolbar_init (GtkAdiToolbar * self);
static void gtk_adi_toolbar_mode_changed (GtkAdiToolbar * self, gpointer user_data);
static void gtk_adi_toolbar_view_changed (GtkAdiToolbar * self, gpointer user_data);
static void gtk_adi_toolbar_fix_toggled (GtkAdiToolbar * self, gpointer user_data);
static void gtk_adi_toolbar_post_init (GtkAdiToolbar * self);

/* pointer to the class of our parent */
static GtkToolbarClass *parent_class = NULL;

GType
gtk_adi_toolbar_get_type (void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof (GtkAdiToolbarClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gtk_adi_toolbar_class_init,
			(GClassFinalizeFunc) NULL,
			NULL /* class_data */,
			sizeof (GtkAdiToolbar),
			0 /* n_preallocs */,
			(GInstanceInitFunc) gtk_adi_toolbar_init,
			NULL
		};

		type = g_type_register_static (GTK_TYPE_TOOLBAR, "GtkAdiToolbar", &info, (GTypeFlags)0);
	}

	return type;
}

/* a macro for creating a new object of our type */
#define GET_NEW ((GtkAdiToolbar *)g_object_new(gtk_adi_toolbar_get_type(), NULL))

static void
gtk_adi_toolbar_finalize(GObject *obj_self)
{
	GtkAdiToolbar *self G_GNUC_UNUSED = GTK_ADI_TOOLBAR (obj_self);
	
	ADI_TRACE("Toolbar finalize: %s", "begin");
	
	if(G_OBJECT_CLASS(parent_class)->finalize) {
		(* G_OBJECT_CLASS(parent_class)->finalize)(obj_self);
	}

	ADI_TRACE("Toolbar finalize: %s", "end");
}

static void 
gtk_adi_toolbar_class_init (GtkAdiToolbarClass * c)
{
	GObjectClass *g_object_class G_GNUC_UNUSED = (GObjectClass*) c;

	parent_class = g_type_class_ref (GTK_TYPE_TOOLBAR);

	g_object_class->finalize = gtk_adi_toolbar_finalize;
}

static void 
gtk_adi_toolbar_init (GtkAdiToolbar * self)
{
	self->cmd = NULL;
	self->tooltips = NULL;
	self->btn_new = NULL;
	self->btn_new_h = NULL;
	self->btn_new_v = NULL;
	self->sep_new = NULL;
	self->btn_cls = NULL;
	self->btn_cls_all = NULL;
	self->sep_cls = NULL;
	self->btn_next = NULL;
	self->btn_prev = NULL;
	self->sep_nav = NULL;
	self->btn_tile_h = NULL;
	self->btn_tile_v = NULL;
	self->sep_tile = NULL;
	
	self->itm_mode = NULL;
	self->cmb_mode = NULL;
	self->tgl_fix = NULL;
	self->sep_mode = NULL;

	self->itm_view = NULL;
	self->cmb_view = NULL;
	self->sep_view = NULL;

	self->btn_new = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_NEW));
	gtk_container_add (GTK_CONTAINER (self), self->btn_new);

	self->btn_new_h = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_NEW_HORIZONTALLY));
	gtk_container_add (GTK_CONTAINER (self), self->btn_new_h);

	self->btn_new_v = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_NEW_VERTICALLY));
	gtk_container_add (GTK_CONTAINER (self), self->btn_new_v);

	self->sep_new = (GtkWidget*) gtk_separator_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->sep_new);

	self->btn_cls = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_CLOSE));
	gtk_container_add (GTK_CONTAINER (self), self->btn_cls);
	
	self->btn_cls_all = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_CLOSE_ALL));
	gtk_container_add (GTK_CONTAINER (self), self->btn_cls_all);
	
	self->sep_cls = (GtkWidget*) gtk_separator_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->sep_cls);

	self->btn_next = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_NEXT));
	gtk_container_add (GTK_CONTAINER (self), self->btn_next);
	
	self->btn_prev = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_PREVIOUS));
	gtk_container_add (GTK_CONTAINER (self), self->btn_prev);
	
	self->sep_nav = (GtkWidget*) gtk_separator_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->sep_nav);

	self->btn_tile_h = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_TILE_HORIZONTALLY));
	gtk_container_add (GTK_CONTAINER (self), self->btn_tile_h);

	self->btn_tile_v = GTK_WIDGET(gtk_tool_button_new_from_stock (GTK_ADI_STOCK_TILE_VERTICALLY));
	gtk_container_add (GTK_CONTAINER (self), self->btn_tile_v);

	self->sep_tile = (GtkWidget*) gtk_separator_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->sep_tile);

	/* Mode */
	
	self->itm_mode = (GtkWidget*) gtk_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->itm_mode);

	self->cmb_mode = gtk_combo_box_new_text ();
	gtk_container_add (GTK_CONTAINER (self->itm_mode), self->cmb_mode);
	gtk_combo_box_append_text (GTK_COMBO_BOX (self->cmb_mode), _(GTK_ADI_TOOLBAR_PAN_MODE_STR));
	gtk_combo_box_append_text (GTK_COMBO_BOX (self->cmb_mode), _(GTK_ADI_TOOLBAR_BOX_MODE_STR));

	self->tgl_fix = GTK_WIDGET(gtk_toggle_tool_button_new_from_stock (GTK_ADI_STOCK_FIX));
	gtk_container_add (GTK_CONTAINER (self), self->tgl_fix);
	
	self->sep_mode = (GtkWidget*) gtk_separator_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->sep_mode);

	gtk_combo_box_set_active (GTK_COMBO_BOX(self->cmb_mode),0);

	/* View */
	
	self->itm_view = (GtkWidget*) gtk_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->itm_view);

	self->cmb_view = gtk_combo_box_new_text ();
	gtk_container_add (GTK_CONTAINER (self->itm_view), self->cmb_view);
	gtk_combo_box_append_text (GTK_COMBO_BOX (self->cmb_view), _(GTK_ADI_TOOLBAR_BOX_VIEW_STR));
	gtk_combo_box_append_text (GTK_COMBO_BOX (self->cmb_view), _(GTK_ADI_TOOLBAR_TAB_VIEW_STR));
	gtk_combo_box_append_text (GTK_COMBO_BOX (self->cmb_view), _(GTK_ADI_TOOLBAR_WIN_VIEW_STR));
	
	self->sep_view = (GtkWidget*) gtk_separator_tool_item_new ();
	gtk_container_add (GTK_CONTAINER (self), self->sep_view);

	gtk_combo_box_set_active (GTK_COMBO_BOX(self->cmb_view),0);
}

static void 
gtk_adi_toolbar_mode_changed (GtkAdiToolbar * self, gpointer user_data)
{
	gint value = 0;
	GtkAdiMode mode = GTK_ADI_PANED;

	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	value = gtk_combo_box_get_active ( GTK_COMBO_BOX (user_data) );

	switch ( value ) {
	case 1:
		mode = GTK_ADI_BOX;
		break;
	default:
		break;
	}
	gtk_adi_cmd_mode_changed (GTK_ADI_CMD(self->cmd), mode);
}


static void 
gtk_adi_toolbar_view_changed (GtkAdiToolbar * self, gpointer user_data)
{
	gint value = 0;
	GtkAdiViewType view = GTK_ADI_VIEW_BOX;

	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	value = gtk_combo_box_get_active ( GTK_COMBO_BOX (user_data) );
	
	ADI_TRACE("Toolbar: %s", "view changed");
	
	switch ( value ) {
	case 1:
		view = GTK_ADI_VIEW_TAB;
		break;
	case 2:
		view = GTK_ADI_VIEW_WIN;
		break;
	default:
		break;
	}
	gtk_adi_cmd_view_changed (GTK_ADI_CMD(self->cmd), view);
}

void 
gtk_adi_toolbar_mode_set (GtkAdiToolbar * self, GtkAdiMode mode)
{
	gint num = 0;

	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	if (mode == GTK_ADI_BOX) {
		num = 1;
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX(self->cmb_mode),num);
}

void
gtk_adi_toolbar_view_set (GtkAdiToolbar *self, GtkAdiViewType view)
{
	gint num = 0;

	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	switch (view) {
	case GTK_ADI_VIEW_TAB:
		num = 1;
		break;
	case GTK_ADI_VIEW_WIN:
		num = 2;
		break;
	default:
		break;
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX(self->cmb_view),num);
}

static void 
gtk_adi_toolbar_fix_toggled (GtkAdiToolbar * self, gpointer user_data)
{
	GtkAdiState state = GTK_ADI_MOVABLE;

	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	if ( gtk_toggle_tool_button_get_active ( GTK_TOGGLE_TOOL_BUTTON (user_data)
											) ) {
		state = GTK_ADI_FIXED;
	}
	gtk_adi_cmd_fix_toggled (GTK_ADI_CMD(self->cmd), state);
}

void 
gtk_adi_toolbar_fix_set (GtkAdiToolbar * self, GtkAdiState state)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	gtk_toggle_tool_button_set_active (
				   GTK_TOGGLE_TOOL_BUTTON (self->tgl_fix),
				   state == GTK_ADI_FIXED);
}

static void 
gtk_adi_toolbar_post_init (GtkAdiToolbar * self)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_new), self->tooltips, _("New Window"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_new_h), self->tooltips, _("Add New Window Horizontally"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_new_v), self->tooltips, _("Add New Window Vertically"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_cls), self->tooltips, _("Close"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_cls_all), self->tooltips, _("Close All"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_next), self->tooltips, _("Next"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_prev), self->tooltips, _("Previous"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_tile_h), self->tooltips, _("Tile Horizontally"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->btn_tile_v), self->tooltips, _("Tile Vertically"), NULL);
	gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (self->tgl_fix), self->tooltips, _("Fix"), NULL);

	g_signal_connect_swapped ((gpointer) self->btn_new, "clicked",
					G_CALLBACK (gtk_adi_cmd_new_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_new_h, "clicked",
					G_CALLBACK (gtk_adi_cmd_new_h_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_new_v, "clicked",
					G_CALLBACK (gtk_adi_cmd_new_v_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_cls, "clicked",
					G_CALLBACK (gtk_adi_cmd_close_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_cls_all, "clicked",
					G_CALLBACK (gtk_adi_cmd_close_all_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_next, "clicked",
					G_CALLBACK (gtk_adi_cmd_next_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_prev, "clicked",
					G_CALLBACK (gtk_adi_cmd_prev_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_tile_h, "clicked",
					G_CALLBACK (gtk_adi_cmd_tile_h_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->btn_tile_v, "clicked",
					G_CALLBACK (gtk_adi_cmd_tile_v_clicked),
					self->cmd);
	g_signal_connect_swapped ((gpointer) self->cmb_mode, "changed",
					G_CALLBACK (gtk_adi_toolbar_mode_changed),
					self);
	g_signal_connect_swapped ((gpointer) self->cmb_view, "changed",
					G_CALLBACK (gtk_adi_toolbar_view_changed),
					self);
	g_signal_connect_swapped ((gpointer) self->tgl_fix, "toggled",
					G_CALLBACK (gtk_adi_toolbar_fix_toggled),
					self);
}

GtkWidget * 
gtk_adi_toolbar_new (GtkObject * cmd)
{
	GtkWidget *widget = NULL;

	g_return_val_if_fail (cmd != NULL, (GtkWidget * )0);
	
	widget = GTK_WIDGET(GET_NEW);

	GTK_ADI_TOOLBAR(widget)->cmd = cmd;
	GTK_ADI_TOOLBAR(widget)->tooltips = gtk_adi_cmd_get_tooltips (GTK_ADI_CMD(cmd));
	gtk_adi_toolbar_post_init (GTK_ADI_TOOLBAR(widget));
	return widget;
}

void 
gtk_adi_toolbar_set_close_buttons (GtkAdiToolbar * self, gboolean enable)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	gtk_widget_set_sensitive (self->btn_cls    , enable);
	gtk_widget_set_sensitive (self->btn_cls_all, enable);
}

void 
gtk_adi_toolbar_set_nav_buttons (GtkAdiToolbar * self, gboolean next, gboolean prev)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	gtk_widget_set_sensitive (self->btn_next, next);
	gtk_widget_set_sensitive (self->btn_prev, prev);
}

void 
gtk_adi_toolbar_set_tile_buttons (GtkAdiToolbar * self, gboolean tile_h, gboolean tile_v)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_TOOLBAR (self));
	
	gtk_widget_set_sensitive (self->btn_tile_h, tile_h);
	gtk_widget_set_sensitive (self->btn_tile_v, tile_v);
}
