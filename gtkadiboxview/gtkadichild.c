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

#include "gtkadichild.h"
#include "gtkadiboxtitle.h"

/* here are local prototypes */
static void gtk_adi_child_class_init (GtkAdiChildClass * c);
static void gtk_adi_child_init (GtkAdiChild * self);
void on_tab_elem_show (GtkWidget *widget, GtkAdiChild * self);

/* pointer to the class of our parent */
static GtkFrameClass *parent_class = NULL;

GType
gtk_adi_child_get_type (void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof (GtkAdiChildClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) gtk_adi_child_class_init,
			(GClassFinalizeFunc) NULL,
			NULL /* class_data */,
			sizeof (GtkAdiChild),
			0 /* n_preallocs */,
			(GInstanceInitFunc) gtk_adi_child_init,
			NULL
		};

		type = g_type_register_static (GTK_TYPE_FRAME, "GtkAdiChild", &info, (GTypeFlags)0);
	}

	return type;
}

/* a macro for creating a new object of our type */
#define GET_NEW ((GtkAdiChild *)g_object_new(gtk_adi_child_get_type(), NULL))

static void 
gtk_adi_child_class_init (GtkAdiChildClass * c)
{
	parent_class = g_type_class_ref (GTK_TYPE_FRAME);
}


static void 
gtk_adi_child_init (GtkAdiChild * self)
{
	gtk_frame_set_label_align (GTK_FRAME (self), 0, 0);
	GTK_WIDGET_SET_FLAGS (self, GTK_CAN_FOCUS);
	
	self->show_tab = TRUE;
	
	self->box = gtk_vbox_new (FALSE, 0);
	
	gtk_container_add (GTK_CONTAINER (self), self->box);
	
	self->title = gtk_adi_box_title_new ();
	gtk_box_pack_start (GTK_BOX (self->box), self->title, FALSE, FALSE, 0);
	gtk_adi_title_set_child (GTK_ADI_TITLE(self->title), GTK_WIDGET(self));
	
	self->separator = gtk_hseparator_new ();
	gtk_box_pack_start (GTK_BOX (self->box), self->separator, FALSE, FALSE, 0);
	
	g_signal_connect(GTK_WIDGET(self->separator), "show", (GCallback) on_tab_elem_show, self);
	g_signal_connect(GTK_WIDGET(self->title), "show", (GCallback) on_tab_elem_show, self);
}



GtkWidget * 
gtk_adi_child_new (void)
{
	return GTK_WIDGET(GET_NEW);
}

void 
gtk_adi_child_set_active (GtkAdiChild * self)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	gtk_adi_box_title_set_active (GTK_ADI_BOX_TITLE(self->title));
}

void 
gtk_adi_child_set_group (GtkAdiChild * self, GSList * group)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	
	gtk_adi_box_title_set_group (GTK_ADI_BOX_TITLE(self->title), group);
}

void 
gtk_adi_child_set_color (GtkAdiChild * self, GtkObject * adi_color)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	g_return_if_fail (adi_color != NULL);
	
	gtk_adi_box_title_set_color (GTK_ADI_BOX_TITLE(self->title), adi_color);
}

void 
gtk_adi_child_set_widget (GtkAdiChild * self, GtkWidget * widget)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	g_return_if_fail (widget != NULL);
	
	self->widget = widget;
	gtk_box_pack_start (GTK_BOX (self->box), self->widget, TRUE, TRUE, 0);
}

void 
gtk_adi_child_remove_widget (GtkAdiChild * self)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	g_return_if_fail (self->widget != NULL);
	
	gtk_container_remove (GTK_CONTAINER (self->box), self->widget);
	self->widget = NULL;
}

void 
gtk_adi_child_set_parent (GtkAdiChild * self, GtkWidget * adi_parent)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	g_return_if_fail (adi_parent != NULL);
	
	gtk_adi_title_set_parent (GTK_ADI_TITLE(self->title), adi_parent);
}

void 
gtk_adi_child_set_text (GtkAdiChild * self, const gchar * str)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	
	if (str) {
		gtk_adi_title_set_text (GTK_ADI_TITLE(self->title), str);
	}
}

void 
gtk_adi_child_set_icon (GtkAdiChild *self, const GdkPixbuf *icon)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	
	if (icon) {
		gtk_adi_title_set_icon (GTK_ADI_TITLE(self->title), icon);
	}
}

GSList * 
gtk_adi_child_get_group (GtkAdiChild * self)
{
	g_return_val_if_fail (self != NULL, (GSList * )0);
	g_return_val_if_fail (GTK_IS_ADI_CHILD (self), (GSList * )0);
	
	return gtk_adi_box_title_get_group (GTK_ADI_BOX_TITLE(self->title));
}

void
gtk_adi_child_set_layout (GtkAdiChild *self, GtkAdiLayout layout)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	
	gtk_adi_title_set_layout (GTK_ADI_TITLE(self->title), layout);
}

GtkAdiLayout
gtk_adi_child_get_layout (GtkAdiChild *self)
{
	g_return_val_if_fail (self != NULL, GTK_ADI_HORIZONTAL);
	g_return_val_if_fail (GTK_IS_ADI_CHILD (self), GTK_ADI_HORIZONTAL);
	
	return gtk_adi_title_get_layout (GTK_ADI_TITLE(self->title));
}

void
gtk_adi_child_set_close_button (GtkAdiChild *self, gboolean enabled)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	
	gtk_adi_title_set_close_button (GTK_ADI_TITLE(self->title), enabled);
}

void
gtk_adi_child_set_tab (GtkAdiChild *self, gboolean enabled)
{
	g_return_if_fail (self != NULL);
	g_return_if_fail (GTK_IS_ADI_CHILD (self));
	
	self->show_tab = enabled;
	on_tab_elem_show(self->separator, self);
	on_tab_elem_show(self->title, self);
}

void
on_tab_elem_show (GtkWidget *widget, GtkAdiChild * self)
{
    if (!self->show_tab)
    {
	if (GTK_WIDGET_VISIBLE(widget))
	{
	    gtk_widget_hide(widget);
	}
    }
    else
    {
	if (!GTK_WIDGET_VISIBLE(widget))
	{
	    gtk_widget_show(widget);
	}
    }
}
